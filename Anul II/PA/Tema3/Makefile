build:
	g++ -g -w -O2 Tema3.cpp -o Tema3

all: test run diff 

run: 
	./Tema3

test:
	cp ./teste/$(TEST)/* ./

diff:
	diff -w segment.pgm example.pgm

energy:
	cat expected.txt

clean:
	rm -f Tema3
	rm -f *.o *~
	rm -f *.pgm
	rm -f *.txt

