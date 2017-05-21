all: ruter sim

sim: multiproc.c helpers.h
	gcc -g -Wall -o sim multiproc.c


ruter: rut.cpp helpers.h
	g++ -g -Wall -o ruter rut.cpp


run: sim ruter events.in
	./sim events.in


clean:
	rm -f ruter sim log_rutare



