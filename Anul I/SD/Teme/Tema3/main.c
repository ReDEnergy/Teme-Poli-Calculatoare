#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define dim 10000

/**	Pentru a apela mai usor tabelele cu keile respective am utilizat
 **	urmatoarele definuri
 **/
#define ORKEY	objroom[ K->ckey]
#define OKEY	object[ K->key1]
#define RKEY	room[ K->key2]

#include "hashtable.h"

int i;
FILE *f1, *f2;


/**		Hashtable object|room	**/

/**	Functia cauta un nod intr-o tabela si intoarce adresa acestuia sau NULL
 **	in cazul in care nu a fost gasita.
 **	Se primeste Primul termen al listei de la pozitia Hashtable[key]
 **/
HashX* Find_HashX_Node (HashX *First, uint key, char *name, int len ) {
	HashX *X=First;
	while (X) {
		if (X->key == key)
			if ( memcmp (X->name, name, len) == 0 )
				return X;
		X = X->next;
	}
	return NULL;
}

/**	Functia creaza un nou nod pentru tabelele de obiecte/room
 **/
HashX* New_HashX_Node (char *name, uint key) {
	HashX *X;
	X = malloc (sizeof(HashX));
	X -> key = key;
	X -> name = name;
	X -> next = NULL;
	X -> elem = NULL;
	return X;
}

/**	Functia adauga un nou nod in fata listei si intoarce adresa acestuia
 **/
HashX* Add_New_HashX_Node (HashX *First, char *name, uint key ) {
	HashX *X;
	X = New_HashX_Node( name, key);
	X->next = First;
	return X;
}

/**		Hashtable objroom		**/

/**	Functia creaza un nou nod pentru tabela de intersectie
 ** Se aloca memorie si se salveaza toate informatiile primite ca paramentru
 **/
HashC* New_HashC_Node (InfoX *K) {
	HashC *X;
	X = malloc (sizeof(HashC));
	X -> nr = malloc(sizeof(int));
	X -> obj = malloc(K->len1 + 1 );
	X -> room = malloc(K->len2 + 1 );

	memcpy(X->obj,K->obj,K->len1+1);
	memcpy(X->room,K->room,K->len2+1);

	X -> key = K->ckey;
	X -> nextObj= NULL;
	X -> nextRoom = NULL;
	X -> valid = K->type;
	*X -> nr = K->nr;

	return X;
}

/**	Functia inctoarcea numarul curent de obiecte dintr-o camera aplicand tipul
 ** de operatie aritmetica in functie de updateul primit
 **/
int Nuber_Of_Obj (int Suma, int nr, char type, char *valid) {
	if (type == '=') {
		*valid = '=';
		return nr;
	}
	if (type == '-')	return Suma - nr ;
	return Suma + nr;
}

/**	Functia cauta un nod in tabela de intersectie si actualizeaza numarul de
 ** obiecte in cazul in care gaseste bucketul respectiv.
 **	Intoarce NULL daca nu se gaseste nimic.
 **	Se primeste Primul termen al listei de la pozitia Hashtable[key]
 **/
HashC* Update_HashC_Node (HashC *First, InfoX *K) {
	HashC *X=First;
	while (X) {
		if (X->key == K->ckey)
			if ( memcmp (X->obj, K->obj, K->len1) == 0 )
				if ( memcmp (X->room, K->room, K->len2 ) == 0 ) {
					*X->nr = Nuber_Of_Obj (*X->nr , K->nr , K->type, &X->valid);
					return X;
					}
		X = X->next;
	}
return NULL;
}

/**	Functia adauga un nou nod in fata listei si intoarce adresa acestuia
 **/
HashC* Add_New_HashC_Node (HashC *First, InfoX *K) {
	HashC *X;
	X = New_HashC_Node(K);
	X -> next = First;
	return X;
}

/**			Main Function		**/
int main (int argc , char **argv) {

/**			Open files 			**/
	if (argc) Open_Files(argv);

/**			Variables			**/
	int n;
	char line[200];
	InfoX *K=malloc(sizeof(InfoX));

/**		Hashtables declaration	**/
	HashC **objroom = (HashC**) calloc (dim, sizeof(HashC));
	HashX **object = (HashX**) calloc (dim, sizeof(HashX));
	HashX **room = (HashX**) calloc (dim, sizeof(HashX));
	HashC *Find, *New;
	HashX *F;

/**		Read the nr of lines	**/
	fscanf(f1, "%d", &n);
	fgets(line,2,f1);

	for (i=0; i<n; i++) {
		fgets(line,200,f1);
		K->type = line[0];

		if (K->type < 62 && K->type > 42 )
		{
			sscanf(line,"%c %d %s %s", &K->type, &K->nr, K->obj, K->room);

			Gen_InfoX (K);
			Find = Update_HashC_Node (ORKEY, K);
			if (Find) continue;

			ORKEY = Add_New_HashC_Node(ORKEY, K);
			New = ORKEY;

			/**	Update Hashtable object **/
			F = Find_HashX_Node(OKEY, K->key1, New->obj, K->len1);
			if (F) {
				New -> nextRoom = F -> elem;
				F -> elem = New;
			}

			else {
				OKEY = Add_New_HashX_Node(OKEY, New->obj, K->key1);
				F = OKEY;
				F -> elem = New;
				F -> elem -> nextRoom = New->nextRoom;
			}

			/**	Daca se gaseste bucketul cautat se actualizeaza pointerii de
			 ** legatura.
			 **	Daca nu se gaseste nimic se adauga o noua intrara in tabela
			 **	respectiva si actualizeaza pointerii
			 **/

			/**	Update Hashtable room **/
			F = Find_HashX_Node(RKEY, K->key2, New->room, K->len2);
			if (F) {
				New -> nextObj = F -> elem;
				F -> elem = New;
			}
			else {
				RKEY = Add_New_HashX_Node(RKEY, New->room, K->key2);
				F = RKEY;
				F -> elem = New;
				F -> elem -> nextObj = New->nextObj;
			}
		continue;
		}
		/**	Query ZONE **/

		/** Se verifica tipul de query in functie de valoarea ASCII type **/
		if (K->type == 35) {
			sscanf(line,"%c %s %s", &K->type, K->obj, K->room);
			Gen_InfoX (K);
			Compute_Query_35( ORKEY, K);
			continue;
		}
		if (K->type == 63) {
			sscanf(line,"%c %s", &K->type, K->room);
			K->key2 = Gen_Hash_Key( K->room, &K->len2);
			Compute_Query_63 (RKEY, K);
			continue;
		}
		if (K->type == 64) {
			sscanf(line,"%c %s", &K->type, K->obj);
			K->key1 = Gen_Hash_Key( K->obj, &K->len1);
			Compute_Query_64 (OKEY, K);
			continue;
		}
	}

/**	Eliberari de memorie **/
	free (K);
	Free_All (object, room, objroom);
	fclose(f1);
	fclose(f2);

return 0;
}

/**	Open File and Free Function	**/

void Open_Files (char **argv) {
	int len = strlen(argv[1]);
	char *file = malloc(len+2);

	strcpy(file,argv[1]);
	file[len-2]='o';	file[len-1]='u';
	file[len]='t';		file[len+1]='\0';

	f1 = fopen( argv[1],"r");
	f2 = fopen(file,"w");
	free(file);
}

void Free_HashX (HashX *First) {
	HashX *X=First;
	while(X) {
		First = First->next;
		free(X);
		X=First;
	}
}

void Free_HashC (HashC *First) {
	HashC *X=First;
	while(X) {
		First = First -> next;
		free(X->nr);
		free(X->obj);
		free(X->room);
		free(X);
		X=First;
	}
}

void Free_All (HashX **object ,HashX **room, HashC **objroom ) {

	for(i=0;i<dim;i++)
		if(object[i]) Free_HashX (object[i]);
	free (object);

	for(i=0;i<dim;i++)
		if (room[i]) Free_HashX (room[i]);
	free (room);

	for(i=0;i<dim;i++)
		if (objroom[i]) Free_HashC (objroom[i]);
	free (objroom);
}

/**	Query: # obj room - ASCII 35 **/

int* Get_Number (HashC *First, InfoX *K) {
	HashC *X=First;
	while (X) {
		if (X->key == K->ckey)
			if ( memcmp (X->obj, K->obj, K->len1) == 0 )
				if ( memcmp (X->room, K->room, K->len2 ) == 0 ) {
					if (X->valid == '=' ) return X->nr;
					return NULL;
					}
		X = X->next;
	}
return NULL;
}

void Compute_Query_35 (HashC *X, InfoX *K) {
	int *rez ;
	rez = Get_Number( X ,K );

	if (rez)	fprintf(f2,"%d\n",*rez);
	else		fprintf (f2,"nu se stie\n");

}

/**	Query: ? room - ASCII 36	**/

void Print_Rooms (HashC *First) {
	HashC *X = First;
	int ok=1;
	while (X) {
		if (*X->nr) {
			if (!ok) fprintf(f2,",");
			fprintf(f2,"%s", X->room);
			ok = 0;
		}
		X = X->nextRoom;
	}
	if (ok) fprintf(f2,"nicaieri");
	fprintf(f2,"\n");
}

void Compute_Query_64 (HashX *X, InfoX *K) {
	HashX *F;
	F = Find_HashX_Node ( X, K->key1, K->obj, K->len1);
	if (F)	Print_Rooms(F->elem);
	else	fprintf (f2,"nu se stie\n");
}

/**	Query: @ object - ASCII 36	**/

void Print_Obj (HashC *First) {
	HashC *X = First;
	int ok=1;
	while (X) {
		if (*X->nr) {
			if (!ok) fprintf(f2,",");
			fprintf(f2,"%s", X->obj);
			ok = 0;
		}
		X = X->nextObj;
	}
	if (ok) fprintf(f2,"nimic");
	fprintf(f2,"\n");
}

void Compute_Query_63 (HashX *X, InfoX *K) {
	HashX *F;
	F = Find_HashX_Node ( X, K->key2, K->room, K->len2);
	if (F)	Print_Obj(F->elem);
	else	fprintf (f2,"nu se stie\n");
}

/**			Hash Generator		**/

/** Hasul calculeaza suma caracterelor unui element in ASCII
 **/
uint Gen_Hash_Key(char *word, int *len) {
	int i = 0, hash = 0;

	while(word[i]) {
		hash = hash + word[i];
		i++;
	}
	*len = i;
	return hash;
}

/**	Generarea tuturor informatiilor necesare pentru a trimiterea ca parametri
 **/
void Gen_InfoX (InfoX *K) {
	K->key1 = Gen_Hash_Key( K->obj, &K->len1);
	K->key2 = Gen_Hash_Key( K->room, &K->len2);
	K->ckey = K->key1+ K->key2;
}



