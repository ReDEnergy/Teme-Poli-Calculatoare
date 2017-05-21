#include <stdio.h>
#include <stdlib.h>

FILE *f1 , *f2;

typedef struct
{
	int nr;
	int **v;
} dictionar;

int main ()
{
	int i,n,card,x,niv,j,k;
	int *A;
	int nra,p;

	f1=fopen ("in.txt","r");
	f2=fopen ("out.txt","w");

	fscanf (f1,"%d",&n);
	fprintf (f2,"%d\n",n);

	dictionar **ceva;
	ceva = (dictionar**) malloc ((n+1) * sizeof (dictionar*));
		for (i=0;i<=n;i++) {
			ceva[i]=(dictionar*) malloc (sizeof(dictionar));
			ceva[i]->v =(int**) malloc (10 * sizeof (int**));
		}

	while ( (fscanf(f1,"%d",&card)) > 0 ) {
		ceva[card]->nr++;
		ceva[card]->v[ ceva[card]->nr - 1 ]= (int*) malloc ( (card+1) * sizeof(int*));

		niv=ceva[card]->nr;
		niv--;
		for (i=0;i<card;i++) {
			fscanf(f1,"%d",&x);
			ceva[card]->v[niv][i]=x;
		}
	}

	for (i=0;i<=n;i++) {
		fprintf (f2,"Pentru keya %i numarul de multimi este %d \n",i,ceva[i]->nr);
		x=ceva[i]->nr;

		for (j=0;j<x;j++) {
			for (k=0;k<i;k++)
				fprintf (f2,"%d ",ceva[i]->v[j][k]);
			fprintf (f2,"\n");
		}
	}

	// Citire vector A
	printf ("Cardinalul vectorului A: ");
	scanf("%d",&nra);
	A=(int*) malloc ( (nra)  * sizeof (int));
	for (i=0;i<nra;i++)
		scanf("%d",&A[i]);

	// Citire cardinal P
	printf ("\nCardinalul de search: ");
	scanf ("%d",&p);

	// Cautare acoperire cardinal
	int ver[10];
	for (i=0;i<nra;i++) 		ver[i]=0;


	ceva[p]->nr=x;
	for (i=0;i<nra;i++)
		for (j=0;j<x+1;j++)
			for (k=0;k<p;k++)
				if ( ceva[p]->v[j][k] == A[i] ) ver[i] = 1;

	// Afisare vectorul A
	fprintf (f2,"\nVecotrul citi A este \n");
	for (i=0;i<nra;i++)
		fprintf(f2,"%d ",A[i]);

	fprintf (f2,"\nCardinalul de search este %d\n",p);

	// Afisare vectorul A
	fprintf (f2,"\nVecotrul ver este \n");
	for (i=0;i<nra;i++)
		fprintf(f2,"%d ",ver[i]);



    return 1;
}
