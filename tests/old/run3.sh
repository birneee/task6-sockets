#!/env/bin/bash

# Test single-threaded server and client
python ./tests/test_server.py &
exit_server=$?
sleep 1
python ./tests/test_client.py
exit_client=$?
sleep 10

if [ $exit_server -eq 0 ]; then
    echo "$cmd command was successful"
else
    exit 2
fi
echo "Finished tests .."
