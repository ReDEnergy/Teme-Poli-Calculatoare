#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <climits>

#include "main.h"

/**
 **	Algoritmul de calcul pret minim pentru colonistul [i][j]
 **		Complexistate O(n^2)
 **/
inline void getMinPriceForNode(int i, int j) {
	int k, l;

	// pretul minim pentru cumpara resursa 0 respectiv 1
	int MIN[2] = {INF, INF};

	// tipul de resursa produs de colonist
	bool t = type[i][j];

	for (k=0; k<n; k++)
		for (l=0; l<n; l++) {
			int new_price = distManhattan(i, j, k, l) + price[k][l];

			// tipul de resursa produs de colonistul de la care se cumpara
			bool ot = type[k][l];

			setMin(MIN[ot], new_price);
		}

	// Minimele corespunzatoare cumpararii resurselor
	buy_min[i][j] = MIN[!t];
	own_min[i][j] = MIN[t];
}

/**
 **	Algortimul de calcul minime pentru toti colonistii
 **		Complexistate O(n^4)
 **/
void getMinPrice() {
	int i, j;
	for (i=0; i<n; i++)
		for (j=0; j<n; j++)
			getMinPriceForNode(i, j);
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
