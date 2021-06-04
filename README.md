# Task 6 - Sockets

Your task is to implement a client/server application over TCP/IP using sockets API (e.g. STREAM_SOCK).
The server process spawns a fixed number of threads which is taken as an argument. 
Each server thread should be able to handle and process more than one connections. Further, the server should be able to continuously accept new client connections. The listening address
should be also passed as an argument. For testing your application locally you should execute ``./server <nb_server_threads> localhost 1025``.

The requests can be of type: ADD, SUB, and TERMINATION and you will be using google::protobufs to serialize and pass the message over the network. 
The server process keeps a global counter and atomically updates it based on the received requests.
Along with ADD/SUB requests, the request also contains a number that will
be added/sub from the global counter respectively.
Lastly, the TERMINATION request, instructs the server to sent back to the client a message that contains that the value of the global counter.
We use the value of the global counter to verify that the applications are executed correctly. In particular, for each termination request the server prints the value of the global counter, it send over
the network. Similarly, each client thread, prints the value the server sent to it. 
As a serialization protocol, you will be using google::protobufs. Note that the .proto files describe the message layout and are already provided for you. You can produce the c++ files using
``protoc --cpp_out=. <filename>.proto``.


## Task 6.1
Implement the ``construct_message`` and ``get_payload_size`` functions at ``util.h``.

TCP communication does not distinguish between different messages; ``recv`` system call might return a stream of bytes that refer to more than one distinct applications messages.
To process the individual messages, applications should implement their own message's formatting or serialization protocol. 

A common approach is to encapsulate the payload size as follows: <size> <payload>, where payload is a serialized protobuf (byte-stream).

For the functions implementation please see comments in ``util.h``.



## Task 6.2

Implement the ``secure_send`` and ``secure_recv`` functions. 

The ``secure_send`` is a wrapper on top of ``send`` syscall that ensures that all requested bytes are written to the socket.
The ``secure_recv`` is a wrapper on top of ``recv`` syscall that returns a single whole message.

Note that ``send`` and ``recv`` syscalls, similarly to ``write``/``read``,  do not quarantee that the entire packet will be written/read. You should repeatedly call them, to ensure that all data are written/read to the socket.
Hint: You should implement those functions assuming non-blocking sockets.


## Task 6.3
### 6.3.1
Implement the ``get_operation`` and ``process_request`` and ``send_termination_message`` functions.
The ``get_operation`` function generates requests for the client application. For your own testing you could generate the requests as you would like. 
However, for passing the tests, the ``get_operation`` should have a specific requests distribution. In ``util_msg.cc``, we provide a skeleton and instructions on how to implement it.


The ``process_request`` is invoked from the server process and should deserialize and execute the received request(s). In case the server receives a termination message, it should also sent as a reply the current value of the global counter value.
Lastly, the ``send_termination_message`` is executed by the client process. It constructs, serializes and sents to the server the termination message. Further it waits for the servers reply. For testing purposes, you could extend the messages layout (.proto files) but for the grading system, this is not required.


### 6.3.2
Implement the ``client`` function at ``client.cc``. 

The client process spawns a number of threads each of which
connects to the server process. The client process takes as an argument the address (hostname and port) of the listening server address. 
After establishing the connection with the server, each thread should sent a variable number of messages (taken as an argument).
Once all messages are received, each client-thread should sent a termination message to the server. Right after the delivery of the termination message
a thread should receive from the server the value of the global counter and should print the received value. Once all the client threads have joined, the client process terminates.


### 6.3.3
Implement the `server` function at `server.cc`.

The server process initially spawns a fixed number of threads (e.g. 2, 4, 8). Then it should accept connections on a port passed an argument. We use ``localhost`` as the specified IP.
The server should be available to 
accept connections at any time and should not terminate. Once a connection is accepted, the server process should assing this connection to one of the server threads. You could find the 
elected thread's id by dividing the number of connections with the number of the server threads (``nb_connections % nb_threads``). 
If a server thread receives a termination message, it should reply back to the client with the number of
received bytes in this connection.


## Important notes
``printf()``-based functions are not thread-safe and in the presence of many clients you might print the correct counter values in the same line. In that case, the tests in the grading system migth fail.
We encourage you to wrap the print-functions with a global mutex to ensure that a message is flushed to standard output in order.
