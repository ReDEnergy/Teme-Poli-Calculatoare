#define DIM 33

using namespace std;


typedef struct {
	int tokens;			/** Number of tokens **/
	int player;			/** Empty : 0
							Me	  : 1
							Enemy : 2   **/
} TableGame;

typedef struct {
	int player;			//	idiul playerului
	int dice[2];		//	zarurile
	int canMoveOut;		//	daca playerul poate scoate piese de pe tabla
	int bar;			//	indicele bar al playerului   0 sau 25
	int moved;			//	numarul de mutari executate
	int moves[8];		//	mutarile
} Stats;

typedef struct {
	int tableValue;		//	valoarea tablei la un moment
	int nr;				//	numarul de mutari pentru a ajunge la aceasta configuratie
	int moves[8];		//	mutarile pentru a genera tabela
	TableGame Table[DIM];	//	configuratia tablei de joc
} Move;


FILE *out;

TableGame Table[DIM];	// Tabla principala de joc
Stats Player[3];		// Informatiile despre jucatori
Move newMoves;			// Se retine cea mai buna mutare
int CValue[3][DIM];		// Valoarea fiecari coloane din tabla necesara pentru c
						// calculul valorii tablei de joc

int color;				// Culoarea playerului
int turn = 0;			// Tura de joc

/**	Functii de constructie mutari pentru zar
 **/

int isValidMove(TableGame *X, int source, int steps, int player);
int evalTable (TableGame *X, Stats PlayerX);
void compareMoves(TableGame *X, Stats PlayerX);
void canMoveOut(TableGame *X);
void get2ValidMoves(Stats PlayerX, TableGame *X, int dice);
void get4ValidMoves (Stats Player, TableGame *Table);

/**	Functii **/

void getMoves();
void resetMoves();
void saveMoves();

/**	Update Tabela
 **/

void makeMove(TableGame *X, int source, int steps, int player);
void updateTable(int nr);
void setStartTable ();
void setTableGame(int nr, int tokens, int player);
int translateMove(int col);

/**	Functii de debugging si printare de tabela
 **/

void printMoves();
void printTable(TableGame *X, FILE *out);
void printSendMessage();
void printReceiveMessage();
void isEndGame();

