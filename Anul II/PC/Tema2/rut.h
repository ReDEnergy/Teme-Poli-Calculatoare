using namespace std;

/** Structura necesara pentru parsarea payloadului la primire de mesaje
 **/
typedef struct {
	int type;
	int pos;
	int A;
	int B;
	int cost;
	int nr;
} Event;

/**	Structura pentru LSA
 **/
typedef struct {
	int time;		// timp update LSA
	int cost[KIDS];	// vector de cost catre vecini
} LSA;

/**	Am redeclarat o serie de variabile din main - globale
 **/
int pipein;
int pipeout;
int timp;
int nod_id;
int NR_SECV = 0;
int tab_rutare [KIDS][2];

Event Ev;

LSA LSADatabase[10];		// LSADatabase
vector <msg> ReceiveMsg;	// Vector pentru retinerea mesajelor


/**	Debug Function - printing information
 **/

FILE *out;	//Fisier de debuging

void PrintLSA (int pos);
void InfoMesaj(int pos);
void PrintLSADatabase ();
void Print_Info_LSA (LSA x, int router);
void OpenDebuggingFile();














