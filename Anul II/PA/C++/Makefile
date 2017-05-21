#
#	Please, send _any_ bug reports or questions to: adrian.sc@rosedu.org
#
# Standard variables to automatize the build process
CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lm

# Adding include directories
INCLUDE=-Iinclude

LIB_SOURCES=$(wildcard src/*.cpp)
LIB_OBJECTS=$(addprefix bin/,$(notdir $(LIB_SOURCES:.cpp=.o)))

# You can edit the following lines if you want, or you can specify a source on call
#LAB=1
#TASK=1
ifdef LAB
ifdef TASK
SRC=lab$(LAB)p$(TASK)
endif
endif

# These two targets should always execute
.PHONY: all clean

# This rule compiles the file named $(SRC) (passed as an argument) to an executable
# of the same name and places it in /bin 
ifdef LAB
ifdef TASK
all: $(LIB_OBJECTS) src-lab$(LAB)/$(SRC).cpp
	$(CC) $(INCLUDE) $(LDFLAGS) -o $(SRC) $(LIB_OBJECTS) src-lab$(LAB)/$(SRC).cpp
else
all: $(LIB_OBJECTS)
	@echo " "
	@echo "To compile a source file, specify a LAB and a TASK number"
	@echo "For example, typing:  "
	@echo "    make LAB=1 TASK=1 "
	@echo "will compile a file called lab1p1.cpp to an executable called \"lab1p1\"."
	@echo " "
endif
else
all: $(LIB_OBJECTS)
	@echo " "
	@echo "To compile a source file, specify a LAB and a TASK number"
	@echo "For example, typing:  "
	@echo "    make LAB=1 TASK=1 "
	@echo "will compile a file called lab1p1.cpp to an executable called \"lab1p1\"."
	@echo " "
endif

# This rule compiles all the files in src/ to corresponding objects in bin/
bin/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c -o bin/$*.o src/$*.cpp

# This rule will clean all files in the current directory    
clean:
	rm -rf bin/*.o
	rm -f  lab*
	clear

