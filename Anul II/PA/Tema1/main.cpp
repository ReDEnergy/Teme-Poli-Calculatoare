#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <string.h>

using namespace std;

#include "main.h"

/**	Functia pentru a citi dictionarul de cuvinte
 ** Dictionarul de cuvinte este reinut intrun vector de Word
 **/
void ReadDictionary ()
{
	FILE *dict_input = fopen("dict.txt", "r");
	int i=0;
	while ( fscanf(dict_input, "%s %u", Dict[i].word, &Dict[i].freq) > 0)
		i++;
}

/**	Functie pentru a calcula distanta de editare dintre string A si un string B
 ** Functia intoarce un vector de int unde distanta de pe pozitia i reprezinta
 ** dist de editare dintre A si substring de B de la pozitia i , la strlen B
 **/
int* Levenshtein(string A, string B)
{
	unsigned int i, j, maxim, line = 0;
	unsigned int lenA = A.length();
	unsigned int lenB = B.length();
	int *dist[2];

	dist[0] = (int*) calloc(DIM, sizeof(int));
	dist[1] = (int*) calloc(DIM, sizeof(int));

	for (i=0; i<=lenB; i++) {
		dist[0][i]=i;
	}

	maxim = lenB;
	if (lenA < lenB)
		maxim = 2 * lenA + 1;

	for (i=1; i<=lenA; i++)
	{
		line = line^1;
		dist[line][0]=i;
		for (j=1; j<=maxim; j++)
		{
			if (A[i-1] == B[j-1])
				dist[line][j] = dist[1-line][j-1];
			else
				dist[line][j] = min(min(dist[1-line][j], dist[1-line][j-1]),
								    min(dist[line][j-1], dist[1-line][j-1]))+1;
		}

		for (; j<=lenB; j++)
			dist[line][j] = DIM;
	}
	return dist[line];
}

/**	Functia calculeaza distanta de editare a tuturor cuvintelor din dictionar
 ** cu fiecare alegere de substring din phrase (pozitia i -> strlen phrase)
 **/
void DistanceTable()
{
	int i, j;
	int size = phrase.size();
	for (i=0; i<=size;i++)
	{
		string X = phrase.substr(i, size-i);
		for (j=0; j<DICTDIM-1; j++)
			Dlev[i][j] = Levenshtein (Dict[j].word, X);
	}
}

/**	Combina informatiile pentru 2 solutii si intoarce o singura solutie
 **/
void CombineSolutions (Solution A, Solution B, Solution &R)
{
	unsigned int i;
	R.dist = A.dist + B.dist;
	R.freq = A.freq + B.freq;
	R.words = A.words;
	for (i=0; i<B.words.size(); i++)
		R.words.push_back(B.words[i]);
}

/** Transforma un Vector de stringuri intru-un string cu spatii intre cuvinte
 ** necesar pentru compararea lexicografica
 **/
void stringTochar(vector <string> X, string &M)
{
	unsigned int i;
	for (i=0; i<X.size(); i++)
	{
		M.append(X[i]);
		M.append(" ");
	}
}

/**	Alege Solutia ce mai buna comparand intre A[i][j] cu A[i][k] + A[k][j]
 **/
void ChooseBest(int i, int j, int k)
{
	Solution R;
	CombineSolutions(A[i][k], A[k][j], R);

	if (A[i][j].dist > R.dist)
	{
		A[i][j] = R;
		return;
	}
	else
	if (A[i][j].dist == R.dist)
	{
		if (A[i][j].words.size() > R.words.size())
		{
			A[i][j] = R;
			return;
		}
		else
		if (A[i][j].words.size() == R.words.size())
		{
			if (A[i][j].freq < R.freq)
			{
				A[i][j] = R;
				return;
			}

			if (A[i][j].freq == R.freq)
			{
				string M, N;
				stringTochar(A[i][j].words, M);
				stringTochar(R.words, N);
				if (N.compare(M) < 0)
					A[i][j] = R;
				return;
			}
		}
	}
}

/**	Functia cea mai buna solutie de corectare pentru un covant pe baza
 ** dictionarului si a distantei de editare acesata din Dist[];
 **/
void CorrectWord(unsigned int i, unsigned int j)
{
	unsigned int dist, k = 0;
	string X = phrase.substr(i, j-i);

	A[i][j].dist = DIM;
	for (k=0; k<DICTDIM-1; k++)
	{
		dist = Dlev[i][k][j-i];
		if ( (dist == A[i][j].dist && Dict[k].freq > A[i][j].freq)
			|| dist < A[i][j].dist)
		{
			A[i][j].dist = dist;
			A[i][j].words.clear();
			A[i][j].words.push_back(Dict[k].word);
			A[i][j].freq = Dict[k].freq;
		}
	}
}

/**	Functie recursiva ce imparte o fraza in toate substringurile posibile
 ** si reconstruieste dinamic pe baza calculelor precedente ce-a mai buna
 ** solutie la un moment dat - reprezentata de A[i][j] - substringurl dintre
 ** pozitiile i si j din fraza PHRASE
 **/
void CorectPhrase(int i, int j)
{
	int k;
	if (A[i][j].freq == 0) {
		CorrectWord (i, j);
		for (k=i+1; k<j; k++)
		{
			CorectPhrase(i, k);
			CorectPhrase(k, j);
			ChooseBest(i,j,k);
		}
	}
}


/**	Functia intoarce stringul X din care se sterg toate aparitiile de spatii
 **/
void ReplaceSpace (string X) {
	string::iterator it = X.begin();
	int pos;
	while ((pos = X.find(' ')) > 0)
		X.erase(it+pos);
	phrase =  X;
}


int main () {
//	fout.open ("out.txt");

	string line;
//	freopen ("read.txt", "r", stdin);

	ReadDictionary();

	getline (cin, line, '\n');

	ReplaceSpace(line);
	DistanceTable();

	CorectPhrase(0, phrase.length());
	ShowSolution(A[0][phrase.length()]);

//	fout.close();
	return 0;
}

/**	Functia printeaza la stdout solutia de corectare a frazei de la input
 **/
void ShowSolution (Solution X)
{
	unsigned int i;
	for (i=0; i<X.words.size(); i++) {
		cout<<X.words[i];
		if (i<X.words.size() - 1)
			cout<<" ";
	}
	cout<<endl;
}

/**	Debug printare
 **/
void PrintSolution ( Solution X)
{
	unsigned int i;
	for (i=0; i<X.words.size(); i++)
		fout<<X.words[i]<<" ";
	fout<<endl;
	fout<<"Dist:"<<X.dist<<endl;
	fout<<"Freq:"<<X.freq<<endl<<endl;
}

/**	Debug printare
 **/
void PrintDictionary ()
{
	int i=0;
	for (i=0; i<DICTDIM-1; i++)
		fout<<Dict[i].word<<" "<<Dict[i].freq<<endl;
}
