CXX=g++
CFLAGS=-I/opt/local/include -g
LDFLAGS=-L/opt/local/lib -lgtest

server: server.cpp
	${CXX} ${CFLAGS} -o server server.cpp


client: client.cpp
	${CXX} ${CFLAGS} -o client client.cpp


all:	server client test

test:   test.cpp
	${CXX} ${CFLAGS} ${LDFLAGS} -o test test.cpp