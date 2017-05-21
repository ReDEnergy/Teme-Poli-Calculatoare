CFLAGS=-m32 -g

all: send recv
	
send: send.o lib.o crc.o functions.o
	gcc $(CFLAGS) send.o lib.o crc.o functions.o -o send

recv: recv.o lib.o crc.o functions.o
	gcc $(CFLAGS) recv.o lib.o crc.o functions.o -o recv

.c.o:
	gcc $(CFLAGS) -Wall -c $?

clean:
	-rm send.o recv.o functions.o crc.o send recv
