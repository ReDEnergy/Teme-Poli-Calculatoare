# Ivanica Gabriel 313CA Tema1 

# Declarațiile de variabile
CC = gcc
CCFLAGS = -Wall -lm
SRC = tema1.c
PROGRAM = tema1

# Regulă de compilare
all:
	$(CC) -o $(PROGRAM) $(SRC)

# Regulile de "curațenie" (se folosesc pentru ștergerea fișierelor intermediare si/sau rezultate)
.PHONY : clean
clean :
	rm -f $(PROGRAM) core *~ 
