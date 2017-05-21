# Ivanica Gabriel 313CA Tema4

# Declarațiile de variabile
CC = gcc
CCFLAGS = -Wall -lm
SRC = tema4.c
PROGRAM = tema4

# Regulă de compilare
all:
	$(CC) -o $(PROGRAM) $(SRC)

# Regulile de "curațenie" (se folosesc pentru ștergerea fișierelor intermediare si/sau rezultate)
.PHONY : clean
clean :
	rm -f $(PROGRAM) core *~ 
