********** Tema 2 APD
********** Ivanica Gabriel
********** 333CA


********************************************************************************
******************************* Implementare ***********************************

	Clasa Main 
		-	reprezinta si Threadul Master ce se ocupa de crearea Workerilor a 
		Threadurilor si distrubuirea tuturor taskurilor
	
	Clasa MapWorker
		-	implmenteaza paradigma MAP
		-	fiecare chunck citit este analizat, generandu-se un HashMap al
			cuvintelor si numarului de aparitii
			
	Clasa ReduceWorker
		-	implmenteaza paradigma REDUCE
		-	se unesc toate HashMap-urile unui fisier intr-unul singur			
		
	Clasa ReduceSolution
		-	implmenteaza paradigma REDUCE
		-	se sorteaza HashMap-ul pentru a determina valoarea ultimului cuvant indexat
		-	se analizeaza fiecare fisier daca contine cuvintele cautate
		
	Clasa WordCounter
		-	counter pentru numarul de aparitii

********************************* Algoritm *************************************

	Paradigma MAP
	
		-	Am utilizat un ThreadPool folosind ExecutorService
		
		-	Fiecare worker citeste o COUNT (sau mai putine daca EOF) caractere din 
			fisierul necesar incepand de la START_POS
		
		-	Verific daca primul caracter si cel precedent sunt litere pentru a
			a vedea daca cuvantul a fost deja indexat sau nu de alt worker 
	
		-	Am utilizat REG_EXP pentru delimitarea cuvintelor

		-	Construisc un HashMap cu numaruld e aparitii al cuvintelor


	Paradigma REDUCE
	
		Paradigma REDUCE este implementata in 2 pasi prin utilizarea a 2 clase diferite
		
		I) ReduceWorker
		
			-	Am folosit o Coada Blocanta Thread Safe : LinkedBlockingQueue
				
			-	Fiecare Thread extrage 2 Elemente din coada	pe care le concateneaza iar 
				rezultatul este adaugat in coada
				
			-	Am implementat o Clasa pentru calcularea numarului de aparitii al unui cuvant
			-	Clasa imi permite sa incrementez usor numarul de aparitii fara a mai fi
				necesar sa extrag valoarea din HashMap si sa creez o noua valoare 	
				
		II) ReduceSolution
			
			-	Sortez un HashMap dupa valori
			-	Nu este necesara calcularea frecventelor intrucat se pot calcula doar la afisarea
				raspunsului final
			-	Astfel calculez numarul de cuvinte total
			-	Salvez numarul de aparitii al ultimului cuvant indexat (cerinta)
			-	Astfel pot verifica fiecare cuvant cautat in cazul in care exista in HashMap
				sa aibe valoarea MAI MARE SAU EGALA cu valoarea ultimului cuvant indexat
			
			-	Initial presupun ca toate documentele sunt valide
			-	Parcurg toate cuvintele cautate
			-	Daca nu este gasit un cuvant in HashMap-ul fisierului respectiv
				voi pori executia workerului si voi seta documetul ca invalid
			-	Daca Se gaseste cuvantul in HashMap dar numarul de aparitii este mai mic decat
				numarul ultimului cuvant indexat documentul este invalid
			
	Final:
		Se parcurge vectorul de documente iar cele valide vor fi afisate impreuna 
		cu frecventele calculate pe baza valorii din HashMap si a numarului de aparitii al 
		cuvintelor
		
		Se afiseaza MAXIM nr_results fisiere


********************************* Fisiere **************************************

	1) Main.java
	2) MapWorker.java
	3) ReduceWorker.java
	4) ReduceSolution.java
	5) WordCounter.java
	6) Readme

