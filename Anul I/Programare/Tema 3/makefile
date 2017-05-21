# Ivanica Gabriel 313CA Tema3

# Declarațiile de variabile
CC = gcc
CCFLAGS = -Wall -lm
SRC = tema3.c
PROGRAM = tema3

# Regulă de compilare
all:
	$(CC) -o $(PROGRAM) $(SRC)

# Regulile de "curațenie" (se folosesc pentru ștergerea fișierelor intermediare si/sau rezultate)
.PHONY : clean
clean :
	rm -f $(PROGRAM) core *~ 
