Pentru utilizatorii din linia de comanda:

Makefile-ul construieste in mod implicit:
	A) fisierele din biblioteca (care se gasesc in directorul src/)
	B) solutia specificata in mod unic de variabilele de mediu LAB si TASK

================================================================================

Puteti specifica compilarea unei surse anume in mai multe moduri. Sa zicem ca 
vreti sa rezolvati problema 1 din laboratorul 1:

1) Setati variabilele de mediu corespunzator astfel (in Linux)

	export LAB=1
	export TASK=1
	make

# Toate apelurile catre make de acum incolo vor compila problema 1 din lab 1.
# Pentru a schimba problema sau laboratorul, re-exportati variabila cu alta
# valoare.
				
2) Definiti variabilele in momentul interpretarii fisierului Makefile

	make LAB=1 TASK=1

# Va trebui sa definiti aceste doua variabile de fiecare data cand rulati make

================================================================================

Sursele pentru un laborator anume (de la care trebuie sa porniti) se gasesc in
directoare de forma src-labX, si au nume de forma labXpY.cpp

Atentie! Toate numele trebuie sa respecte aceste conventii daca vrei sa folosi
Makefile.

================================================================================

Documentatia pentru sursele de biblioteca pe care vi le punem la dispozitie
inafara de STL (cele care se regasesc in src/) se gaseste in
       *	doc/html/index.html (deschideti in Browser)
       * 	doc/latex (trebuie sa o compilati in latex - are Makefile)

================================================================================

Executabilele generate sunt de forma labXpY

