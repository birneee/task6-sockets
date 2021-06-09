#!/env/bin/bash

# Test construct_message() and get_payload_size() functions
python ./test_messages.py


# Test the secure_recv() and secure_sent() functions
python ./test_2.py

# Test single-threaded server and client
python ./test_3.py


# Test multithreaded-server and clients
python ./test_4.py



# Test multithreaded-server and clients
python ./test_5.py


# Test multithreaded-server and clients
python ./test_6.py


# Test repeated requests for connections
python ./test_7.py

echo "Finished tests .."
