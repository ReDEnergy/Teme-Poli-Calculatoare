CC = g++
CCFLAGS = -g -O2 -Wall

all: clean server client

server:
	$(CC) $(CCFLAGS) $(LIBSOCKET) server.cpp -o server

client:
	$(CC) $(CCFLAGS) $(LIBSOCKET) client.cpp -o client

runs:
	./server 7777

runc:
	./client $(NAME) 127.0.0.1 7777

clean:
	rm -f *.o *~
	rm -f server client



