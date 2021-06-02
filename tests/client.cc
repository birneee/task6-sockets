#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <thread>
#include <vector>
#include <iostream>
#include <cstring>
#include <atomic>
#include "util.h"
#include "util_msg.h"
#include "client_message.pb.h"
#include "server_message.pb.h"


int nb_clients  = -1;
int port        = -1;
int nb_messages = -1;

std::atomic<int> threads_ids{0};
uint64_t last_result = 0;

void send_termination_msg(int sockfd, int64_t bytes_sent) {
    sockets::client_msg client_message;
    sockets::client_msg::OperationData* op =  client_message.add_ops();

    op->set_type(sockets::client_msg::TERMINATION);
    std::string client_msg;
    client_message.SerializeToString(&client_msg);

    char number[4];
    size_t sz = client_msg.size();
    std::cout << sz << "\n";
    convertIntToByteArray(number, sz);
    std::unique_ptr<char[]> buf = std::make_unique<char[]>(sz+4+1);
    buf[sz+4] = '\0';
    ::memcpy(buf.get(), number, 4);
    ::memcpy(buf.get()+4, client_msg.c_str(), sz);

    secure_send(sockfd, buf.get(), sz+4);
    bytes_sent += sz+4;

    std::unique_ptr<char[]> buf2;
    secure_recv(sockfd, buf2, -1);

    sockets::server_msg _msg;
    _msg.ParseFromString(buf2.get() + 4);


    //    std::cout << "server said : " << convertByteArrayToInt(number) << " bytes_sent : " << bytes_sent << "\n";
    if (_msg.total_bytes() != bytes_sent)
        exit(2);

    auto id = threads_ids.fetch_sub(1);
    if (id == 1)
        last_result = _msg.result();

}

void client(void* args) {
    threads_ids.fetch_add(1);
    int sockfd, numbytes;

    // connector.s address information
    struct sockaddr_in their_addr; 

    //***************************block of code finds the localhost IP
    
    /*
    char hostn[400]; //placeholder for the hostname
    char ipadd[400]; //placeholder for my IP address

    struct hostent *hostIP; //placeholder for the IP address

    //if the gethostname returns a name then the program will get the ip address using gethostbyname
    if((gethostname(hostn, sizeof(hostn))) == 0) {
        hostIP = gethostbyname(hostn); //the netdb.h function gethostbyname
    }
    else {
        printf("ERROR:FC4539 - IP Address not found."); //error if the hostname is not found
    }
    */
    //****************************************************************



    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    struct hostent *he      = reinterpret_cast<struct hostent*>(args);
    their_addr.sin_family   = AF_INET;
    their_addr.sin_port     = htons(port);
    their_addr.sin_addr     = *((struct in_addr *)he->h_addr);
    memset(&(their_addr.sin_zero), 0, 8);

    if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }

    int64_t bytes_sent = 0;
    int iterations = nb_messages;
    while (iterations > 0) {
        size_t size = 0;
        std::unique_ptr<char[]> buf = get_operation(size);
        if ((numbytes = secure_send(sockfd, buf.get(), size)) == -1) {
            std::cout << std::strerror(errno) << "\n";
            exit(1);
        }
        bytes_sent += numbytes;
        iterations--;
    }

    send_termination_msg(sockfd, bytes_sent);

    close(sockfd);
}


int main (int args, char* argv[]) {

    if (args < 5) {
        std::cerr << "usage: ./client <nb_threads> <hostname> <port> <nb_messages>\n";
    }

    nb_clients  = std::atoi(argv[1]);
    port        = std::atoi(argv[3]);
    nb_messages = std::atoi(argv[4]);

    struct hostent *he;
    if ((he = gethostbyname(argv[2])) == NULL) {
        exit(1);
    }

    // creating the client threads
    std::vector<std::thread> threads;

    for (int i = 0; i < nb_clients; i++) {
        threads.emplace_back(std::thread(client, he));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout <<  get_result(last_result) << "\n";

    std::cout << "** all threads joined **\n";
}
