CC = gcc
CFLAGS = -Wall
LDFLAGS = 

heapsort: heapsort.o heap.o

heapsort.o: heapsort.c heap.h

heap: heap.c heap.h

clean:
	rm -f *.o heapsort

.PHONY: clean
