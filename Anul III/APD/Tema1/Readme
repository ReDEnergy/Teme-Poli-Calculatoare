********** Tema 1 APD
********** Ivanica Gabriel
********** 333CA


********************************************************************************
******************************* Implementare ***********************************

==== Variablile si structuri

	Am incarcat toate functiile si varibilele comuni tuturor surselor din
	headerul main.h.

	Astfel pentru toate sursele sunt utilizate urmatoarele variabile:

				 n	-	dimensiunea unei matrici
		time_frame	-	numarul de ani pe care se calculeaza
		Pmin, Pmax	-	pretul minim si maxim
			   INF	-	Numar foarte mare , echivalent INFINIT

	Matrici necesare pentru implementare

	 type	-	tipul resursei produse
	 price	-	pretul resuresei produse
	 budget	-	bugetul anual pentru a cumpara resursa complementara
	 buy_min-	pretul minim platit pentru resursa complementara
	 own_min-	pretul minim platit pentru resursa produsa



	Alte functii importante:

		Am implementat functiile getMin() si setMin() si setMax() deoarece sunt
		mult mai rapide decat alternativele din std.
			getMin() - intoarce minimul dintre 2 parametri
			setMin() - seteaza minimul in primul parametru
			setMax() - seteaza maximul in primul parametru


	==== Pentru SERIAL ====

	Afisarea informatiilor anuale se face prin utilizarea printYearResult()
	Bugetul pentru urmatorul an este recalculat apeland newBudget()

	==== Pentru PARALEL ====

	Se incarca headerul paralel.h ce contine

	Afisarea informatiilor anuale se face prin utilizarea printParalelYearResult()
	Bugetul pentru urmatorul an este recalculat apeland newParalelBudget()


********************************* Algoritm *************************************

	1) Toate

	Singura diferenta intre surse/algoritmi este reprezentata de algoritmul de
	baza de mai jos.

	Algoritmul de baza => pentru fiecare se an calculeaza:
		Matricile cu minimele pentru resursa 0 respectiv resursa 1
		Se recalculeaza bugetele pentru fiecare colonist
		Se afiseaza informatiile anuale.

	Se afiseaza matricea tuplu cu informatiile la final

	for (i=0; i<time_frame; i++) {
		getMinPrice();
		newBudget();
		printParalelYearResult();
	}

	Toate sursele folosesc alte aceleasi functii putin modificate sau alte functii.

	1) SERIAL Neoptimizata

		Pentru fiecare colonist N^2 verific toti vecinii acestuia N^2 si calculez
		pretul minim pentru a cumpara resursa colonistului vechi.

		min_resursa = minimul(min_curect, pret_resursa[i][j] + distanta_Manhattan)

		Implementare basic 4 for-uri.

		COMPLEXITATE: O(T * n^4)	, T = numarul de ani

		Timpi rulsare QSUB:

		===================================================
			FISIER			TIME_FRAME				TIMP
		===================================================
		in100_1.txt				10					 2.900s
		in100_1.txt				50					14.427s
		in100_1.txt				100					28.813s
		in50_1.txt				10					 0.201s
		in50_1.txt				50					 0.968s
		in50_1.txt				500					 9.457s

		Se poate observa liniaritatea complexitatii


	2)	PRALEL Neoptimizata

		Am paralelizat cele 3 functii utilizate in algoritmul de baza utilizand
		directivele #pragma omp

		Functia ce calculeaza matricele minime. getMinPrice()

			#pragma omp parallel for private(i, j)
			for (i=0; i<n; i++)
				for (j=0; j<n; j++)
					getMinPriceForNode(i, j);

		Astfel fiecare THREAD va calcula minimele pentru toti colonistii aflati
		pe aceeasi linie J matricii de colonisti.

		Functia ce recalculeaza bugetele colonistilor

			#pragma omp parallel for private (i, j)
			for (i=0; i<n; i++)
				for (j=0; j<n; j++) {
					recalculare_buget_nou;
				}

		De asemenea fiecare THREAD va calcula bugetele pentru colonistii aflatii
		pe aceeasi linie J

		Pentru afisarea informatiilor anuale : printParalelYearResult()

		#pragma omp parallel for private(i, j)
		for (i=0; i<n; i++) {

			gasirea_maxim_pret_linie
			calculare_numar_colonisti_linie

			#pragma omp critical
			{
				actualizare_maxim_pret
				actualizare_numar_colonisti_pt_resursa_X
			}
		}

		Fiecare THREAD va pargurge o linie a matricii si va cacula un maxim local.
		Apoi se va calculeaza maximul global in sectiunea #pragma omp critical.
		Se face update-ul de catre un singur THREAD la un moment dat.

		Timpi rulsare QSUB:

		========================================================================
			FISIER		TIME_FRAME		THREADS			TIMP		SHEDULE
		========================================================================
		in100_1.txt			 10				4		  0.839s		 static,2
		in100_1.txt			 10				3		  1.127s		 static,1
		in100_1.txt			 10				2		  1.669s		 static,4
		in100_1.txt			 10				1		  3.253s		 static
		in100_1.txt			100				4		  8.210s		 static,12
		in100_1.txt			100				3		 10.985s		 static,16
		in100_1.txt			100				2		 16.307s		 static,4
		in100_1.txt			100				1		 32.218s		 static,2
		in100_1.txt			100				8		  4.337s		dynamic
		in100_1.txt			100				8		  4.346s		 guided
		in100_1.txt			100				8		  4.282s		 static


		Comparativ SERIALA:

		in100_1.txt			100			serial		 28.813s

		Se poate observa scalarea foarte eficienta a algoritmului in functie de
		numarul de procesoare alocate.

		S = 28.8 / 4.28 = 6.72		8 threads			E = S/P = 0.84
		S = 28.8 / 8.21 = 3.50		4 threads			E = S/P = 0.88

		Speed-up este destul de constant indiferent de numarul de thread-uri.

	3)	SERIAL Optimizata

		Algoritmul serial optimizat se bazeaza pe calcularea pretului minim al
		resurselor cumparate de toti colonistii in cadrul aceleasi parcurgeri de
		matrice.

		I

		Algortimul se poate extinde la matrice plecand de la 1 linie.
		Prin 2 parcurgeri consecutive a unei linii, de la stanga la dreapta iar
		apoi de la dreapta la stanga se poate calcula minimul resursei cumparate
		pentru fiecare colonist in parte de pe acea linie.

		a) De la stanga la dreapta.

		Se ia minimul dintre min curent si pretul vechinului din stanga si
		se aduna 1 ce reprezinta distanta Manhattan pana la colonistul actual.
		In cazul in care resursele sunt de acelasi timp se face comparatia si cu
		pretul colonistului actual.

		Se seteaza minimul in functie de cel de sus.
		Minimul pentru fiecare resursa in parte este incrementat pentru a considera
		distanta Manhattan pana urmatorul colonist ce trebuie analizat.

		b) De la dreapta la stanga
		Se compara la fel, comparandu-se si cu preturile deja calculate precedent


		II

		Pentru o matrice trebuie urmat aplicat o versiune modificata a acestui
		algoritm si anume: Se calculea minimul pe linii , se salveaza matricea
		intro matrice auxilizara. Apoi se calculeaza minimul pe coloane considerand
		doar calculele facute la minimele pe linii (se ignora preturile).

		Se compara matricea rezultanta cu cea auxilizara si se retine minimul
		fiecarui element in parte

		Algoritmul se poate aplica si invers : min (coloane->linii, coloane)

		Numarul de parcurgeri al matricii pentru 1 an este o constanta C, =>

		COMPLEXITATE: O(T * n^2) 	,T numarul de ani

		Timpi rulsare QSUB:

		===================================================
			FISIER			TIME_FRAME			  	TIMP
		===================================================
		in100_1.txt				1000				 0.691s
		in100_1.txt				2000				 1.336s
		in50_1.txt				500					 0.094s
		in50_1.txt				5000				 0.784s


		Se poate observa complexitatea extrem de mica comparativ cu varianta
		neoptimizata.


	3)	PARALELA Optimizata

		Pentru varianta paralelizata a algoritmului optimizat am utilizat
		aceleasi functii de paralelizate de la varianta neoptimizata pentru :
			calculul bugetului urmatorului an
			calculul informatiilor anuale

		Aceste functii sunt deja eficiente deci nu mai pot fi optimizate.

		Parlelizarea calculului matricelor de minime am facut-o prin utlilizarea
		a 4 instructiuni #pragma omp parallel for pentru fiecare for principal :

			1) aplicarea algoritmului precedent pe linii
				- fiecare Thread calculeaza cate o noua linie pe rand.
			2) matricea rezultanta se copiaza paralel in matricea auxilizare
				- fiecare Thread copiaza cate o linie pe rand
			3) se aplica algoritmul pentru coloane
				- fiecare Thread calculeaza cate o noua linie
			4) se compara 1) cu 3) si se retine minimul
				- fiecare Thread se ocupa de cate 1 line pe rand


		Numarul de linii/coloane ce revin unui Thread sunt date de chunck size.
		Algoritmul se plica paralel pe cate linie/coloana in acelasi timp pentru
		fiecare thread.

		Timpi rulsare QSUB:

		========================================================================
			FISIER		TIME_FRAME		THREADS			TIMP		SHEDULE
		========================================================================
		 in50_1.txt			5000			1		  0.900s		 static
		 in50_1.txt			5000			2		  0.762s		 static,1
		 in50_1.txt			5000			3		  0.713s		 static,4
		 in50_1.txt			5000			4		  0.795s		 static,4
		 in50_1.txt			5000			8		  0.908s		 static,4
		 in50_1.txt			5000			1		  0.822s		dynamic
		 in50_1.txt			5000			2		  0.704s		 static,4
		 in50_1.txt			5000			4		  0.707s		dynamic,2
		 in50_1.txt			5000			8		  0.838s		dynamic,6

		in100_1.txt			2000			1		  1.358s		dynamic,64
		in100_1.txt			2000			1		  1.354s		 guided,32
		in100_1.txt			2000			2		  0.935s		 guided,32
		in100_1.txt			2000			4		  0.632s		 guided,32
		in100_1.txt			2000			8		  0.757s		 guided,32
		in100_1.txt			2000			4		  0.757s		 guided,32
		in100_1.txt			2000			4		  0.670s		dynamic,64
		in100_1.txt			2000			4		  0.646s		 guided,64

		in100_1.txt			2000			2		  0.931s		 static,16
		in100_1.txt			2000			4		  0.946s		 static,128
		in100_1.txt			2000			4		  0.938s		 static,32
		in100_1.txt			2000			4		  0.917s		 static,16
		in100_1.txt			2000			4		  0.941s		 static,4

		in100_1.txt			2000			2		  0.930s		dynamic,16
		in100_1.txt			2000			2		  0.941s		dynamic,2
		in100_1.txt			2000			4		  0.625s		dynamic,1
		in100_1.txt			2000			4		  0.672s		dynamic,16
		in100_1.txt			2000			4		  0.649s		dynamic,32
		in100_1.txt			2000			8		  0.741s		dynamic,64
		in100_1.txt			2000			8		  0.737s		dynamic,2


		in100_1.txt			10000			1		  6.461s		dynamic,32
		in100_1.txt			10000			2		  4.534s		dynamic,32
		in100_1.txt			10000			4		  3.163s		dynamic,32
		in100_1.txt			10000			8		  3.703s		dynamic,32

		Se poate observa diferenta de paralelizare in functie de ce SHEDULE este
		utilizat. Static ofera cea mai slaba paralelizare, cu maxim 2 thread-uri.
		Dynamic si Guided ofera paralelizare buna pana la 4 thread-uri. Pentru 8
		threaduri paralelizarea este mai slaba putin peste 4 thread-uri.

		S = 1.336 / 0.625s = 2.13		4 threads			E = S/P = 0.53
		S = 1.336 / 0.737s = 1.81		8 threads			E = S/P = 0.22

		OBSERVATIE:
			Din pacate algoritmul aplicat este extrem de eficient iar paralelizarea,
		desi prezenta, nu este foarte eficienta. Algortimul este foarte eficient,
		rezultatele apropiate putand fi cauzate si de scriere datelor in fisierul
		de output.

		Daca dimensiunea matricei de test ar fi fost mai mare paralelizarea ar fi
		fost mult mai eficienta.


********************************* Fisiere **************************************

	1) serial.cpp		- algoritmul neoptimizat serial
	2) paralel.cpp		- algoritmul neoptimizat paralel
	3) optims.cpp		- algoritmul optim serial
	4) optimp.cpp		- algoritmul optim paralel

	5) main.h			- functiile de baza de citire, alocare, variabilele
						globale, imporata de toate sursele
	6) paralel.h		- functiile paralele de calcul a bugetului si a
						informatiilor pentru fiecare an, imporata de sursele
						paralele

	7) Readme 			- explicatii
	8) Makefile
	8) run.sh


********************************* Rulare ***************************************

	Se compileaza utilizand Makefile-ul
	Se poate utiliza scriptul bash run.sh pentru a rula testele

	Specificatii run.sh

	./run.sh SOURCE YEAR FILE_IN FILE_OUT THREADS SHEDULE CHUNCK_SIZE

	* atat pentru varianta seriala cat si pentru cea paralele se pot omite
	parametrii incepand cu THREADS

	EX: ./run.sh optimp 2000 ./teste/in100_1.txt out.txt 4 1 16

	INFO: "echo THREADS: 4 SCHEDULE: dynamic,16"

	================
	TYPE	SCHEDULE
	================
	0		static
	1		dynamic
	2		guided

	CHUNCK_SIZE - int number

	De asemenea se poate specifica DIRECTORUL de unde se gaseste FILE_IN sau
	folosi o cale relativa catre FILE_IN

	TEST_DIR="."






