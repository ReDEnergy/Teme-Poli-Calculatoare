#ifndef _HEAP_H
#define _HEAP_H

typedef struct heap_vector_struct
{
	int *values;
	int dimVect;
	int capVect;
} HeapVector;

HeapVector* CreazaVector(int *values, int dimVect, int capVect);
void ElibereazaVector(HeapVector *h);
void InterschimbaValori(HeapVector *h, int poz1, int poz2);

int Parinte(HeapVector *h, int poz);
int DescS(HeapVector *h, int poz);
int DescD(HeapVector *h, int poz);

void CoboaraValoareMinHeap(HeapVector *h, int poz);
void UrcaValoareMinHeap(HeapVector *h, int poz);
int Minim(HeapVector *h);
int ExtrageMinim(HeapVector *h);
void ConstruiesteMinHeap(HeapVector *h);
void AdaugaValoareMinHeap(HeapVector *h, int val);

void CoboaraValoareMaxHeap(HeapVector *h, int poz);
void UrcaValoareMaxHeap(HeapVector *h, int poz);
int Maxim(HeapVector *h);
int ExtrageMaxim(HeapVector *h);
void ConstruiesteMaxHeap(HeapVector *h);
void AdaugaValoareMaxHeap(HeapVector *h, int val);

#endif
