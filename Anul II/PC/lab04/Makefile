all: clean reader writer
	mkfifo data
	mkfifo ack

clean:
	rm -f A
	rm -f B
	rm -f data
	rm -f ack

reader: 
	gcc -o B B.c

writer:
	gcc -o A A.c alarm.c

run:
	./A
