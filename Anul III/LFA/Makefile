CC=g++
LEX=flex
LDFLAGS=-lfl
CFLAGS=-c -g -O3 -Wall
OFLAGS=-o $@
RM=rm

mtx : mtx.o Turing.o Parser.o ElemMachine.o StateMachine.o Definition.o
	$(CC) -o $@ $^ $(LDFLAGS)

mtx.o : mtx.cpp
	$(CC) $(CFLAGS) -o $@ $<

mtx.cpp : mtx.l
	$(LEX) -o $@ $<


Definition.o : Definition.h Definition.cpp
	$(CC) $(CFLAGS) Definition.cpp

Turing.o : Turing.h Turing.cpp
	$(CC) $(CFLAGS) Turing.cpp

Parser.o : Parser.h Parser.cpp
	$(CC) $(CFLAGS) Parser.cpp

ElemMachine.o : ElemMachine.h ElemMachine.cpp
	$(CC) $(CFLAGS) ElemMachine.cpp

StateMachine.o : StateMachine.h StateMachine.cpp
	$(CC) $(CFLAGS) StateMachine.cpp

.PHONY: clean

clean: 
	rm *.o mtx.cpp mtx

run:	
	bash -c "time ./mtx ./test/advanced.mt Product_10 '#$>$#$#7#8#$#'"


