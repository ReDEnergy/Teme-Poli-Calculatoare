all: clean reader writer
	mkfifo data
	mkfifo ack

clean:
	rm -f reader
	rm -f writer
	rm -f data
	rm -f ack

reader: reader.c
	gcc -o reader reader.c

writer: writer.c
	gcc -o writer writer.c

run:
	./reader
