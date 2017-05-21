CFLAGS = -Wall
EXE = Tema3
N = 4

build: Complex.h Complex.cpp
	mpic++ $(CFLAGS) Tema3.cpp Complex.cpp -o $(EXE)

mandel:
	bash -c "time mpirun -n $(N) $(EXE) ./test/mandelbrot$(T).in out.pgm"

julia:
	bash -c "time mpirun -n $(N) $(EXE) ./test/julia$(T).in out.pgm"

juliatest:
	./imgdiff32 ./test/julia$(T).pgm out.pgm

mandeltest:
	./imgdiff32 ./test/mandelbrot$(T).pgm out.pgm
	
run:
	bash -c "time mpirun -n $(N) $(EXE) $(IN) $(OUT)"

all: build run

.PHONY: clean

clean:
	rm -f *.o $(EXE)
