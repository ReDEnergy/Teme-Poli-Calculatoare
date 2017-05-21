#include <stdlib.h>
#include <fstream>
#include <climits>
#include <omp.h>

using namespace std;

/**
 **	Functia afiseaza informatiile necesare la fiecare an
 ** Numarul de Colonisti ce produc resurele A respectiv B
 ** Pretul maxim al resursei A respectiv B
 **/
void printParalelYearResult() {
	int MAX[2] = {0, 0};
	int NR[2] = {0, 0};
	int i, j;

	// Paralelizare
	// Fiecare thread calculeza maximul pe linie si numarul de colonisti ce
	// produc resursa X
	#pragma omp parallel for private(i, j)
	for (i=0; i<n; i++) {
		int pNR[2] = {0, 0};
		int pMAX[2] = {0, 0};

		for (j=0; j<n; j++) {
			pNR[type[i][j]]++;
			setMax(pMAX[type[i][j]], price[i][j]);
		}

		// Sincronizare
		// Se actualizeaza maximul global si numarul global de colonisti
		// Se utlizeaza directiva critical pentru a se asigura ca actualizarea
		// este executata de un sigur THDREAD la un moment de timp
		#pragma omp critical
		{
			NR[0] += pNR[0];
			NR[1] += pNR[1];

			setMax(MAX[0], pMAX[0]);
			setMax(MAX[1], pMAX[1]);
		}
	}

	fout<<NR[0]<<" "<<MAX[0]<<" "<<NR[1]<<" "<<MAX[1]<<"\n";
}


/**
 ** Recalcularea bugetului pentru toti colonistii
 ** Se aplica regulile din cerinta
 **/
void newParalelBudget() {
	int i, j;

	// Fiecare thread recalculeaza bugetul pentru colonistii aflsati pe acceasi linie
	#pragma omp parallel for private (i, j)
	for (i=0; i<n; i++)
		for (j=0; j<n; j++) {
			if (buy_min[i][j] > budget[i][j]) {
				price[i][j] += buy_min[i][j] - budget[i][j];
				budget[i][j] = buy_min[i][j];

				if (price[i][j] > Pmax)
					newBusinessModel(i, j);

				continue;
			}

			if (buy_min[i][j] < budget[i][j]) {
				price[i][j] += (buy_min[i][j] - budget[i][j])/2;
				budget[i][j] = buy_min[i][j];

				if (price[i][j] < Pmin )
					price[i][j] = Pmin;
				continue;
			}

			if (budget[i][j] == buy_min[i][j]) {
				price[i][j] = own_min[i][j] + 1;

				if (price[i][j] > Pmax)
					newBusinessModel(i, j);
			}
		}
}
