#!/env/bin/bash

# Test the secure_recv() and secure_sent() functions
python ./tests/single_threaded_server.py &
sleep 1
python ./tests/single_threaded_client.py
sleep 1

echo "Finished tests .."
