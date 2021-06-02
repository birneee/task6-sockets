#!/env/bin/bash

python ./test_something.py

python ./test_server_1.py & python ./test_client_1.py
python ./test_server.py & python ./test_client.py
