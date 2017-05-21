#include <stdlib.h>
#include <fstream>
#include <climits>
#include <omp.h>

using namespace std;

/** Variable globale
 ** 			 n	-	dimensiunea unei matrici
 ** 	time_frame	-	numarul de ani pe care se calculeaza
 ** 	Pmin, Pmax	-	pretul minim si maxim
 **			   INF	-	Numar foarte mare , echivalent INFINIT
 **/

int n;
int time_frame;
int Pmin, Pmax;
int INF = INT_MAX-10000;


/**
 ** Matrici necesare pentru a retine informatiile citite si calculate
 ** Pentru colonistul [i][j]:
 ** 	type	-	tipul resursei produse
 ** 	price	-	pretul resuresei produse
 ** 	budget	-	bugetul anual pentru a cumpara resursa complementara
 ** 	buy_min	-	pretul minim platit pentru resursa complementara
 ** 	own_min	-	pretul minim platit pentru resursa produsa
 **/
bool **type;
int **price;
int **budget;
int **buy_min;
int **own_min;

/**
 ** Stream de citire si scriere
 **/
fstream fin;
fstream fout;

/**
 ** Functia necesara pentru alocarea unei matrici N*N de tip T
 **/
template<typename T>
void newMatrix(T** &M) {
	M = new T*[n];
	for (int i=0; i<n; i++)
		M[i] = new T[n];
}

/**
 ** Functia necesara a citi informatiile din fisier intr-o matrice N*N
 **/
template<typename T>
void readMatrix(T **M) {
	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++)
			fin >> M[i][j];
}

/**
 ** Functie de debuging utilizata la printarea unei matrici in fisier de output
 **/
template<typename T>
void printMatrix(T **M) {
	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++)
			fout << M[i][j] <<" ";
		fout<<"\n";
	}
	fout<<"\n";
}

/**
 ** Functie generala de minim
 ** Minimul este salvat in prima varibila a functiei
 **/
template<typename T>
void setMin(T &a, const T &b) {
	if (b < a)
		a = b;
}


template<typename T>
void setMax(T &a, const T &b) {
	if (b > a)
		a = b;
}

/**
 ** Functie generala de minim
 ** Se returneaza minimul dintre cei 2 parametri
 **/
template<typename T>
T getMin(const T &a, const T &b) {
	if (b < a)
		return b;
	return a;
}

/**
 ** Functie sterge matricele alocate dinamic
 **/
void deleteAll() {
	for (int i=0; i<n; i++) {
		delete type[i];
		delete price[i];
		delete budget[i];
		delete buy_min[i];
		delete own_min[i];
	}

	delete type;
	delete price;
	delete budget;
	delete buy_min;
	delete own_min;
}

/**
 ** Se aloca spatiu pentru toate matricele necesare
 ** Se citeste inputul din fisier
 **/
void readInput() {
	fin >> Pmin;
	fin >> Pmax;
	fin >> n;

	newMatrix(type);
	newMatrix(price);
	newMatrix(budget);
	newMatrix(buy_min);
	newMatrix(own_min);

	readMatrix(type);
	readMatrix(price);
	readMatrix(budget);

}

/**
 ** Modificarea tipului de resursa produsa de colonistul [i][j]
 ** Se modifica parametrii corespunzatori
 **/
inline void newBusinessModel(int i, int j) {
	type[i][j] = 1 - type[i][j];
	budget[i][j] = Pmax;
	price[i][j] = (Pmin + Pmax)/2;
}

/**
 ** Recalcularea bugetului pentru toti colonistii
 ** Se aplica regulile din cerinta
 **/
void newBudget() {
	int i, j;
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

/**
 ** Functie ce intoarce distanta Manhattan dintre 2 colonisti
 **/
inline int distManhattan(int x1, int y1, int x2, int y2) {
	return abs(x1 - x2) + abs(y1 - y2);
}

/**
 ** Functia afiseaza matricea finala
 ** Tuplu (tip_resursa, pret_resursa, buget)
 **/
void printFinalResult() {
	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++)
			fout<<"("<<type[i][j]<<","<<price[i][j]<<","<<budget[i][j]<<") ";
		fout<<"\n";
	}
}

/**
 **	Functia afiseaza informatiile necesare la fiecare an
 ** Numarul de Colonisti ce produc resurele A respectiv B
 ** Pretul maxim al resursei A respectiv B
 **/
void printYearResult() {
	int i, j;
	int NR[2] = {0, 0};
	int pMAX[2] = {0, 0};

	for (i=0; i<n; i++)
		for (j=0; j<n; j++) {
			NR[type[i][j]]++;
			pMAX[type[i][j]] = max(pMAX[type[i][j]], price[i][j]);
		}

	fout<<NR[0]<<" "<<pMAX[0]<<" "<<NR[1]<<" "<<pMAX[1]<<"\n";
}
