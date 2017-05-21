********** Tema 3 APD
********** Ivanica Gabriel
********** 333CA


********************************************************************************
******************************* Implementare ***********************************


	Imaginea calculata este retinuta intr-un vector de char pentru a fi usor de 
	comunicat cu celelalte procese.

	Struct ReadFileInfo
		-	Pentru trimiterea de date catre procese (ce este necesar de calculat)
		si citirea din fisier, am folosit Structura FileInfo.
		

	Clasa Complex
		-	Am implementat o clasa Complex pentru a putea calcula usor operatiile
		cu numere complexe
		

********************************* Algoritm *************************************

	

	Master
	
		-	Procesul Master citeste informatiile din fisier dupa care trimite celorlalte
			procese structura citita.
			

		-	Imaginea este impartina in atatea bucati cate procese exista.
			Fiecare proces calculeaza imaginea aplicand algoritmul, iar la final trimite
			catre Master toate datele.
			
		-	Mastreul uneste toate datele in acelasi vector si scrie in fisier imaginea
			
	
	Procese
	
		-	Un proces primeste de la Master structura FileInfo
		
		-	In functie de Rank fiecare proces isi calculeaza propria parte din imagine 
			(dimensiune / nr_procese * rank)
			
		-	Dupa calcul, procesul trimite Materului bucata de imagine
		
		
		
	Escalabilitate
		-	Am testat mai multe imagini pe un numar diferit de procesoare
		-	Timpi sunt foarte variabili , am testat fiecare test de mai multe ori si am
			selectat timpul minim pentru
		
		-	Timpii sunt extrem imprecisi : diferente pe acelasi test putand sa varieze foarte mult
				ex - julia6 - variaza pana la 3 secunde pe test
					mandelbrot1 - probabil din cauza dimensiunii de 50 de MB am avut timpi si de 2 minute
			
		
		TEST	PROCESE		1			2			3			4			8			16
		-------------------------------------------------------------------------------------
		julia6				2.54		2.28		2.21		1.92		1.743		1.665
				
		mandelbrot1			41.370		28.844		24.477		21.363		19.450		14.083
		
		
		se observa ca in conditii optime de testare pe cluster algoritmul scaleaza
		la testele mandelbrot intrucat dureaza mai mult algoritmul scaleaza mult mai bine

********************************* Rulare ***************************************

	make / make build complileaza
	
	make mandel	T=nr_test N=nr_procese
	make julia	T=nr_test N=nr_procese
	
	make mandeltest	T=nr_test
	make juliatest	T=nr_test
	

	QSUB
	qsub -q ibm-quad.q run.sh ~/APD/Tema3/Tema3 FILE_IN FILE_OUT NR_PROCESSES
	
	EX
	qsub -q ibm-quad.q run.sh ~/APD/Tema3/Tema3 mandelbrot4.in m4.pgm 8
	


********************************* Fisiere **************************************

	1) Tema3.cpp
	2) Complex.cpp
	3) Makefile
	4) Readme

