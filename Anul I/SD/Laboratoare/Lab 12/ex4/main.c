#include <stdio.h>
#include <stdlib.h>

FILE *f1, *f2;

char a[32][32];
int v[32];

int main(){
	f1 = fopen("in.txt", "r");
	f2 = fopen("out.txt", "w");

    int i, j, M, N, x, y;

    int curs[32];
    int c, cr, sem = 1;


    fscanf(f1,"%d %d",&N,&M);

    for( i=0; i<M; i++ ){
        fscanf(f1,"%d %d", &x, &y);
		/** gradul de intrare la destinatie **/
		/** graf orientat **/
        v[y]++;
		/** relatia intre curs**/
        a[x][y] = 1;
    }

/** Parcurg graful si sterg fiecare nod care are gradul de intrare =0
	Scand gradul de intrare de la nodurile care depind de nodul sters
	Incrementez semestrul
**/

/** cr - numar de cursuri(noduri) ramase in graf **/
    cr = N;

    while( cr ){
        c = 0;
        fprintf(f2,"sem.%d - curs ",sem);

        for(i=1; i<=N; i++)
            if(v[i] == 0) {
                v[i] = -1; 	/// nod sters
                curs[c++] = i;
                /// Salvezi numarul cursurilor pentru scadea gradul
                /// dependentelor
                fprintf(f2,"%d ", i);
                cr--;
            }

/// Scad gradul cursurilor ce depid de cursurile sterse
		if (cr) {
			for(i=0; i<c; i++)
				for( j=1; j<=N; j++)
					if( a[curs[i]][j] )
						v[j]--;
			fprintf(f2,"\n");
			sem++;
		}
    }
    return 0;
}
