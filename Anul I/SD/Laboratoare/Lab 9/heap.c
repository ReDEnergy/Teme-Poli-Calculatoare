#include "heap.h"
#include <stdlib.h>
#include <string.h>

HeapVector* CreazaVector(int *values, int dimVect, int capVect)
{
	HeapVector *h = malloc(sizeof(HeapVector));
	
	h->values = malloc(capVect * sizeof(int));
	memcpy(h->values, values, dimVect * sizeof(int));
	
	h->dimVect = dimVect;
	h->capVect = capVect;
	
	return h;
}

void ElibereazaVector(HeapVector *h)
{
	free(h->values);
	free(h);
}

void InterschimbaValori(HeapVector *h, int poz1, int poz2)
{
	int aux = h->values[poz1];
	h->values[poz1] = h->values[poz2];
	h->values[poz2] = aux;
}

int Parinte(HeapVector *h, int poz)
{
	if (poz <= 0 || poz >= h->dimVect)
		return -1;
	
	return (poz - 1) / 2;
}

int DescS(HeapVector *h, int poz)
{
	if (poz < 0)
		return -1;
	
	int ds = 2 * poz + 1;
	
	if (ds >= h->dimVect)
		return -1;
	
	return ds;
}

int DescD(HeapVector *h, int poz)
{
	if (poz < 0)
		return 0;
	
	int dd = 2 * poz + 2;
	
	if (dd >= h->dimVect)
		return -1;
	
	return dd;
}

void CoboaraValoareMinHeap(HeapVector *h, int poz)
{
	for (; ;)
	{
		int ds = DescS(h, poz);
		int dd = DescD(h, poz);
	
		int min = poz;
		if (ds != -1 && h->values[ds] < h->values[min])
			min = ds;
		if (dd != -1 && h->values[dd] < h->values[min])
			min = dd;
	
		if (min == poz)
			return;
	
		InterschimbaValori(h, poz, min);
		poz = min;
	}
}

void UrcaValoareMinHeap(HeapVector *h, int poz)
{
	int p;
	
	while ((p = Parinte(h, poz)) != -1 && h->values[poz] < h->values[p])
	{
		InterschimbaValori(h, poz, p);
		poz = p;
	}
}

int Minim(HeapVector *h)
{
	return h->values[0];
}

int ExtrageMinim(HeapVector *h)
{
	int min = h->values[0];
	
	InterschimbaValori(h, 0, h->dimVect - 1);
	h->dimVect--;
	
	CoboaraValoareMinHeap(h, 0);
	
	return min;
}

void ConstruiesteMinHeap(HeapVector *h)
{
	int i;
	
	for (i = h->dimVect / 2; i >= 0; i--)
		CoboaraValoareMinHeap(h, i);
}

void AdaugaValoareMinHeap(HeapVector *h, int val)
{
	h->dimVect++;
	h->values[h->dimVect - 1] = val;
	
	UrcaValoareMinHeap(h, h->dimVect - 1);
}

void CoboaraValoareMaxHeap(HeapVector *h, int poz)
{
	for (; ;)
	{
		int ds = DescS(h, poz);
		int dd = DescD(h, poz);
	
		int max = poz;
		if (ds != -1 && h->values[ds] > h->values[max])
			max = ds;
		if (dd != -1 && h->values[dd] > h->values[max])
			max = dd;
	
		if (max == poz)
			return;
	
		InterschimbaValori(h, poz, max);
		poz = max;
	}
}

void UrcaValoareMaxHeap(HeapVector *h, int poz)
{
	int p;
	
	while ((p = Parinte(h, poz)) != -1 && h->values[poz] > h->values[p])
	{
		InterschimbaValori(h, poz, p);
		poz = p;
	}
}

int Maxim(HeapVector *h)
{
	return h->values[0];
}

int ExtrageMaxim(HeapVector *h)
{
	int max = h->values[0];
	
	InterschimbaValori(h, 0, h->dimVect - 1);
	h->dimVect--;
	
	CoboaraValoareMaxHeap(h, 0);
	
	return max;
}

void ConstruiesteMaxHeap(HeapVector *h)
{
	int i;
	
	for (i = h->dimVect / 2; i >= 0; i--)
		CoboaraValoareMaxHeap(h, i);
}

void AdaugaValoareMaxHeap(HeapVector *h, int val)
{
	h->dimVect++;
	h->values[h->dimVect - 1] = val;
	
	UrcaValoareMaxHeap(h, h->dimVect - 1);
}
