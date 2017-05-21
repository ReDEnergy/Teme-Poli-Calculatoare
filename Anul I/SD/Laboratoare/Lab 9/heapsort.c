#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

void HeapSort1(int *values, int dimVect)
{
	int i;
	HeapVector *h = CreazaVector(values, 0, dimVect);
	
	for (i = 0; i < dimVect; i++)
		AdaugaValoareMinHeap(h, values[i]);
	
	for (i = 0; i < dimVect; i++)
		values[i] = ExtrageMinim(h);
	
	ElibereazaVector(h);
}

void HeapSort2(int *values, int dimVect)
{
	int i;
	HeapVector *h = CreazaVector(values, dimVect, dimVect);
	
	ConstruiesteMinHeap(h);
	
	for (i = 0; i < dimVect; i++)
		values[i] = ExtrageMinim(h);
	
	ElibereazaVector(h);
}

void HeapSort3(int *values, int dimVect)
{
	int i;
	HeapVector *h = CreazaVector(values, dimVect, dimVect);
	
	ConstruiesteMaxHeap(h);
	
	for (i = h->dimVect - 1; i >= 1; i--)
	{
		InterschimbaValori(h, 0, i);
		h->dimVect--;
		
		CoboaraValoareMaxHeap(h, 0);
	}
	
	memcpy(values, h->values, dimVect * sizeof(int));
	
	ElibereazaVector(h);
}

int main()
{
	int dimVect, i;
	int *values = NULL;
	
	printf("dimVect = ");
	scanf("%d", &dimVect);
	
	values = malloc(dimVect * sizeof(int));
	
	for (i = 0; i < dimVect; i++)
	{
		printf("values[%d] = ", i);
		scanf("%d", &values[i]);
	}
	
	for (i = 0; i < dimVect; i++)
		printf("%d ", values[i]);
	printf("\n");
	
	HeapSort3(values, dimVect);
	
	for (i = 0; i < dimVect; i++)
		printf("%d ", values[i]);
	printf("\n");
	
	free(values);
	
	return 0;
}
