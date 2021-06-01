.PHONY : all check client server clean
all : check client server

check: clean client server format_test run

LINKING_FLAGS = -lpthread  -fsanitize=address
SRC_FILES =

format_test:
	g++ -O3 -g -std=c++17 message_formatting_tests.cc -o message_formatting_test ${LINKING_FLAGS}
	mv message_formatting_test ../

client: 
	g++ -O3 -g -std=c++17 client.cc ${SRC_FILES} -o client ${LINKING_FLAGS}
	mv client ../

server:
	g++ -O3 -g -std=c++17 server.cc ${SRC_FILES} -o server ${LINKING_FLAGS}
	mv server ../

clean:
	rm -f ../server ../client ../message_formatting_test


run:
	sh ./run.sh
