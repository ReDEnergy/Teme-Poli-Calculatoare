#include <stdio.h>
#include <stdlib.h>

FILE *f1, *f2;

#define sus		a[x+1][y]
#define dreapta	a[x][y+1]
#define jos		a[x-1][y]
#define stanga	a[x][y-1]

char **a;

void gogo(int x, int y) {
	a[x][y]='*';
	if ( sus!='#' && sus!='*')
		gogo(x+1,y);
	if ( dreapta!='#' && dreapta!='*')
		gogo(x,y+1);
	if ( jos!='#' && jos!='*')
		gogo(x-1,y);
	if ( stanga!='#' && stanga!='*')
		gogo(x,y-1);
}


int main () {
	f1 = fopen ("in.txt","r");
	f2 = fopen ("out.txt","w");

	int i,j,M,N;
	char nextline;

	fscanf(f1,"%d %d ",&M, &N);

	a = malloc( (M+2) *sizeof(char*));
	for (i=1; i<=M+2; i++)
		a[i] = malloc( N+1 );

	fprintf(f2, "%d %d\n",M,N);

	for (i=1; i<=M; i++) {
		for (j=1; j<=N; j++)
			fscanf(f1,"%c", &a[i][j]);
	fscanf(f1,"%c",&nextline);
}


	gogo(5,9);
	a[5][9]='R';

	for (i=1; i<=M; i++) {
		for (j=1; j<=N; j++)
			fprintf(f2,"%c", a[i][j]);
		fprintf(f2,"\n");
	}

	return 0;
}


