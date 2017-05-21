CFLAGS = -Wall
EXE = Tema4
TOP = topologie.txt
MSG = mesaje.txt
N = 12

build: 
	mpic++ $(CFLAGS) Tema4.cpp -o $(EXE)

run:
	bash -c "time mpirun -n $(N) $(EXE) $(TOP) $(MSG)"

debug:
	bash -c "time mpirun -n $(N) $(EXE) $(TOP) $(MSG) 1"

all: build run


.PHONY: clean

clean:
	rm -f *.o $(EXE)

