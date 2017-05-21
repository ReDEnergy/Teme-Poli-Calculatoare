********** Tema 4 APD
********** Ivanica Gabriel
********** 333CA


********************************************************************************
******************************* Implementare ***********************************


	IMPLEMENTARE

		mesajele sunt trimise folosind structuri in care retin informatii necesare

		topologie : vectori si int

		trimitere mesaje : structura Message
			
				int sender;
				int destination;		
				char msg[256];
			

		votarea liderului si a asistentului

				int sender;
				int destination;		
				int leader;
				int assistant;
				int result;
				char msg[256];

		rezultat : tipul mesaje / rezultat
			ELECTION	- trebuie ales atat leaderul cat si asistentul
			RESULT		- rezultat OK
			LEADER		- trebuie reales leaderul		
			ASSISTANT	- trebuie reales assistentul


********************************************************************************
	STABILIRE TOPOLOGIE

	Pentru stabilirea Topologiei am pornit din nodul 0 si am folosit algortimul
	de stabilire al topologiei bazat pe SONDA / ECOU / ECOU VID

	Pe parcursul acestui algoritm se creeaza un arbore minim de acoperire.
	Cum totii fii asteapta la randul lor un subarbore minim de acoperire am ales
	sa construiesc topologia astfel.

	Cand se primeste o SONDA
		se seteaza PARINTE

	Daca se mai primeste SONDA
		se trimite ECOU VID

	Cand un nod primeste ECOU
		NEXT HOP pentru topologia primita ca ECOU devine nodul de la care a primit ECOUL
		se seteaza toate celelelalte noduri cu NEXT HOP pe PARINTE

	Cand se primeste ECOU VID
		nu se face nimic deoarece vreau sa generez un armore minim de acoperire


	TOPOLOGIA astfel creata se baseaza pe arorele minim de acoperire al nodului de
	la care porneste algoritmul
	
	Algoritmul se termina dupa un numar de 2M mesaje unde M e numarul de muchii


	SINCRONIZARE

	Cum in acest moment toate procesele treubuie sincronizate pentru a nu se trece la
	etapa de transmitere de MESAJE voi trimite un BROADCAST de la NODUL 0 iar toate
	celelalte NODURI vor astepta acest mesaj
	

********************************************************************************
	TRANSMITERE MESAJE

	Mesajele sunt transmise folosind vectorul NEXT HOP creat la punctul precedent

	Pentru a putea ruta toate mesajele am creat o bucla "infinita" care se va termina
	doar atunci cand 
		- fiecare proces a notificat toate celelalte procese ca nu mai are
	de transmis nici un mesaj
		- toate mesajele transimse de nodul curent au fost confirmate ca si primite

	Folosesc astfel un vector de conexiuni CONNECTIONS, NR_CONNECTIONS , UNCONFIRMED_MSG
	pentru a determina cand sa termin programul

	Fiecare nod isi trimite toate mesajele sale (citite din fisier) iar apoi trimite
	un mesaj de DONE_SENDING prin care notifica ca nu mai are de transmis mesaje

	Fiecare nod ramane in bucla permitand rutarea mesajelor pana cand e noticat de
	toate nodurile ca nu mai este necesar.


********************************************************************************
	ALEGEREA LEADERULUI

	Pentru alegerea leaderului nodul 0 trimite tuturor un mesaj de tipul
		ELECTION iar toate nodurle trimit alegerea lor pentru cele 2 pozitii

	In cazul in care nu se aleg din prima nodurile castigatoare, nodul 0 retrimite
	mesajul de interogare si asteapta iar voturile pana cand vodul este corect.

	Rezultatele voturilor sunt trimise la fiecare NOD (bradcast) iar in caz OK
	toate nodule afiseaza raspunsul iar programul se termina

		

********************************* OUTPUT ***************************************

Un mesaj de vot este afizat in acest mode:

	sender: 2	from: 2		dest: 0		msg: 'Vote'
	Leader: 7	Assistant: 10	Result: 0
	 => received 

	Leader: 	{1 1 0 2 1 0 0 4 0 2 0 1 }	- voturile pentur leader de catre noduri
	Assistant: 	{0 1 0 2 0 3 0 1 0 0 1 0 }	- voturile pentur asistent de catre noduri	


Pentru orice mesaj :
	=> received  - mesajul era destinat nodului curent si a fost receptionat
	=> forward	 - mesajul e destinat altui nod si este rutat prin nodul curent



********************************* Rulare ***************************************


	In fisierul TOPOLOGIE si MESAJE am adaugat alte 2 teste pentru rulare
	Pentru a rula aceste teste trebuie copiate informatiile la inceputul fisierului




	make complileaza
	
	make run	=> ruleaza cu fisierele default pe 12 procese
	bash -c "time mpirun -n 12 Tema4 topologie.txt mesaje.txt"

	make debug	=> la fel cu "run" se afiseaza DEBUG mesaje intermediare
					in fisierele de output

	bash -c "time mpirun -n 12 Tema4 topologie.txt mesaje.txt 1"
		
	
	parametrul DEBUG
		0	doar mesaje importante si esentiale
		1	afisarea toturor mesajelor intermediare dintre noduri
			si alte informatii despre conexiune, etc


	QSUB
	qsub -q ibm-quad.q run.sh ~/APD/Tema4/Tema4 FILE_TOP FILE_MSG NR_PROCESSES DEBUG
	
	EX
	qsub -q ibm-quad.q run.sh ~/APD/Tema4/Tema4 ~/APD/Tema4/topologie.txt ~/APD/Tema4/mesaje.txt 12
	


********************************* Fisiere **************************************

	1) Tema4.cpp
	2) Makefile
	3) Readme
	4) run.sh	- rulat pe cluster

