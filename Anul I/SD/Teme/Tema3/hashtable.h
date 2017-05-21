#ifndef Hash_Table
#define Hash_Table

typedef unsigned int uint;

typedef struct Hash_X {
	uint key;
	char *name;
	struct Hash_X *next;
	struct Hash_C *elem;
} HashX;

/**
HashX reprezinta bucketul unui hashtable object / room
Pentru a rezolva coliziunile am implementat liste.
Se retine un nume si o keye pentru a folosi cautarea eficienta a datelor.
Informatiile despre elementele retinute : HashC *elem.
 **/

typedef struct Hash_C {
	int *nr;
	uint key;
	char *obj;
	char *room;
	char valid;
	struct Hash_C *nextObj;
	struct Hash_C *nextRoom;
	struct Hash_C *next;
} HashC;

/**
HashC reprezinta bucketul pentru hashtable de intersectii object+room
De asemenea HashC reprezinta si un element al listei de obiecte/camere din
hashtable object sau room.

Se retin toate informatiile necesare: nume obiect, nume camera , numar de
obiecte din camera, validitatea informatiilor , si keya bucketului.

Pentru lista de obiecte din camera se folosesc pointerii:  *nextObj
Pentru lista de camere in care se gaseste un obiecte:	*nextRoom
 **/

typedef struct Read_Info {
	int len1;
	int len2;
	char obj[65];
	char room[65];
	uint key1;
	uint key2;
	uint ckey;
	char type;
	int nr;
} InfoX;

/**
Pentru usurinta in trimiterea datelor ca parametru am utilizat o structura in
care retin toate informatiile citite si necesare prelucrarii datelor.
**/

uint Gen_Hash_Key(char *word, int *len);

void Gen_InfoX (InfoX *K);

void Compute_Query_35 (HashC *X, InfoX *K);

void Compute_Query_63 (HashX *X, InfoX *K);

void Compute_Query_64 (HashX *X, InfoX *K);

void Open_Files (char **argv);

void Free_All (HashX **object ,HashX **room, HashC **objroom );

#endif

