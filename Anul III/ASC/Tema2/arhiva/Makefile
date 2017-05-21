#
# @Gabriel Ivanica
#
CC=gcc
CFLAGS=-O3 -Wall -Wno-strict-aliasing -Wno-unused-result
LDFLAGS=-lcblas -latlas
LIB-OPTERON=/opt/tools/libraries/atlas/3.10.1-opteron-gcc-4.4.6/lib
LIB-NEHALEM=/opt/tools/libraries/atlas/3.10.1-nehalem-gcc-4.4.6/lib
LIB-QUAD=/opt/tools/libraries/atlas/3.10.1-quad-gcc-4.4.6/lib/

BIN=tema2
MATRIX_SIZE=5000

#
# Build Commands 
#

build: tema2.o
	$(CC) $^ -o $(BIN) $(CFLAGS) $(LDFLAGS)

opteron: tema2.o
	$(CC) $^ -o $@ -L$(LIB-OPTERON) $(LDFLAGS)

nehalem: tema2.o
	$(CC) $^ -o $@ -L$(LIB-NEHALEM) $(LDFLAGS)

quad: tema2.o
	$(CC) $^ -o $@ -L$(LIB-QUAD) $(LDFLAGS)

#
# Run Commands 
#

run:
	./$(BIN) 5000 home

run_opteron:
	./opteron $(MATRIX_SIZE) opteron

run_nehalem:
	./nehalem $(MATRIX_SIZE) nehalem

run_quad:
	./quad $(MATRIX_SIZE) quad

#
# Clean option
#

clean:
	rm -Rf ./out
	rm -Rf rez*.txt
	rm -Rf core.*
	rm -Rf $(BIN) opteron nehalem quad *.o
