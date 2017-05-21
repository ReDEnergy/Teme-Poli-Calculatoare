#include <stdio.h>
#include <stdlib.h>

FILE *f1, *f2;

void add(int *v, int *j, int val) {
	v[*j] = val;
	(*j)++;
}

int drop(int *v, int *i) {
	(*i)++;
	return v[(*i)-1];
}

int main () {
	f1 = fopen ("in.txt","r");
	f2 = fopen ("out.txt","w");


	int i, nod;
	int a[100][100];
	int N,C,R,x,y;
	int coada[1000];
	int viz[100], dist[100];
	int i1=0,i2=0;
	int orase = 1;

	for (i=2;i<=C;i++) {
		viz[i] = 0;
		dist[i] = 0;
	}

	fscanf(f1,"%d %d %d",&N, &C, &R);
	for (i=1; i<=C; i++) {
		fscanf (f1,"%d %d", &x, &y);
		a[x][y]=1;
		a[y][x]=1;
	}

	viz[1]=1;
	dist[1]=0;
	add (coada,&i2,1);

	while (i1 != i2) {
		nod = drop(coada,&i1);
		for (i=1; i<=C; i++) {
			if (a[nod][i] && !viz[i]) {
				dist[i]= dist[nod]+1;
				viz[i]=1;
				if (dist[i] < R) {
					add(coada, &i2, i);
					orase++;
				}
			}
		}
	}


	for (i=1; i<=C; i++)
		fprintf(f2, "%d ", dist[i]);
	fprintf(f2,"\n");

	for (i=1; i<=C; i++)
		fprintf(f2, "%d ",viz[i]);

		fprintf(f2, "\n Sunt visitate: %d  orase!",orase);

	return 0;
}

