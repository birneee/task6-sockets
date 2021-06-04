#!/env/bin/bash


# Test multithreaded-server and clients
python ./tests/multithreaded_server_2.py &
exit_server=$?
sleep 1
python ./tests/multithreaded_client_6.py
exit_client=$?
sleep 12

if [ $exit_server -eq 0 ]; then
    echo "command was successful"
else
    if [ $exit_client -eq 0 ]; then
        echo "command was successful"
    else
        exit 127
    fi
fi

