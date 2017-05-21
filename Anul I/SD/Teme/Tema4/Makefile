# Ivanica Gabriel 313CA

# Declarațiile de variabile
CC = gcc
CCFLAGS = -Wall -O3 -Wno-unused-result
SRC = main.c heap.c header.h heap.h
PROGRAM = hufftar

# Compilare
all:
	$(CC) -o $(PROGRAM) $(SRC) $(CCFLAGS)
build:
	$(CC) -o $(PROGRAM) $(SRC) $(CCFLAGS)

.PHONY : clean
clean :
	rm -f $(PROGRAM) core *~
