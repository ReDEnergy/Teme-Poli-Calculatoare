#ifndef _Min_Heap_H
#define _Min_Heap_H

typedef unsigned int	uint;
/** Structuri pentru Heap si HeapNode
 **/
typedef struct _HeapNode {
	unsigned long int freq;
	unsigned long int index;
	unsigned char symb;
} HeapNode;

/** Se retine frecventa de aparitie a fiecarui simbol , pozitia acestuia in
 ** arborele Huffman Tree si caracterul in sine
 **/
typedef struct _Heap {
	HeapNode *values;
	int dim;
	int max;
} Heap;

/**	Header functii
**/
HeapNode* NewHeapNodes (HeapNode *X, uint *v, int n);
Heap* NewHeap(HeapNode *values, int dim);

void Swap(Heap *H, int poz1, int poz2);

inline int Parinte		(int poz );
inline int LeftChild	(int poz );
inline int RightChild	(int poz );

void DownHeap (Heap *h, int poz);
void UpHeap	  (Heap *h, int poz);

HeapNode Pop (Heap *H);
void Push (Heap *H, HeapNode val);

void MakeMinHeap (Heap *H);
#endif
