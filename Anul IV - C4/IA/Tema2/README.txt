--------------------------------------------------------------------------------
Tema2 - Clasificare
Gabriel Ivanica 
Grupa 342C5
--------------------------------------------------------------------------------

Am implementat tema cu bonus.
Inamicii se grupeaza cate 2 cate 2 (cei care nu vad harta)

Starea jocului este reprezentata de 2 vectori de dimensiune N
N reprezinta numarul de sectiuni in care se imparte zona prazii

In fiecare vector voi retine informatii referitoare la pradatori sau capcane 
aplicand o anumita functie.

Am incercat atat cu valori discrete dar si continue
	- suma paradtorilor/capcanelor din cadran
	- suma distantelor pradatorilor/capcanelor
	- suma distantelor pe X si Y
	
Actiunile sunt in numar de N+1, N directii si posibilitatea de a sta pe loc

--------------------------------------------------------------------------------
Detalii
--------------------------------------------------------------------------------

Initial salvez toate starile jocului pentru un numar K de simulari.
Minimizez numarul de stari prin aplicarea algoritmului K-means
Distanta intre 2 stari este distanta euclidiana (nu cred ca e prea buna ...)

Aplic Q-learning pentru invatare

Pentrua  obtine o stare stabila aproximez la cel mai apropiat centroid din K-means

--------------------------------------------------------------------------------
Rezultate
--------------------------------------------------------------------------------

Am rulat pe 4 sau 3 inamici si 2 sau 3 capcane:

600			// dimensiune habitat x
400			// dimensiune habitat y
3.6			// viteza prada
3			// raza coliziune prada
100			// raza perceptie prada
4			// numar pradatori
4.1			// viteza pradatori
2			// raza coliziune pradatori
200			// raza perceptie pradatori
2			// numar capcane
5			// raza coliziune capcane

In functie de parametrii am obtinut imbunatatiri pana la 20% procentaj de jocuri
castigate.

Rezultatele relevante sunt doar pentru ALHA mare (0.7 - 0.8) si GAMMA mic (0.3 - 0.2)
pentru alte valori invatarea nu pare sa functioneze prea bine avand rezultate 
mai mici decat random

Exemplu rezultate:
	K-means, Q-learning
		- random - 20000000 stari ~ 10% WINS
		- reprezentare discreta => 2048 clustere
			- porneste cu 2% si ajunge la 14% dupa 20000 de jocuri
			- 5-6% spre 14%

		- random - 100000 stari ~ 10% WINS
		- stari continue => 1024 clustere
			- incepe cu 10%  ajunge spre 21-22% dupa 5000 de jocuri

PS. Am adaugat si fisiere cu rulari			
