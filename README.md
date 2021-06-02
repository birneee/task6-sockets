# Task 6 - Sockets

Your task is to implement a client/server application over TCP/IP using sockets API (e.g. STREAM_SOCK).
The server process spawns a fixed number of threads (taken as an argument) each of which processes clients' requests.
Each server thread should be able to handle more than one connections concurrently.

The requests can be of type: ADD, SUB, TERMINATION and RANDOM_DATA. The server counts the number of received bytes.
Along with ADD/SUB requests, the request also contains a number that will
be added/sub from a global counter respectively. The RANDOM_DATA request has no impact to the global counter; the server only updates the total number of received bytes.
Lastly, the TERMINATION request, instructs the server to sent back to the client a message that contains that total bytes received in that connection and the value of the global counter.
As a serialization protocol, you will be using Google::protobufs. Note that the .proto files describe the message layout and are already provided. You can produce the c++ files using
``protoc --cpp_out=. <filename>.proto``


## Task 6.1
Implement the ``construct_message`` and ``destruct_message`` functions at ``util.h``.``

TCP communication does not distinguish between different messages; ``recv`` system call might return a stream of bytes that refer to more than one distinct applications messages.
To process the individual messages, applications should implement their own message's formatting or serialization protocol. 

A common approach is to encapsulate the payload size as follows: <size> <payload>

For the functions design please see comments in ``util.h``.



## Task 6.2

Implement the ``secure_send`` and ``secure_recv`` functions. 

The ``secure_send`` is a wrapper on top of ``send`` syscall that ensures that all requested bytes are written to the socket.
The ``secure_recv`` is a wrapper on top of ``recv`` syscall that returns a single whole message.

Hint: You should design those functions for non-blocking sockets.


## Task 6.3

Implement the ``get_operation`` and ``process_request`` and ``send_termination_message`` functions. 


## Task 6.4

Implement the ``client`` function at ``client.cc``. 

The client process spawns a number of threads each of which
connects to the server process. After establishing the connection, each thread should sent a variable number of messages (taken as an argument).
Once all messages are received, each client-thread should sent a termination message to the server. Right after the delivery of the termination message
a thread should receive from the server the number of all bytes received on the remote side. Right after that the client should terminate.


## Task 6.5

Implement the `server` function at `server.cc`.

The server process initially spawns a fixed number of threads (e.g. 2, 4, 8). Then it should accept connections on ``PORT 1025``. The server should be available to 
accept connections at any time. Once a connection is accepted, the main server thread should assing this connection to one of the server threads. You could find the 
elected thread's id by dividing the number of connections with the number of the server threads (``nb_connections % nb_threads``). 
Each server thread counts for each connection the received bytes. If a server thread receives a termination message, it should reply back to the client with the number of
received bytes in this connection.
