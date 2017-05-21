#include<stdio.h>
#include<stdlib.h>

/*	Programul citeste numere din fisierul de intrare si le ordoneaza
 *	in functie de functia de comparare
 */

FILE *f1,*f2;

//	Functie de comparare

int compar(const void *a, const void *b) {
    int *x = (int*)a;
    int *y = (int*)b;
    return *x-*y;
}

//	Quicksort

void quicksort(int arr[], int left, int right, int (*comp)() ) {
	int i = left;
	int j = right;
	int swap;
	int m = arr[(left + right) / 2];

	do {
		while( comp( &m, &arr[i] )>0 ) i++;
		while( comp( &arr[j], &m )>0 ) j--;
		if(i <= j)
		{
			swap   = arr[i];
			arr[i] = arr[j];
			arr[j] = swap;
			i++;
			j--;
		}
	} while(i <= j);

	if(left < j)	quicksort(arr, left, j, comp);
	if(i < right)	quicksort(arr, i, right, comp);
}

//	Citirea din fisier a numerelor

int citire (int *v) {
	int nr=0;
	while ( fscanf( f1,"%d",&v[nr] ) > 0 )
		nr++;
	return nr;
}

//	Afisarea in fisier a numerelor

void afisare (int *v,int n) {
	int i;
	for(i=0;i<n;i++)
		fprintf(f2,"%d ",v[i]);
}

//	Main program

int main()
{
    f1=fopen("in.txt","r");
    f2=fopen("out.txt","w");

    int i,nr,v[100];

	//	Citire numere din fisier
	nr=citire(v);

	//	Apelarea functiei de ordonare
	quicksort(v,0,nr-1,compar);

	//	Afisare
	afisare(v,nr);

	//	Inchidere fisiere
	fclose(f1);
	fclose(f2);
	return 1;
}
