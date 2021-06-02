CXX = g++
COMPILE_FLAGS = -O3 -std=c++17 -Wall -g

.PHONY : all check clean
all : check

check: clean client_test server_test message_formatting_tests client server run

LINKING_FLAGS = -lpthread  #-fsanitize=address
SRC_FILES =
PROTO_FILES = client_message.pb.cc server_message.pb.cc
EXTRA_LFLAGS = $(LINKING_FLAGS) -lprotobuf lib_msg2.a

message_formatting_tests:
	$(CXX) $(COMPILE_FLAGS) $@.cc -o $@ ${LINKING_FLAGS}
	mv message_formatting_tests ../

client_test: 
	$(CXX) $(COMPILE_FLAGS) $@.cc ${SRC_FILES} -o $@ ${LINKING_FLAGS}
	mv client_test ../

server_test:
	$(CXX) $(COMPILE_FLAGS) $@.cc ${SRC_FILES} -o $@ ${LINKING_FLAGS}
	mv server_test ../

server:
	$(CXX) $(COMPILE_FLAGS) $@.cc ${PROTO_FILES} -o $@ ${EXTRA_LFLAGS}
	mv server ../

client:
	$(CXX) $(COMPILE_FLAGS) $@.cc ${PROTO_FILES} -o $@ ${EXTRA_LFLAGS}
	mv client ../


clean:
	rm -f ../server ../client ../message_formatting_tests ../server_test ../client_test server.txt client.txt output_.txt


run:
	sh ./run.sh
