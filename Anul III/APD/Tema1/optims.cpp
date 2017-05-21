#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <climits>

#include "main.h"

/**
 **	Matrici auxiliare pentru copierea altor matrici
 **/
int **buy_copy;
int **own_copy;

/**
 **	Algoritmul de calcul pret minim pentru toti colonistii
 **
 **		Complexistate O(n^2)
 **/
void getMinPrice() {

	// Minimul corespunzator cumpararii resursei 0 respectiv 1
	int pMIN[2];

	// Se calculeaza pentru mai intai pentru linii
	for (int i=0; i<n; i++) {

		// Resetare minime
		pMIN[0] = pMIN[1] = INF;

		own_min[i][0] = INF;
		buy_min[i][0] = INF;

		// De la stanga la dreapta
		for (int j=1; j<n; j++) {
			bool lt = type[i][j-1];
			bool ct = type[i][j];

			// Se calculeaza minimele in cazul in care timpul resursei actuale
			// este acelasi cu tipul resursei precedente
			if (ct == lt) {
				pMIN[ct] = getMin(price[i][j], getMin(pMIN[ct], price[i][j-1]) + 1);
				own_min[i][j] = pMIN[ct];
				buy_min[i][j] = ++pMIN[!ct];
			}

			// Se calculeaza minimele in cazul in care timpul resursei actuale
			// este diferit de tipul resursei precedente
			else {
				pMIN[lt] = getMin(pMIN[lt], price[i][j-1]);
				buy_min[i][j] = ++pMIN[lt];
				own_min[i][j] = pMIN[ct] = getMin(price[i][j], pMIN[ct] + 1);
			}
		}

		// Resetare minime
		pMIN[0] = pMIN[1] = INF;

		// De la dreapta la stanga
		for (int j=n-2; j>=0; j--) {
			bool lt = type[i][j+1];
			bool ct = type[i][j];

			// Se calculeaza minimele in cazul in care timpul resursei actuale
			// este acelasi cu tipul resursei precedente
			if (ct == lt) {
				pMIN[ct] = getMin(price[i][j], getMin(pMIN[ct], price[i][j+1]) + 1);
				setMin(own_min[i][j], pMIN[ct]);
				setMin(buy_min[i][j], ++pMIN[!ct]);
			}

			// Se calculeaza minimele in cazul in care timpul resursei actuale
			// este diferit de tipul resursei precedente
			else {
				pMIN[lt] = getMin(pMIN[lt], price[i][j+1]);
				setMin(buy_min[i][j], ++pMIN[lt]);
				pMIN[ct] = getMin(price[i][j], pMIN[ct] + 1);
				setMin(own_min[i][j], pMIN[ct]);
			}
		}
	}


	// Se salveaza matricile in matrici auxilizare
	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++) {
			buy_copy[i][j] = buy_min[i][j];
			own_copy[i][j] = own_min[i][j];
		}

	// Se calculeaza minimele pe coloane
	for (int i=0; i<n; i++) {

		// Resetare minime
		pMIN[0] = pMIN[1] = INF;

		// De sus in jos
		for (int j=1; j<n; j++) {
			bool ct = type[j][i];
			bool lt = type[j-1][i];

			// Se calculeaza minimele pe coloana
			pMIN[!lt] = getMin(pMIN[!lt], buy_min[j-1][i]);
			pMIN[lt] = getMin(pMIN[lt], own_min[j-1][i]);

			// Se actualizeaza minimele in functie de tipul de resursa
			setMin(buy_min[j][i], ++pMIN[!ct]);
			setMin(own_min[j][i], ++pMIN[ct]);

		}

		// Resetare minime
		pMIN[0] = pMIN[1] = INF;

		// De jos in sus
		for (int j=n-2; j>=0; j--) {
			bool lt = type[j+1][i];
			bool ct = type[j][i];

			// Se calculeaza minimele pe coloana
			pMIN[!lt] = getMin(pMIN[!lt], buy_min[j+1][i]);
			pMIN[lt] = getMin(pMIN[lt], own_min[j+1][i]);

			// Se actualizeaza minimele in functie de tipul de resursa
			setMin(buy_min[j][i], ++pMIN[!ct]);
			setMin(own_min[j][i], ++pMIN[ct]);
		}
	}


	// Se calcumeaza minimul dintre preturile calculate:
	// 		- pentru aplicarea algortimului pe linii si
	//		- pentru aplicarea algoritmului pe linii apoi coloane

	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++) {
			setMin(own_min[i][j], own_copy[i][j]);
			setMin(buy_min[i][j], buy_copy[i][j]);
		}
	}
}

/**
 **	MAIN FUNCTIONS
 **/
int main(int argc, char *argv[]) {

	// Stremurile de citire scriere
	fin.open(argv[2], fstream::in);
	fout.open(argv[3], fstream::out);

	// Numarul de ani
	time_frame = atoi(argv[1]);

	// Se citesc informatiile de input
	readInput();

	// Se aloca spatiu pentru matricile auxiliare
	newMatrix(buy_copy);
	newMatrix(own_copy);

	// Aplicarea algoritmului
	for (int i=0; i<time_frame; i++) {
		getMinPrice();
		newBudget();
		printYearResult();
	}

	// Afisarea matricii rezultante finale
	printFinalResult();

	// Se sterge memoria alocata dinamic
	deleteAll();

	// Se inchid streamurile
	fin.close();
	fout.close();

	return 0;
}
