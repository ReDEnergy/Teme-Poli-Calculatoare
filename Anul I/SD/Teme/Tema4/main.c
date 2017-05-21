#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <unistd.h>

#include "header.h"
#include "heap.h"
#define		BFDIM		4096
#define		index(pos)	H -> values[pos].index

FILE *f1, *f2;

/** Functia calculeaza numarul de caractere ASCII care trebuie encodate
 ** Se primeste vectorului in care s-au salvat aparitiile caracterelor
 **/
int CountChar (uint *v) {
	int i, nr_of_symbols = 0;

	for (i=0; i<256; i++)
		if (v[i]) nr_of_symbols++;

	return nr_of_symbols;
}

/** Functia salveaza nodurile initiale din Heap pe ultimele n pozitii ale
 ** vectorului HuffmanTree.	Vectorul HuffmanTree are 2*n-1 noduri , unde n
 ** reprezinta numarul de caractere ce trebuie encodate.
 **/
HuffmanNode* SaveLeaf (Heap *H) {
	int i;
	int n = H->dim;
	HuffmanNode *HTree;
	HTree = malloc( (2*n-1)*sizeof(HuffmanNode) );

	for (i=0; i<n; i++ ) {
		HTree[ index(i) ].value = H -> values[i].symb;
		HTree[ index(i) ].left = -1;
		HTree[ index(i) ].right = -1;
	}
	return HTree;
}

/** Functia calculeaza indicii radacinilor din vectorul HuffmanTree cat si fii
 ** acestor indici. Astfel se creaza arborele Huffman
 ** Cazul n=1 este tratat separat
 **/
HuffmanNode* CalculateRoot (HuffmanNode* HTree, Heap *H) {
	int n = H->dim;
	HeapNode A, B, S;

	while ( n > 1 ) {
		n--;
		A = Pop (H);
		B = Pop (H);
		S.freq = A.freq + B.freq;
		S.index = n-1;

		HTree[n-1].value = 238;
		HTree[n-1].left = A.index;
		HTree[n-1].right = B.index;
		Push (H, S);

	}
	return HTree;
}

/** Functia creaza tabela de translatie pentru encoding
 ** Se parcurge arborele Huffman tree in adangime recursiv
 ** Se salveaza la fiecare apelare valoarea encodingului pana in acel moment
 ** in variabila val (pentru fiecare nod nou apelat se sifteaza la stanga si se
 ** adauga valoarea respectiva directiei in val 0 : stanga si 1: dreapta)
 ** Cand se ajunge la o frunza se salveaza valoarea acesteia si nivelul frunzei
 ** in arborele Huffman - ce reprezinta numarul de biti pe care se scrie
 ** caracterul encodat
 ** In cazul in care trebuie ecodat un singur caracter, datorita impelemtarii
 ** arborelui Huffman fara radiacina, nivelul este 0, desi trebuie cel putin 1
 ** bit pentru reprezentare. Se incrementeaza cu 1 numarul de biti.
 **/
void SaveEncoding
(HuffmanNode *HTree, HuffmanNode Node, EncodeTable *Table, uint val, int nr) {
	if (Node.left>0)	{
		nr++;
		val = val<<1 ;
		SaveEncoding( HTree, HTree[Node.left], Table, val, nr);
	}
	if (Node.right>0)	{
		if (!Node.left) {
			nr++;
			val = (val<<1) | 1;
		}
		else val = val | 1;
		SaveEncoding( HTree, HTree[Node.right], Table, val ,nr);
	}

	if (Node.left <0 && Node.right <0) {
		if (nr==0)	nr++;
		Table[ Node.value ].val = val;
		Table[ Node.value ].nrbits = nr;
	}
}

/** Functia deshide binar pe rand fiecare fisier si citeste caracterele ASCII
 ** intalnite. Pentru fiecare caracter intalnit se incrementeaza frecventa
 ** de intalnire a acestuia.
 ** In cazul in care nu exista drepturi asupra fisierului se afiseaza un mesaj
 ** de eroare.
 ** Se construieste vectorul de frecvente si se intoarce varibila valid ce
 ** reprezinta tipul de fisiere ce trebuie encodate
 ** 		valid = 0 // Nu sunt fisiere de encodat
 ** 		valid = 1 // Este mai mult 1 caracter de encodat
 ** 		valid = 2 // Toate fisierele au dimensiunea 0 bytes
 **/
int Readfreq (int argc, char **argv, uint *v) {
	int i=0, j, nre, valid=0;
	uchar *buffer = malloc (BFDIM);

	for (i=3; i<argc; i++) {
		f1 = fopen (argv[i],"rb");
		if (f1 == NULL)
			fprintf(stderr,"Error: %s can't be opened\n", argv[i]);
		else {
			if (valid == 0) valid=2;
			while ( (nre = fread(buffer, 1, BFDIM, f1)) ) {
				for (j=0; j<nre; j++) v[(int)buffer[j] ]++;
				valid = 1;
			 }
			fclose(f1);
		}
	}
	free(buffer);

	if(valid == 0)
		fprintf(stderr,"Error: No input files\n");
	return valid;
}

int main (int argc, char **argv) {

if (strcmp (argv[1],"compress") == 0 ) {
	return compress(argc, argv);
}

if (strcmp (argv[1],"extract") == 0 ) {
	return extract(argv);
}

if (strcmp (argv[1],"list") == 0 )
	list(argv[2]);
	return 0;
}

/******************************************************************************/
/**						COMPRIMAREA FISIERELOR INTRO ARHIVA					 **/
/******************************************************************************/

/** Functia extrage numele unui fiser din path-ul corespunzator
 ** Algoritmul functioneaza astat pe Windows cat si pe Linux
 **/
char* ExtractFileName ( char *path) {
	int k=0;
	char *name=path;

	while (path[k]) {
		if(path[k] == 92 || path[k] == 47)	name = &path[k]+1;
		k++;
	}
	return name;
}

/** Functia verifica daca un fisier a fost deja adaugat in arhiva comparand
 ** numele acestuia cu cele salvate in vectorul de fisiere adaugate
 ** Returneaza 0 daca fisierul a fost adaugat
 ** Salveaza informatiile despre fisier in Headerul fisierlui , adauga fisierul
 ** in vectorul de fisiere , return 1
 **/
int AddFile (FileEntryHeader *F, char *name, char ***files, int nr) {
	int i;

	for (i=0; i<nr; i++)
		if ( strcmp ( (*files)[i], name) == 0 )
			return 0;

	*files = realloc(*files, (nr+1) * sizeof(char*));
	(*files)[nr] = calloc(30,1);

	strcpy( (*files)[nr], name);
	strcpy( F->file_name, (*files)[nr]);

	F->bytes_n=0;
	F->checksum=0;
	F->file_size=0;

	return 1;
}

/** Functia salveaza intr-o pozitie a bufferului bitii de encoding
 ** Returneaza numarul de biti ramasi de scris si numarul de bitii liberi
 **/
int Save_In_Buffer (uchar *buffer, uint encode, int bits, int *free) {
	if (bits > *free) {
		bits  -= *free;
		encode = encode >> bits;
		*buffer  = *buffer | encode;
		*free = 0;
		return bits;
	}
	if (bits < *free) {
		*free -= bits;
		encode = encode << *free;
		*buffer = *buffer | encode;
		return 0;
	}
	if (bits == *free) {
		*buffer = *buffer | encode;
		*free = 0;
		return 0;
	}
	return bits;
}

/** Functia Arhiveaza o un fisier pe care il scrie in Arhiva
 **/
void ArchiveFile (EncodeTable *Table, FileEntryHeader *F, FILE *f) {
	int i, index=0;
	int nr_of_bits, nr_of_symbol, free_bits = 8;
	uint encode;
	uchar *buffer = calloc(BFDIM, 1);
	uchar READ_BUFFER[BFDIM];
	uchar symbol=0;

	/** Se citesc un numar de symboluri din fisierul de arhivat**/
	while ( (nr_of_symbol = fread (READ_BUFFER, 1, BFDIM, f)) ) {

		/** Se actualizeaza numarul de bytes al fisierului **/
		F->file_size += nr_of_symbol;

		/** Fiecare symbol in parte este salvat in buffer
		 ** Se calculeaza checksumul fisierului
		 **/
		for (i=0; i<nr_of_symbol; i++) {

			symbol = READ_BUFFER[i];
			F->checksum += symbol;

			/** Bitii ce urmeaza sa fie scrisi si numarul lor **/
			encode = Table[symbol].val;
			nr_of_bits = Table[symbol].nrbits;

			/** Se scriu bitii de encoding in buffer prin apelare recursiva
			 ** in cazul in care sunt mai multi de 8 biti sau octetul curent
			 ** nu mai are numarul necesar de biti liberi
			 **/
			while (nr_of_bits) {
				nr_of_bits = Save_In_Buffer(&buffer[index], encode, nr_of_bits, &free_bits);
				if (index == BFDIM-1 && free_bits == 0) {
					F->bytes_n += BFDIM;
					fwrite (buffer, 1, index + 1, f2);
					index = -1;
				}
				if(free_bits == 0) {
					index++;
					buffer[index] = 0;
					free_bits = 8;
				}
			}
		}
	}

	/** Daca checksum >=0 sau fisierele contin doar symbol=NULL se arhiveaza **/
	if ( F->checksum || symbol==0) {
		 F->bytes_n += index + 1;
		fwrite (buffer, 1, index + 1, f2);
	}
	free(buffer);
}

/** Functia Arhiveaza fiecare fisier in parte si salveaza informatiile despre
 ** acestea in headerul corespunator fiecaruia
 ** Numarul de fisiere arhivate se salveaza in Headerul arhivei
 **/
void ArchiveFiles (ArchiveHeader *H, EncodeTable *Table, int argc, char **argv){
	int i, add, nr_of_files=0;
	long int poz_i;
	char *name;
	char **files = calloc(1,sizeof(char*));
	FileEntryHeader *F = calloc( 1 , sizeof (FileEntryHeader));

	for (i=3; i<argc; i++) {
		f1 = fopen (argv[i],"rb");
		if (f1) {
			name = ExtractFileName(argv[i]);
			add = AddFile (F, name, &files, nr_of_files);
			if (add) {
				nr_of_files++;
				H->file_entries_n = nr_of_files;

				poz_i = ftell(f2);
				fseek (f2, sizeof(FileEntryHeader), SEEK_CUR);
				ArchiveFile(Table, F, f1);

				fseek (f2, poz_i, SEEK_SET);
				fwrite (F, sizeof(FileEntryHeader), 1, f2);

				fseek (f2, 0, SEEK_END);
			}
		fclose(f1);
		}
	}

	for (i=0; i<nr_of_files; i++)
		free(files[i]);
	free (files);

	free (F);
}

/** Functia Arhiveaza fisiere NULE
 ** Sunt scrise doar Headerele fisierelor
 ** Aceeasi functie ca si cea precedenta dar se sare peste partea de Encoding
 **/
void NullArchive (ArchiveHeader *H, int argc, char **argv) {
	int i, add, nr_of_files=0;
	char *name;
	char **files = calloc(1,sizeof(char*));
	FileEntryHeader *F = calloc( 1 , sizeof (FileEntryHeader));

	for (i=3; i<argc; i++) {
		f1 = fopen (argv[i],"rb");
		if (f1) {
			name = ExtractFileName(argv[i]);
			add = AddFile (F, name, &files, nr_of_files);
			if (add) {
				nr_of_files++;
				H->file_entries_n = nr_of_files;
				fwrite (F, sizeof(FileEntryHeader), 1, f2);
			}
		fclose(f1);
		}
	}

	/** Eliberare memorie **/
	for (i=0; i<nr_of_files; i++)
		free(files[i]);
	free (files);
	free(F);
}

/** Functia de comprimare a fisierelor **/
int compress (int argc, char **argv) {

	/**			Variables 		**/
	int dim=0, skip_pos;
	uint *v = calloc(256, sizeof(int));
	/*****************************/

	int valid = Readfreq(argc, argv, v);
	if (valid == 0)	{
		free(v);
		return 0;
	}

	/** Deschiderea fisierului in care se va scrie arhiva **/
	f2 = fopen	(argv[2], "wb");


	ArchiveHeader *ArchH = calloc(1,sizeof(ArchiveHeader));
	dim = CountChar(v);

	/** Daca sunt caractere de encodat **/
	if (valid == 1 ) {
		/** Create HeapNodes and the MinHeap **/
		HeapNode *X =(HeapNode*) calloc ( dim , sizeof(HeapNode));
		HeapNode *HN = NewHeapNodes (X, v, dim);
		Heap *H = NewHeap (HN, dim);
		MakeMinHeap (H);

		/** Create the Huffman tree, save leafs and root positions **/
		HuffmanNode *HTree;
		HTree = SaveLeaf (H);
		HTree = CalculateRoot (HTree, H);

		/** Generarea tabelei de translatie pentru encoding **/
		EncodeTable *Table;
		Table = (EncodeTable*) calloc(256 , sizeof(EncodeTable));
		SaveEncoding (HTree, HTree[0], Table, 0, 0);

		/** Se sare peste octetii in care se vor scrie ArchiveHeader si
		 ** HuffmanNodes deoarece nu au toate informatiile necesare
		 **/
		skip_pos = (2*dim-1) * sizeof (HuffmanNode) + sizeof (ArchiveHeader);
		if (dim==1) skip_pos +=sizeof (HuffmanNode);
		fseek (f2, skip_pos, SEEK_SET);

		/** Se arhiveaza fisierele
		 ** In acelasi timp se calculeaza si numarul de visiere arhivate
		 **/
		ArchiveFiles (ArchH, Table, argc, argv);

		/** Se completeaza arhiva cu  ArchiveHeader si HuffmanNodes **/
		fseek (f2, 0 , SEEK_SET);
		ArchH->huffman_nodes_n = 2*dim-1;

		/** In cazul in care exista un sigur nod in HuffmanTree se adauga manual
		 ** o radacina si se incrementeaza numarul de noduri la 2.
		 **/
		if (dim==1) ArchH->huffman_nodes_n = 2;
		fwrite (ArchH, sizeof(ArchiveHeader), 1, f2);

		/** Adaugarea radacinii manuale **/
		if (dim==1) {
			HuffmanNode *ROOT = malloc (sizeof(HuffmanNode));
			ROOT->right = -1;
			ROOT->left = 1;
			ROOT->value = 0;
			fwrite (ROOT, sizeof(HuffmanNode), 1, f2);
		}
		fwrite (HTree, sizeof(HuffmanNode), 2*dim-1, f2);

		/** Eliberare de memorie **/
		free(HN);
		free(H->values);
		free(H);
		free(HTree);
		free(Table);
	}

	/** Daca toate fisierele sunt NULE , 0 bytes in size **/
	if (valid == 2) {
		fwrite (ArchH, sizeof(ArchiveHeader), 1, f2);
		NullArchive (ArchH, argc, argv);
		fseek (f2, 0 , SEEK_SET);
		fwrite (ArchH, sizeof(ArchiveHeader), 1, f2);
	}

	/** Eliberare memorie **/
	free(v);
	free(ArchH);
	fclose(f2);
	return 0;
}

/******************************************************************************/
/**						LISTAREA FISIERELOR DIN ARHIVA						 **/
/******************************************************************************/

/** Functia de comparare pentru listarea alfabetica a fisierelor din ARHIVA
 **/
int compare (const void *a, const void *b) {
	char *x = ((FileEntryHeader*) a)->file_name;
	char *y = ((FileEntryHeader*) b)->file_name;
	return strcmp ( x, y);
}

/** Functia de listare a fisierelor din arhiva
 **/
void list (char *arhiva) {
	f1 = fopen (arhiva,"rb+");

	int i, nr_nodes, nr_files;

	/** Se citeste headerul arhivei **/
	ArchiveHeader *ArchH = malloc (sizeof(ArchiveHeader));
	fread (ArchH, sizeof(ArchiveHeader),1,f1);

	/** Intrucat HuffmanTree nu este important se sare peste acesta **/
	nr_files = ArchH->file_entries_n;
	nr_nodes = ArchH->huffman_nodes_n;
	fseek (f1, nr_nodes * sizeof(HuffmanNode) , SEEK_CUR);

	/** Se citeste fiecare fisier pe rand intr-o positie a unui vector
	 ** Se sare peste octetii ce reprezinta arhivarea fisierelor
	 **/
	FileEntryHeader *F = malloc( nr_files * sizeof(FileEntryHeader));
	for (i=0; i<nr_files; i++) {
		fread (&F[i], sizeof(FileEntryHeader), 1, f1);
		fseek (f1, F[i].bytes_n, SEEK_CUR);
	}

	/** Se ordoneaza fisierele dupa nume si se afiseaza
	 **/
	qsort(F, nr_files, sizeof(FileEntryHeader), compare);
	for (i=0; i<nr_files; i++)
		printf ("%-30s %d\n", F[i].file_name, F[i].file_size);

	/** Eliberare de memorie
	 **/
	free(F);
	free(ArchH);
	fclose(f1);
}

/******************************************************************************/
/**						EXTRAGEREA FISIERELOR DIN ARHIVA					 **/
/******************************************************************************/

int extract (char **argv) {
	int i;

	/** Se deschide arhiva **/
	f1 = fopen (argv[2],"rb");

	/** Se verifica daca avem dreptul de extragere in fisierul argv[3]
	 ** In caz contrar se iese din program si se afiseaza mesajul de eroare
	 **/
	if (argv[3]) {
		int ok = chdir(argv[3]);
		if (ok) {
			fprintf(stderr,"Error: Can't open %s\n",argv[3]);
			fclose(f1);
			return 0;
		}
	}

	/** Se citeste Headerul arhivei **/
	ArchiveHeader *ArchH = malloc (sizeof(ArchiveHeader));
	fread (ArchH, sizeof(ArchiveHeader),1,f1);

	int nr_files = ArchH->file_entries_n;
	int nr_nodes = ArchH->huffman_nodes_n;

	/** Se citeste arbore Huffman **/
	HuffmanNode *HTree = malloc(nr_nodes * sizeof(HuffmanNode) );
	fread(HTree, sizeof(HuffmanNode) ,nr_nodes, f1);


	FileEntryHeader *F = malloc( sizeof(FileEntryHeader));
	/** Se citeste  pe rand cate fisier din arhiva si se decodifica **/

	/** Se citeste BFDIM caractere pe rand in buffer **/
	uchar *buffer = malloc(BFDIM);

	for (i=0; i<nr_files; i++) {
		fread(F, sizeof(FileEntryHeader), 1, f1);
		f2 = fopen(F->file_name,"wb");

		int file_size = F->file_size;
		int bytes_n = F->bytes_n;
		unsigned int checksum=0;
		int child, index=BFDIM+1;

		/** Bitul de verificare al valorii buferului pe o pozitie **/
		int bit_nr = 7;

		/** Se initiaza pozitia cu radacina arborelui Huffman **/
		int pos=ArchH->huffman_root_node;

		/** Daca mai sunt caractere de scris in fisier se continua **/
		while ( file_size ) {
			/** Daca bitul de verificare este -1 se reinitializeaza
			 ** Se trece la urmatorul byte prin cresterea indexului
			 **/
			if(bit_nr == -1) {
				bit_nr = 7;
				index++;
			}
			/** Daca bufferul este parcurs se citeste din nou in acesta
			 ** Se reinitializeaza index = 0 si se actualizeaza numarul de
			 ** octeti ce mai trebuie cititi
			 **/
			if (bytes_n && index >= BFDIM) {
				if (bytes_n >= BFDIM) {
					fread (buffer, 1, BFDIM, f1);
					bytes_n -= BFDIM;
					index = 0;
				}
				else {
					fread (buffer, 1, bytes_n, f1);
					bytes_n = 0;
					index =0;
				}
			}

			/** Se verifica tipul bitului 0: stanga si 1: dreapta
			 **/
			child = ( buffer[index] & (1<<bit_nr) );
			if ( child ) pos =  HTree[pos].right;
			else pos =  HTree[pos].left;
			bit_nr--;

			/** Daca Nodul din Huffman Tree este o frunza se scrie in fisier
			 ** caracterul , se reinitializeaza pozitia pe Radacina HuffmanTree
			 ** se actualizeaza checksum si numarul de caractere ramase de scris
			 **/
			if ( HTree[pos].left == -1 && HTree[pos].right == -1) {
				fwrite (&HTree[pos].value, 1, 1, f2);
				file_size--;
				checksum += HTree[pos].value;
				pos=ArchH->huffman_root_node;
			}
		}

		fclose(f2);

		/** Daca checksumul nu este corect se sterge fisierul **/
		if (checksum != F->checksum) {
			remove (F->file_name);
			fprintf(stderr, "Error: %s is CORRUPT\n", F->file_name);
		}
	}

	/** **/
	free(buffer);
	free(ArchH);
	free(HTree);
	free(F);
	fclose(f1);
	return 0;
}

