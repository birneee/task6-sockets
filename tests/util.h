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


/**
 * It takes as arguments one char[] array of 4 or bigger size and an integer.
 * It converts the integer into a byte array.
 */
void convertIntToByteArray(char* dst, int sz) {
    auto tmp = dst;
    tmp[0] = (sz >> 24) & 0xFF;
    tmp[1] = (sz >> 16) & 0xFF;
    tmp[2] = (sz >> 8) & 0xFF;
    tmp[3] = sz & 0xFF;
}

/**
 * It takes as an argument a ptr to an array of size 4 or bigger and 
 * converts the char array into an integer.
 */
int convertByteArrayToInt(char* b) {
    return (b[0] << 24)
        + ((b[1] & 0xFF) << 16)
        + ((b[2] & 0xFF) << 8)
        + (b[3] & 0xFF);
}


/**
 * It constructs the message to be sent. 
 * It takes as arguments a destination char ptr, the payload (data to be sent)
 * and the payload size.
 * It returns the expected message format at dst ptr;
 *
 *  |<---msg size (4 bytes)--->|<---payload (msg size bytes)--->|
 *
 *
 */
void construct_message(char* dst, char* payload, size_t payload_size) {
    convertIntToByteArray(dst, payload_size);
    ::memcpy(dst+4, payload, payload_size);
}

/**
 * It returns the actual size of msg.
 * Not that msg might not contain all payload data. 
 * The function expects at least that the msg contains the first 4 bytes that
 * indicate the actual size of the payload.
 */
int destruct_message(char* msg, size_t bytes) {

    auto actual_msg_size = convertByteArrayToInt(msg);

    return actual_msg_size;
}


/**
 * Sends to the connection defined by the fd, a message with a payload (data) of size len bytes.
 */
int secure_send(int fd, char* data, size_t len) {
    int64_t bytes = 0;
    int64_t remaining_bytes = len + 4;
    
    std::unique_ptr<char[]> buf = std::make_unique<char[]>(len + 4);
    construct_message(buf.get(), data, len);
    char* tmp = buf.get();

    while (remaining_bytes > 0) {
        bytes = send(fd, tmp, remaining_bytes, 0);
        if (bytes < 0) {
            // @dimitra: the socket is in non-blocking mode; select() should be also applied 
            return -1;
        }
        remaining_bytes -= bytes;
        tmp += bytes;
    }

    return len;

}

/**
 * 
 */
int secure_recv(int fd, std::unique_ptr<char[]>& buf, size_t len) {
    int64_t bytes = 0;
    char* tmp;
    int64_t remaining_bytes = 4;

    // recv the message size (the first 4 bytes)
    char dlen[4];
    tmp = dlen;
    while (remaining_bytes > 0) {
        bytes = recv(fd, tmp, remaining_bytes, 0);
        if (bytes < 0) {
            // @dimitra: Note that the socket is non-blocking so it is fine to return -1 (EWOULDBLOCK/EAGAIN).
        }
        else if (bytes == 0) {
            // @dimitra: Connection reset by peer
            return 0;
        }
        else {
            remaining_bytes -= bytes;
            tmp += bytes;
        }
    }


    int64_t actual_msg_size = destruct_message(dlen, 4);
    remaining_bytes = actual_msg_size;
    buf = std::make_unique<char[]>(actual_msg_size+1);
    buf[actual_msg_size] = '\0';
    tmp = buf.get();
    
    while (remaining_bytes > 0) {
        bytes = recv(fd, tmp, remaining_bytes, 0);
        if (bytes < 0) {
            // @dimitra: Note that the socket is non-blocking so it is fine to return -1 (EWOULDBLOCK/EAGAIN).
        }
        else if (bytes == 0) {
            return 0;
        }
        else {
            remaining_bytes -= bytes;
            tmp += bytes;
        }
    }


    return actual_msg_size;
}


