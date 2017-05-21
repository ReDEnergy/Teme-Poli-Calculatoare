CFLAGS = -O3 -Wall

all: serial, paralel

serial: serial.cpp main.h
	g++ $(CFLAGS) serial.cpp -o serial

paralel: paralel.cpp main.h paralel.h
	g++ $(CFLAGS) -fopenmp paralel.cpp -o paralel

optims: optims.cpp main.h
	g++ $(CFLAGS) optims.cpp -o optims

optimp: optimp.cpp main.h paralel.h
	g++ $(CFLAGS) -fopenmp optimp.cpp -o optimp

clean:
	rm -f *.o serial paralel optims optimp
