#!/env/bin/bash

# Test construct_message() and get_payload_size() functions
python ./test_messages.py


# Test the secure_recv() and secure_sent() functions
python ./single_threaded_server.py &
sleep 1
python ./single_threaded_client.py
sleep 1

# Test single-threaded server and client
python ./test_server.py &
sleep 1
python ./test_client.py
sleep 10


# Test multithreaded-server and clients
python ./multithreaded_server_2.py &
sleep 1
python ./multithreaded_client_6.py &
sleep 12



# Test multithreaded-server and clients
python ./multithreaded_server_6.py &
sleep 1
python ./multithreaded_client_6.py &
sleep 12


# Test multithreaded-server and clients
python ./multithreaded_server_2.py &
sleep 1
python ./multithreaded_client_24.py
sleep 12


# Test repeated requests for connections
python ./multithreaded_server_longrunning.py &
sleep 1
python ./run_many_client_procs.py

sleep 30

echo "Finished tests .."
