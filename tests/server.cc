#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <atomic>

#include <sys/wait.h>
#include <signal.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
#include <map>

#include <mutex>
#include <condition_variable>
#include <algorithm>
#include "util.h"
#include "client_message.pb.h"
#include "server_message.pb.h"


int nb_server_threads   = 2;
int port                = 1025;
constexpr int backlog   = 1024; // how many pending connections the queue will hold

std::atomic<uint64_t>   _bytes{0};
std::atomic<uint64_t>   _messages{0};
std::atomic<int64_t>    number{0};

struct thread_args {
    std::vector<int> listening_socket;
    std::mutex mtx;
    std::condition_variable cv;
    std::map<int, uint64_t> total_bytes;
};


bool process_request(std::unique_ptr<char[]>& buffer, struct thread_args* ptr, int csock, int buf_size) {
    _messages.fetch_add(1);
    _bytes.fetch_add(buf_size);
    sockets::client_msg _msg;
    size_t payload_size = buf_size - 4;
    std::string tmp(buffer.get() + 4, payload_size);
    _msg.ParseFromString(tmp);


    for (auto i = 0; i < _msg.ops_size(); i++) {
        sockets::client_msg::OperationData op = _msg.ops(i);

        switch (op.type()) {
            case sockets::client_msg::ADD:
                {
                    number.fetch_add(op.argument());
                    continue;
                }
            case sockets::client_msg::SUB:
                {
                    number.fetch_sub(op.argument());
                    continue;
                }
            case sockets::client_msg::TERMINATION:
                {
                    // send_termination message
                    sockets::server_msg response;
                    response.set_result(number.load());
                    response.set_total_bytes(ptr->total_bytes[csock]);


                    std::string server_msg;

                    response.SerializeToString(&server_msg);

                    char _number[4];
                    size_t sz = server_msg.size();
                    convertIntToByteArray(_number, sz);
                    std::unique_ptr<char[]> buf = std::make_unique<char[]>(sz+4);
                    ::memcpy(buf.get(), _number, 4);
                    ::memcpy(buf.get()+4, server_msg.c_str(), sz);
                    secure_send(csock, buf.get(), 4 + sz);

                    return true;


                }
            case sockets::client_msg::RANDOM_DATA:
                {
                    continue;
                }
            default:
                exit(2);

        }
    }
    return false;

}

void server(void* args) {
    std::vector<int> listening_socket;
    auto ptr = reinterpret_cast<struct thread_args*>(args);

    fd_set rfds;
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    int max_fd = -1;

    // block until at least one connection
    {
        std::unique_lock<std::mutex> lk(ptr->mtx);
        auto nb_connections = ptr->listening_socket.size();
        while (nb_connections == 0) {
            std::cout << "no connections \n";
            ptr->cv.wait(lk);

            nb_connections = ptr->listening_socket.size();
        }

        FD_ZERO(&rfds);
        for (auto rfd : ptr->listening_socket) {
            FD_SET(rfd, &rfds);
            max_fd = (max_fd < rfd) ? rfd : max_fd;

        }
        listening_socket = ptr->listening_socket;
    }
    
    int retval = 0;
    while (1) {
        retval = select((max_fd+1), &rfds, NULL, NULL, &tv);
        if (retval == 0) {
            std::cout << "update connections\n";
            // update connections
            {
                std::unique_lock<std::mutex> lk(ptr->mtx);
                auto nb_connections = ptr->listening_socket.size();
                while (nb_connections == 0) {
                    std::cout << "no connections \n";
                    ptr->cv.wait(lk);

                    nb_connections = ptr->listening_socket.size();
                }
                FD_ZERO(&rfds);
                max_fd = -1;
                for (auto s_fd : ptr->listening_socket) {
                    FD_SET(s_fd, &rfds);
                    max_fd = (max_fd < s_fd) ? s_fd : max_fd;

                }
                listening_socket = ptr->listening_socket;
            }
            continue;
        }
        else if (retval < 0) {
            std::cout << "timeout\n";
            continue;
        }


        {
            uint64_t bytecount = 0;
            std::unique_ptr<char[]>buffer;
            for (auto csock : listening_socket) {
                if(FD_ISSET(csock, &rfds)) {
                    if ((bytecount = secure_recv(csock, buffer, -1))  <= 0) {
                        if (bytecount == 0) {
                            {
                                std::unique_lock<std::mutex> lk(ptr->mtx);
                                auto it = std::find(ptr->listening_socket.begin(), ptr->listening_socket.end(), csock);
                                ptr->listening_socket.erase(it);

                                auto nb_connections = ptr->listening_socket.size();
                                while (nb_connections == 0) {
                                    std::cout << "packets: " << _messages.load() << " bytes: " << _bytes.load() << "\n";
                                    std::cout << "no connections \n";
                                    ptr->cv.wait(lk);

                                    nb_connections = ptr->listening_socket.size();
                                }

                                FD_ZERO(&rfds);
                                for (auto rfd : ptr->listening_socket) {
                                    FD_SET(rfd, &rfds);
                                    max_fd = (max_fd < rfd) ? rfd : max_fd;

                                }

                                listening_socket = ptr->listening_socket;
                                ptr->total_bytes.erase(csock);
                                break;

                            }

                        }

                    }
                    // process request
                    ptr->total_bytes[csock] += bytecount;

                    if (process_request(buffer, ptr, csock, bytecount)) {
                        std::cout << number.load() << "\n";
                        std::cout << "received termination msg\n";
                    }
                }

            }
        }
    }
    {
        std::unique_lock<std::mutex> lk(ptr->mtx);
        FD_ZERO(&rfds);
        max_fd = -1;
        for (auto s_fd : ptr->listening_socket) {
            FD_SET(s_fd, &rfds);
            max_fd = (max_fd < s_fd) ? s_fd : max_fd;

        }
    }
}



int main(int args, char* argv[]) {
    if (args < 5) {
        std::cerr << "usage: ./server <nb_server_threads> <port>\n";
    }

    nb_server_threads = std::atoi(argv[1]);
    port = std::atoi(argv[2]);

    std::vector<std::thread> threads;
    std::vector<std::unique_ptr<struct thread_args>> threads_args;

    for (int i = 0; i < nb_server_threads; i++) {
        auto ptr = std::make_unique<struct thread_args>();
        threads_args.emplace_back(std::move(ptr));
        threads.emplace_back(std::thread(server, threads_args.back().get()));
    }

    /* listen on sock_fd, new connection on new_fd */
    int sockfd, new_fd; 

    /* my address information */
    struct sockaddr_in my_addr; 

    /* connector.s address information */
    struct sockaddr_in their_addr; 
    socklen_t sin_size;

    int yes=1;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
        perror("setsockopt");
        exit(1);
    }

    my_addr.sin_family      = AF_INET;          // host byte order
    my_addr.sin_port        = htons(port);    // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY;       // automatically fill with my IP
    memset(&(my_addr.sin_zero), 0, 8);          // zero the rest of the struct

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, backlog) == -1) {
        perror("listen");
        exit(1);
    }


    uint64_t nb_clients = 0;
    while(1) {
        sin_size = sizeof(struct sockaddr_in);
        std::cout << "waiting for new connections ..\n";
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
            std::cerr << "accept() failed .. " << std::strerror(errno) << "\n";
            continue;
        }

        printf("Received request from Client: %s:%d\n", inet_ntoa(their_addr.sin_addr), port);
        {
            std::cout << "socket : " << new_fd << " matched to thread: " << (nb_clients % nb_server_threads) << "\n";
            std::unique_lock<std::mutex> lk(threads_args[nb_clients % nb_server_threads].get()->mtx);
            fcntl(new_fd, F_SETFL, O_NONBLOCK);
            threads_args[nb_clients % nb_server_threads].get()->listening_socket.push_back(new_fd);
            threads_args[nb_clients % nb_server_threads].get()->total_bytes.insert({new_fd, 0});
            threads_args[nb_clients % nb_server_threads].get()->cv.notify_one();
            nb_clients++;
        }
    }

    return 0;
}








