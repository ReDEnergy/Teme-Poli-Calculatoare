using namespace std;

#define DICTDIM 8001
#define DIM 67


typedef struct
{
	vector <string> words;
	unsigned int dist;
	unsigned int freq;
} Solution;

typedef struct {
	unsigned int freq;
	char word[20];
} Word;


FILE *out;
ofstream fout;

Word Dict[DICTDIM];
string phrase;
Solution A[DIM][DIM];
int *Dlev[DIM][DICTDIM];


void PrintDistanceTable(int);
void ShowSolution (Solution);
void PrintDictionary();
