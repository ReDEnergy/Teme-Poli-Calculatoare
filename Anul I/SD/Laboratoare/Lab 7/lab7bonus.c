#include <stdio.h>
#include <stdlib.h>

FILE *f1 , *f2;


int main ()
{
	int n;

	f1=fopen ("in.txt","r");
	f2=fopen ("out.txt","w");

	while ( (fscanf(f1,"%c",&n) ) >0 )

}
