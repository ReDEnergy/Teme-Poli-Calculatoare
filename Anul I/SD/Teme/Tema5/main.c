# include <stdio.h>
# include <stdlib.h>

FILE *f1;

/**	Matricea de adiacenta a hartii de pomi
 **	Vectorul de noduri vizitate set
 **/
int harta[1501][1501];
int set[1501];

/**	Structura necesara pentru ordonarea pomilor dupa numarul de vecini
 **/
typedef struct ord {
	int nr_vecini;
	int id;
} Ord;

/**	Functia de comparare
 **	Se ordoneaza crescator dupa dumarul de vecini si descrescator dupa id
 **/
int compare ( const void *a, const void *b) {
	Ord *x = (Ord*) a;
	Ord *y = (Ord*) b;

	int dif_vecini  = y->nr_vecini - x->nr_vecini;
	int dif_id = y->id - x->id;
	if (dif_vecini == 0 )
		return dif_id;
	return dif_vecini;

}

/**	Functia reinitializeaza matricea de adiacenta cu 0
 **/
void seteaza0 (int N) {
	int i,j;
	for (i=0;i<N;i++)
		for (j=i;j<N;j++) {
			harta[i][j] = 0;
			harta[j][i] = 0;
		}
}

/**	Functia afiseaza rezultatul : numarul de soldati si copacii pe care trebuie
 **	pozitionati soldatii
 **/
void rezultat (int nr_of_guards, int *set, int N) {
	int i;
	printf("%d: ",nr_of_guards);
	for(i=0;i<N;i++) {
		if (set[i] ==2)
			printf("%d ", i);
	}
	printf("\n");

}

int main () {

	f1 = fopen("input","r");

	int N;
	int i, j, id, nr_vecini, vecin;
	int nr_of_guards, to_guard;
	char skip;


	while( (fscanf(f1,"%d",&N)) > 0 ) {

		/** Se seteaza cu 0 matricea de adiacenta**/
		seteaza0(N);

		/** Alocare memorie pentru structura de sortare **/
		Ord *sort;
		sort = (Ord*) calloc (N, sizeof(Ord));

		/**	Citirea matricii de adiacenta si setarea vecinilor **/
		for (i=0; i<N; i++) {
			fscanf (f1, "%d", &id);
			sort[id].id = id;
			fscanf (f1, "%c %c", &skip,&skip);
			fscanf (f1, "%d", &nr_vecini);
			fscanf (f1, "%c", &skip);
				for (j=0; j<nr_vecini; j++) {
				fscanf(f1, "%d", &vecin);
					harta[id][vecin] = 1;
				harta[vecin][id] = 1;
					sort[id].nr_vecini ++;
				sort[vecin].nr_vecini ++;
			}
		}

		/** Ordonarea vectorului de structuri dupa numarul de vecini **/
		qsort (sort, N, sizeof(Ord), compare );

		/**	Se seteaza vecotrul de copaci aparati cu 0
		 **	Numarul de soldati pozitionati = 0
		 **	Numarulde copaci ce trebuie aparati = N
		 **/
		to_guard = N;
		nr_of_guards = 0;
		for (i=0;i<N;i++)
			set[i]=0;

		i = -1;
		/**	Cat timp mai sunt copaci de aparat se pozitioneaza un soldat
		 **	pe copacul (nevizitat) ce are cel mai mare numar de vecini
		 **	si se steaza toti vecinii ca aparati.
		 **	set = 1 ->	aparat
		 **	set = 2 ->	soldat
		 **/
		while (to_guard > 0) {
			i++;
			id = sort[i].id;
			if (set[id])
				continue;

			for (j=0; j < N; j++)
				if ( harta[id][j] == 1)
					if (set[j] == 0 ) {
						set[j] = 1;
						to_guard --;
					}

			if (set[id] == 0)
				to_guard--;
			set[id] = 2;
			nr_of_guards++;
		}

		/**	Afisarea raspunsului **/
		rezultat(nr_of_guards, set, N);

		/**	Eliberare memorie **/
		free(sort);
	}

	/**	Inchidere fisier **/
	fclose(f1);
	return 0;
}
