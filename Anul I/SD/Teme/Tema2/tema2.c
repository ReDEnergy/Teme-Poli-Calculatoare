#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEVEL 8
#define P 0.5
FILE *f1,*f2,*f3;
int NR_DEF = 0;

/*	Structura reprezinta un nod al unui SkipList
 *	Se retine valoarea = keya de cautare
 *	bis = numarul de repetitii al unui cuvant in definitie
 *	freq =  frecventa de aparitie a cuvantului : 0 < freq < 1000
 *	**next pointerii catre urmatoarele noduri pentru fiecare nivel in parte
 */
typedef struct sn{
	char *value;
	int bis;
	int freq;
	struct sn **next;
} SkipNode;

/*	Structura reprezinta un SkipList asociat unui cuvant
 *	Level = levelul SkipList
 *	head - pointerul spre nodulul 0 (head)
 *	nr = numarul de cuvinte retinute in SkipList
 */
typedef struct {
	int level;
	int nr;
	SkipNode *head;
} SkipList;

/*	Structura pentru memorarea rezultatelor si ordonarea acestora
 *	cuvant =  cuvant definit
 *	freq = frecventa totala a aparitiilor elementelor cautate
 */
typedef struct {
	char *cuvant;
	int freq;
} Result;

/* ============================================================================
 * ======================== Functii pentru free ===============================
 * ============================================================================
 */

void free_char(char ***a,int n) {
	int i;
	for (i=0; i<=n; i++)
		free((*a)[i]);
	free((*a));
}

void free_TOP ( Result ** TOP , int n) {
	int i;
	for (i=0; i<n; i++) {
		free(TOP[i]->cuvant);
		free(TOP[i]);
	}
	free(TOP);
}

void free_SkipList ( SkipList ** SL , int n) {
	int i;
	for (i=0; i<n; i++) {
		free(SL[i]);
	}
	free(SL);
}

/* ============================================================================
 * =======================Functii pentru Skiplist==============================
 * ============================================================================
 */

/*	Functia intoarce o valoare random de tip float intre 0 si 1
 *	Necesara pentru a calcula random level
 */
float rand48() {
    return (float) rand() / RAND_MAX;
}

/*	Functia returneaza un level radom dupa probabilitatea P
 *	Astfel 1/2 din noduri au nivel 2
 *	1/4 level 3, 1/8 level 4 , ..
 */
int random_level() {
    static int first = 1;
    int lvl = 0;

    if(first) {
		srand( (unsigned)time( NULL ) );
        first = 0;
    }

    while(rand48() < P && lvl < MAX_LEVEL)
	lvl++;

    return lvl;
}

/*	Functia creaza un nou nod pentru SkipList
 *	Se adauga valorile initiale in acest nod
 */
SkipNode* create_node ( int level, char *value ) {
	int n=strlen(value);
	SkipNode *x;
	x=(SkipNode*) malloc (sizeof(SkipNode));
	x->next = (SkipNode**) calloc (level+1, sizeof(SkipNode*));
	x->value = (char*) malloc (n+1);
	x->bis = 1;
	x->freq = 0;
	strcpy(x->value,value);
	return x;
}

/*	Fucntia creaza un nou Skiplist pe care il returneaza
 *	Se initializeaza valoriabilele
 *	Se creaza un nod header
 */
SkipList* create_SkipList (char *cuvDef) {
	SkipList *x;
	x=(SkipList*) malloc (sizeof(SkipList));
	x->level = 0;
	x->nr = 0;
	x->head = create_node( MAX_LEVEL ,cuvDef);
	return x;
}

/*	Fucntia adauga un nou nod la SkipList in cazul in care acesta nu exista
 *	Daca key-a se gaseste in Skiplist se incrementeaza numarul apartiilor
 *	La fiecare apelare numaul cuvintelor din Skiplist se incrementeaza nr++;
 */
void add_node (SkipList *SL, int level, char *value) {
	int i,find=1;
	SL->nr ++;
	SkipNode *x = SL->head;
	SkipNode *update [MAX_LEVEL + 1];

	/* Initializare vector NULL
	 */
	memset (update, 0, MAX_LEVEL + 1);

	/*	Se cauta Keya in SkipList sau valoarea cea mai apropiata a acestia
	 *	In acelasi timp se actualizeaza vectorul de legaturi necesar pentru
	 *	a insera un nou nod
	 */
	for ( i=SL->level; i>=0; i--) {
		while (x->next[i] && strcmp (x->next[i]->value, value) < 0 )
			x=x->next[i];
		update[i]=x;
	}

	/*	Intrucat verificarea se face mereu la nodul urmator se trece la
	 *	acesta pentru a insera dupa acesta ( alfabetic)
	 */
	x=x->next[0];

	/*	Se testeaza daca nodul curent este cel cautat
	 *	Se incrementeaza numarul de aparitii sau se trece la adaugarea nodului
	 */
	if (x)	find=strcmp(x->value, value);

	if (!find)	x->bis++;
	else {
		/*	Daca levelul noului nod este mai mare decat cel al SkipListului
		 *	se actualizeaza vectorul de legaturi
		 */
		if (level>SL->level) {
			for (i=SL->level+1; i<=level; i++)
				update[i]=SL->head;
			SL->level=level;
		}

		/*	Adaugare nod la Skiplist
		*/
		SkipNode *y;
		y=create_node(level, value);

		/*	Refacerea legaturilor
		 */
		for (i=0; i<=level; i++) {
			y->next[i] = update[i]->next[i];
			update[i]->next[i] = y;
		}
	}
}

/*	Functia parcurge un Skiplist si calculeaza frecv de aparitie a termenilor
 *	Pentru calculul mai rapid se retine informatia intr-un int
 *	Numarul de zecimale importante este 3 : *1000
 */
void set_freq (SkipList *SL) {
	SkipNode *x = SL->head->next[0];
	while (x) {
		x->freq = (int)((float)x->bis * 1000 /SL->nr);
		x=x->next[0];
	}
}

/*	Functia verifica corectitudinea unui termen al definitiei
 *	Return :	NULL in caz de invaliditate
 *				term in caz de validitate
 */
char* verify_term (char *term) {
	int i=0;
	while (term[i]) {
		if ( !isalpha(term[i]) ) return NULL;
		else term[i] = tolower(term[i]);
		i++;
	}
	if (i>=3) return term;
	return NULL;
}

/*	Fucntia genereaza termenii definitiei pentru un SkipList
 *	Se parseaza linia citita din fisier si se insereaza ficare cuvant valid
 *	Se intoarce SkipListul SL completat prin referinta
 */
void new_Deff (SkipList ***SL, char *line) {
	int level;
	char *Deff,*term;
	Deff = strtok(line,":");
	Deff[0]=tolower(Deff[0]);

	/*	Alocare memorie pentru o nouna Definitie a unui cuvant
	 */
	(*SL) = (SkipList**) realloc ( (*SL), (NR_DEF) * sizeof(SkipList*));
	(*SL) [NR_DEF-1] = create_SkipList (Deff);

	/*	Adaugarea termenilor de definitie
	 *	Se verifica validitatea acestora
	 */
	while ( (term = strtok(NULL," ;,.()-\n")) ) {
		term = verify_term(term);
		if (!term) continue;
		level = random_level();
		add_node( (*SL)[NR_DEF-1], level, term);
	}

	free(term);
}

/*	Fucntia deschide fisierul *file
 *	La fiecare rand se apeleaza new_Deff pentru a crea un SkipList
 *	cu o noua definitie
 *	Pentru fiecare SkipList se calculeaza frecventa de aparitie a termenilor
 *	Functia intoarce prin referinta SkipListul creat
 */
void read_def_from (SkipList ***SL, char *file) {
	char *line;
	line=(char*) malloc (8000 * sizeof(char));

	f3=fopen(file,"r");

	while (fgets(line,8000,f3)) {
		NR_DEF++;
		new_Deff(SL,line);
		set_freq((*SL)[NR_DEF-1]);
	}

	fclose(f3);
}

/* ============================================================================
 * ===================Functii pentru Cautare Ghicitori=========================
 * ============================================================================
 */

/*	Functia cauta in SkipList o keye si intoarce frecventa de aparitie
 *	Return 0 in cazul in care nu se gaseste valoarea cautata
 */
int search_nod (SkipList *SL, char *value) {
	int i;
	SkipNode *x = SL->head;

	for ( i=SL->level; i>=0; i--) {
		while (x->next[i] && strcmp ( x->next[i]->value, value) < 0 )
			x=x->next[i];
	}
	x=x->next[0];

	if(x) {
		int a = strcmp (x->value, value);
		if (a == 0) return (x->freq);
	}
	return 0;
}

/*	Functia scaneaza un SkipList si verifica daca toate cuvintele ghicitorii
 *	se regasesc in aceasta
 *	Return : 	0 in cazul in care un cuvant din ghicitoare lipseste
 *				frecventa totala a cuvintelor din ghicitoare
 */
int search_puzzle (SkipList *SL, char **words, int nr) {
	int i, p ,x=0;

	for (i=0; i<nr; i++) {
		p=search_nod(SL,words[i]);
		if(p==0) return 0;
		x = x + p;
	}
	return x;
}

/*	Fucntia construieste vectorul de cuvinte al Ghicitorii
 *	Verifica existenta Ghicitorii in fiecare SkipList
 *	Salveaza Definitia si frecventa totala in care se regaseste Ghicitoare
 *	Ordoneaza crescator dupa frecventa totala si alfabetic in caz de ==
 */
void build_puzzle (SkipList **SL, char *puzzle) {
	int i, nr=0, length, pondere;
	int NR=0;
	char **words = (char**) malloc (sizeof(char*));
	char *term = (char*) malloc (50);
	Result **TOP;
	TOP = (Result**) malloc (sizeof (Result*));

	/*	Salavea cuvintelor ghicitorii
	 */
	term = strtok(puzzle,"\n ");
	while (term) {
		nr++;
		length = strlen (term);
		words = (char**) realloc (words,nr * sizeof(char*));
		words[nr-1] = (char*) malloc (length+1);
		words[nr-1] = term;
		term = strtok(NULL,"\n ");
	}

	/*	Verificarea existentei si salvarea definitiilor valide
	 */
	for (i=0; i<NR_DEF; i++) {
		pondere = search_puzzle(SL[i], words, nr);
		if (pondere){
			NR++;
			TOP = (Result**) realloc (TOP, NR * sizeof (Result*));
			TOP[NR-1] = (Result*) malloc (sizeof(Result*));
			TOP[NR-1]->cuvant = malloc(50);
			TOP[NR-1]->cuvant = SL[i]->head->value;
			TOP[NR-1]->freq = pondere;
		}
	}

	/*	Sortarea rezultatului dupa frecventa sau alfabetic in caz de egalitate
	 *	Sorry pentru BUBBLE sort , nu stiu ce a avut qsort de nu vrea sa mearga
	 */
	int g=1;
	Result *aux;
	while (g) {
		g=0;
		for (i=0; i<NR-1 ;i++) {
			if( TOP[i]->freq < TOP[i+1]->freq) {
				aux = TOP[i];
				TOP[i]=TOP[i+1];
				TOP[i+1]=aux;
				g=1;
			}

			if (TOP[i]->freq == TOP[i+1]->freq) {
				int diff = strcmp(TOP[i]->cuvant , TOP[i+1]->cuvant);
				if (diff > 0) {
					aux = TOP[i];
					TOP[i] = TOP[i+1];
					TOP[i+1] = aux;
					g=1;
				}
			}
		}
	}

	/*	Afisarea rezultatelor
	 */
	for (i=0; i<NR;i++)
		fprintf(f2,"%s\n",TOP[i]->cuvant);

	if (NR == 0)
		fprintf(f2,"nici un rezultat\n");

	fprintf(f2,"\n");

	/*	Eliberari de memorie
	 */
//	free_char(&words,nr-1);
//	free_TOP(TOP,NR-1);
	free(term);
}

/*	Functia citeste pe rand cate o linie cu ghicitoare si apeleaza
 *	functia de parsare a liniei ghicitorii
 */
void read_puzzle (SkipList **SL) {
	int n,i;
	char puzzle[1000];

	fscanf(f1,"%d",&n);

	/*	Skip rand */
	fgets(puzzle, 10, f1);

	for (i=0; i<n; i++) {
		while ( fgets(puzzle, 1000, f1) )
			build_puzzle(SL, puzzle);
	}
}

/* ============================================================================
 * =================Functii de Printare si Debugging===========================
 * ============================================================================
 */

void print_Skiplist (SkipList *SL) {
	SkipNode *x = SL->head->next[0];

	fprintf(f2,"%s | %d cuvinte:\t{ ",SL->head->value, SL->nr);
	int numar = 0;
	while (x) {
		numar ++;
		fprintf(f2,"%d*%s:%d",x->bis, x->value, x->freq );
		if ( (numar%10) == 0) fprintf(f2,"\n");
		if (x->next[0]) fprintf (f2,"| ");
		x=x->next[0];
	}
	fprintf(f2," }\n");
}

void dimension (SkipList *SL) {
	int nr=0;
	SkipNode *x = SL->head->next[0];
	while (x) {
		nr++;
		x=x->next[0];
	}
	fprintf(f2,"Dim: %d \t",nr);
	fprintf(f2,"Level: %d\n",SL->level);
}

void print_Dictionary (SkipList **SL) {
	int i;
	for (i=0; i<NR_DEF; i++) {
		print_Skiplist (SL[i]);
//		dimension (SL[i]);
	}
}

/* ============================================================================
 * =============================Functia Main===================================
 * ============================================================================
 */

int main (int argc, char **argv)
{
	/*	Deschiderea fisierelor necesare
	 */
	f1=fopen (argv[argc-1],"r");
	f2=fopen ("out.txt","w");

	int i,n;
	char file[20];

	/*	Declararea Dictionarului de cuvinte SL
	 *	Alocare memorie pentru 1 cuvant
	 */
	SkipList **SL;
	SL = (SkipList**) malloc ( 1* sizeof(SkipList*));

	/*	Citire numar de fisiere
	 */
	fscanf(f1,"%d",&n);

	/*	Se citeste pe rand un fisier
	 *	Se apeleaza citirea definitiilor din aceste fisiere
	 */
	for (i=0;i<n;i++){
		fscanf (f1,"%s",file);
		read_def_from (&SL, file);
	}

	/*	Se citest si rezolva ghicitorile prin intermediul read_puzzle
	 */
	read_puzzle(SL);

	fclose(f1);
	fclose(f2);
	return 1;
}
