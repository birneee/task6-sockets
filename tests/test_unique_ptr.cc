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
#include "../util.h"



std::string getstring(size_t size) {
    std::string st;
    for (int i = 0; i < size; i++) {
        st += "d";
    }
    return st;
}

std::unique_ptr<char[]> get_rand_data(size_t& size) {
    static int i = 1;
    i*= 2;
    if ( i > 65536 )
        i = 1;

    size = i;
    auto st = getstring(i);
    char* data = const_cast<char*>(st.c_str());

    std::unique_ptr<char[]> ptr = std::make_unique<char[]>(size);
    ::memcpy(ptr.get(), data, size);

    return std::move(ptr);
}



int main (int args, char* argv[]) {
    size_t size =0;
    std::unique_ptr<char[]>buf = get_rand_data(size);

}
