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
 #include <sys/wait.h>
 #include <signal.h>
 #include <cstring>
 #include <iostream>
 #include <thread>
 #include <vector>
 #include <mutex>
 #include <condition_variable>
 #include <algorithm>
 #include <memory>
#include "solution/util.h"
#include "../util.h"

std::string random_alpharithmetic(size_t len) {
    std::string st;
    for (size_t i = 0; i < len; i++) 
        st += "d";
    return st;
}


int main(int args, char** argv) {
    // TEST 1
    auto st_payload = random_alpharithmetic(2);
    int payload_size = st_payload.size();
    char* payload = const_cast<char*>(st_payload.c_str());
    std::unique_ptr<char[]> dst1 = std::make_unique<char[]>(payload_size+4);
    std::unique_ptr<char[]> dst2 = std::make_unique<char[]>(payload_size+4);
    solution::construct_message(dst1.get(), payload, payload_size);
    construct_message(dst2.get(), payload, payload_size);

    if (::memcmp(dst1.get(), dst2.get(), payload_size) != 0)
        exit(1);


    if (payload_size != destruct_message(dst2.get(), 0)) {
        exit(2);

    }


    // TEST 2
    st_payload = random_alpharithmetic(1024);
    payload_size = st_payload.size();
    payload = const_cast<char*>(st_payload.c_str());
    dst1 = std::make_unique<char[]>(payload_size+4);
    dst2 = std::make_unique<char[]>(payload_size+4);
    solution::construct_message(dst1.get(), payload, payload_size);
    construct_message(dst2.get(), payload, payload_size);

    if (::memcmp(dst1.get(), dst2.get(), payload_size) != 0)
        exit(3);


    if (payload_size != destruct_message(dst2.get(), 0)) {
        exit(4);

    }

    // TEST 3
    st_payload = random_alpharithmetic(2096);
    payload_size = st_payload.size();
    payload = const_cast<char*>(st_payload.c_str());
    dst1 = std::make_unique<char[]>(payload_size+4);
    dst2 = std::make_unique<char[]>(payload_size+4);
    solution::construct_message(dst1.get(), payload, payload_size);
    construct_message(dst2.get(), payload, payload_size);

    if (::memcmp(dst1.get(), dst2.get(), payload_size) != 0)
        exit(5);


    if (payload_size != destruct_message(dst2.get(), 0)) {
        exit(6);

    }

    // TEST 4
    st_payload = random_alpharithmetic(8192);
    payload_size = st_payload.size();
    payload = const_cast<char*>(st_payload.c_str());
    dst1 = std::make_unique<char[]>(payload_size+4);
    dst2 = std::make_unique<char[]>(payload_size+4);
    solution::construct_message(dst1.get(), payload, payload_size);
    construct_message(dst2.get(), payload, payload_size);

    if (::memcmp(dst1.get(), dst2.get(), payload_size) != 0)
        exit(7);


    if (payload_size != destruct_message(dst2.get(), 0)) {
        exit(8);

    }

    // TEST 4
    st_payload = random_alpharithmetic(10000);
    payload_size = st_payload.size();
    payload = const_cast<char*>(st_payload.c_str());
    dst1 = std::make_unique<char[]>(payload_size+4);
    dst2 = std::make_unique<char[]>(payload_size+4);
    solution::construct_message(dst1.get(), payload, payload_size);
    construct_message(dst2.get(), payload, payload_size);

    if (::memcmp(dst1.get(), dst2.get(), payload_size) != 0)
        exit(9);


    if (payload_size != destruct_message(dst2.get(), 0)) {
        exit(10);

    }
}
