all: clean build run

build:
	mkfifo data
	mkfifo ack
	gcc -o read reader.c crc.c
	gcc -o write writer.c crc.c

clean :
	rm -f read
	rm -f write
	rm -f data
	rm -f ack
run:
	./read

