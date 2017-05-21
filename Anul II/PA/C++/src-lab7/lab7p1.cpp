#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

enum Color {
  WHITE,
  GRAY,
  BLACK
};

struct Node {
  unsigned int color;
  unsigned int component;
  unsigned int instack;
  /* TODO: Adaugat aici tot ce vi se pare util. */
};

using namespace std;

/* Matricea de adiacenta globala. */
namespace graph {

/* In C++, variabilele dintr-un namespace se pot accesa prefixand numele
 * namespace-ului numelui variabilelelor (asta ca sa nu fie globale si sa intre
 * in conflict cu alte variabile exportate). De exemplu:
 *
 * graph::size
 */
std::vector<std::vector<bool> > edge;
unsigned int size;
std::vector<Node> nodes;

};


int c[100];


/* Functie care face toate nodurile albe. */
void clearColor()
{
  for (unsigned int i = 0; i < graph::size; ++i) {
    graph::nodes[i].color = WHITE;
  }
}

void dfs(unsigned int node, int a
         /* TODO: Adaugati orice parametri vi se par utili/relevanti. */)
{
  /* TODO: Implementati un DFS (obs: Puteti implementa unul generic pe care sa
   * il refolositi la toate subpunctele. Va trebui insa sa dati parametri
   * speciali care sa tine cont daca se face DFS pe graful
   * neorientat/transpus) */
	unsigned int i;
	graph::nodes[node].color = BLACK;

	for (i=0; i<graph::nodes.size(); i++)
		if (graph::edge[node][i]==true || graph::edge[node][i]==true)
			if (graph::nodes[i].color == WHITE )
			{
				c[i] = a;
				dfs(i, a);
			}
}


void paintComponents(std::vector<std::vector<unsigned int> >& components)
{
  /* TODO 1: Implementati aici detectarea componentelor din graful reunit cu graful
   * transpus (altfel spus, graful daca ar fi fost neorientat).
   *
   * Trebuie ca in vectorul de components sa puneti cate un vector pentru
   * fiecare componenta conexa detectata, acel vector continand ID-urile/home/red/Desktop/Dropbox/ReDEnergy/PA/C++/src-lab7
   * nodurilor din componenta. */

	unsigned int i;

	for (i=0; i<graph::nodes.size(); i++ )
		if (graph::nodes[i].color == WHITE)
			dfs(i, i);

}

int index, idx[20], lowlink[20], x;
stack <int> S;


void tarjan(int node, int level) {
	unsigned int i;
	idx[node] = level;
	lowlink[node] = level;
	S.push(node);
	graph::nodes[node].instack = 1;
	for (i=0; i<graph::nodes.size(); i++ ) {
		if (graph::edge[node][i] == true) {
		{
			if (idx[i] == -1) {
				tarjan(i, level + 1);
				lowlink[node] = min (lowlink[node], lowlink[i]);
			}
			else
				if (graph::nodes[node].instack == 1)
					lowlink[node] = min (lowlink[node], lowlink[i]);
		}
		}
	}

	if (lowlink[node] == idx[node]) {
		cout<< "Componenta tare conexa:";
		do
		{
			x =  S.top();
			S.pop();
			graph::nodes[x].instack = 0;
			cout<<x<<" ";
		} while (x == node);
		cout<<endl;
	}
}


void dfs_tarjan() {
	unsigned int i;
	S.empty();

	for (i=0; i<graph::nodes.size(); i++ ) {
		idx[i] = -1;
		graph::nodes[i].instack = 0;
		lowlink[i] = 10000;
	}

	for (i=0; i<graph::nodes.size(); i++ )
		if (idx[i] == -1)
			tarjan(i, 0);
}


/*

ctc_tarjan(G = (V, E))
        index = 0
        S = stiva vida
        pentru fiecare v din V
                daca (idx[v] nu e definit) // nu a fost vizitat
                        tarjan(G, v)

puncte_articulatie(G = (V, E))
        timp = 0
        pentru fiecare v din V
        daca (d[v] nu e definit)
                dfsCV(G, v)

*/
int t;

void dfsCV(int node, int level) {
	unsigned int i;
	idx[node] = level;
	level++;
	lowlink[node] = level;
	for (i=0; i<graph::nodes.size(); i++ ) {
		if (graph::edge[node][i] == true)
		{
			if (idx[i] == -1)
			{

			}
		}
	}
}


void articulatii() {
	unsigned int i;
	for (i=0; i<graph::nodes.size(); i++ ) {
		idx[i] = -1;
	}

	for (i=0; i<graph::nodes.size(); i++ )
		if (idx[i] == -1)
			dfsCV(i, 0);

}



/** Functie care incarca un graf dintr-un fisier daca se da numele fisierului.
 * Atentie, fisierul cu graful trebuie sa aiba sintaxa corecta. */
bool loadGraph(const char* fileName)
{
  std::ifstream in(fileName);
  if (!in.good()) {
    std::cout << "Error! Could not open graph file '" << fileName << "'."
              << std::endl;
    return false;
  }

  in >> graph::size;
  graph::edge = std::vector<std::vector<bool> >(
      graph::size, std::vector<bool>(graph::size, false));
  graph::nodes = std::vector<Node>(graph::size, Node());
  unsigned int edgeCount;
  in >> edgeCount;
  for (unsigned int i = 0; i < edgeCount; ++i) {
    unsigned int a, b;
    in >> a >> b;
    graph::edge[a][b] = true;
  }

  if (!in.good()) {
    std::cout << "Error. Bad graph file syntax." << std::endl;
    return false;
  }

  in.close();
  return true;
}

int main()
{
  /* Deschide fisierul de intrare si citeste graful. */
  if (!loadGraph("Graph.txt")) {
    return 0;
  }

	clearColor();

  /* Calculam si afisam componentele conexe. */
  std::vector<std::vector<unsigned int> > components;
  paintComponents(components);

  std::cout << std::endl << "Componentele conexe ale grafului neorientat sunt: "
      << std::endl;
  for (unsigned int i = 0; i < components.size(); ++i) {
    std::cout << "Componenta " << i << ":";
    for (unsigned int j = 0; j < components[i].size(); ++j) {
      std::cout << " " << components[i][j];
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

	unsigned int i;

	for (i=0; i<graph::nodes.size(); i++ )
		cout<<c[i]<<" ";




  /* TODO 2: Calculati si afisati componentele tare conexe. */


	clearColor();
	dfs_tarjan();


  /* TODO 3: Calculati si afisati nodurile critice. */

  /* TODO 4: Calculati si afisati puntile. */

  /* TODO 5: Calculati si afisati o strategie de pavare. */

  return 0;
}
