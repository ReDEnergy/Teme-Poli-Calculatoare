#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"
#define		freq(pos)	H -> values[pos].freq
#define		symb(pos)	H -> values[pos].symb
#define		index(pos)	H -> values[pos].index
#define 	Hnode(pos)	H -> values[pos]


/** Functia creaza un cevtor de HeapNode pentru Heap pe care in completeaza
 ** cu informatiile din vectorul de caractere ASCII ce trebuie encodate
 **/
HeapNode* NewHeapNodes (HeapNode *X, uint *v, int n) {
	int i, k = 0;

	for (i=0; i<256; i++) {
		if (v[i]) {
			X[k].freq = v[i];
			X[k].index = n + k - 1;
			X[k].symb = i;
			k++;
		}
	}
	return X;
}

/** Functia primeste un vector de HeapNode si dimensiunea pe care ii salveaza
 ** intr-un Heap.
 **/

Heap* NewHeap(HeapNode *values, int dimension) {
	Heap *H = malloc(sizeof(Heap));

	H->values = malloc( dimension * sizeof(HeapNode));
	memcpy(H->values, values, dimension * sizeof(HeapNode));

	H->dim = dimension;

	return H;
}

/**	Functia interschimaba 2 noduri din Heap
 **/
void Swap (Heap *H, int pos1, int pos2) {
	HeapNode aux = Hnode(pos1);
	Hnode(pos1) = Hnode(pos2);
	Hnode(pos2) = aux;
}

/** Functia intoarce parintele unui nod
 **/
inline int Parent (int pos) {
	return (pos - 1) / 2;
}

/** Functia intoarce fiul din drapta al unui nod
 **/
inline int RightChild (int pos) {
	return 2 * pos + 1;
}

/** Functia intoarce fiul din stanga al unui nod
 **/
inline int LeftChild (int pos) {
	return 2 * pos + 2;
}

/** Functia coboara un nod din Heap pentru a reface MinHeap
 **/
void DownHeap (Heap *H, int pos) {
	while(1) {
		int pmin = pos;
		int LC = LeftChild (pos);
		int RC = RightChild (pos);

		if ( LC < H->dim && freq(LC) < freq(pmin) )	pmin = LC;
		if ( RC < H->dim && freq(RC) < freq(pmin) )	pmin = RC;
		if ( pmin == pos )
			return;

		Swap (H, pos, pmin);
		pos = pmin;
	}
}

/**	Functia urca o valoare in Heap pentru reface MinHeap
 **/
void UpHeap (Heap *H, int pos) {
	int p;
	p = Parent (pos);
	while ( freq(pos) < freq(p) ) {
		Swap(H, pos, p);
		pos = p;
	}
}

/**	Functia scoate valoare minima din MinHeap , radacina pe care o returneaza
 **/
HeapNode Pop (Heap *H) {
	HeapNode min = Hnode(0);

	H -> dim --;
	Swap (H, 0, H->dim);
	DownHeap (H, 0);

	return min;
}

/**	Functia adauga o valoare in MinHeap
 **/
void Push (Heap *H, HeapNode node) {
	H->dim++;
	Hnode(H->dim-1) = node;
	UpHeap(H, H->dim - 1);
}

/**	Functia creaza un MinHeap pornind de la un Heap
 **/
void MakeMinHeap(Heap *H) {
	int i;

	for (i = H->dim / 2; i >= 0; i--)
		DownHeap(H, i);
}
