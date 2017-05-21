#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <algorithm>

#include "GraphAdjMat.h"
#define MAX 100
#define INF 10000

using namespace std;

int P[MAX];
int d[MAX];




void transitive_closure(GraphAdjMat& graph)
{
  /* TODO: Update-ati graful folosind algoritmul Floyd-Warshall pentru a
   *  contine inchiderea sa tranzitiva.
   *
   * Pentru a accesa muchiile grafului, folositi get_edge(a, b), iar pentru
   * a adauga muchii in graf, folositi set_edge(a, b, cost).
   *
   * Mai multe informatii gasiti in documentatie. */

	int mat[MAX][MAX];
	int i, j, k, dist;
	int n = graph.get_n();

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			if (graph.get_edge(i,j) > 0 )
				mat[i][j] = graph.get_edge(i, j);
			else
				mat[i][j] = INF;
		}

	cout<<endl;

	for (k = 0; k < n; ++k)
		for (i = 0; i < n; ++i)
			for (j = 0; j < n; ++j)
			{
				dist = mat[i][k] + mat[k][j];
				if (dist < mat[i][j]) {
					mat[i][j] = dist;
					graph.set_edge(i,j,dist);
					graph.set_detour(i,j,k);
				}
			}
}

void print_routing_table(GraphAdjMat& graph)
{
  /* TODO: Din graful pe care ati aplicat un algoritm un algoritm care sa
   * calculeze drumul de lungime optima intre oricare doua noduri, precum si
   * urmatorul nod, afisati tabela de rutate pentru fiecare nod in parte in
   * formatul din enunt:
   *
   * DESTINATIA | NEXT HOP | TOTAL COST */

	for (int i = 0; i < graph.get_n(); ++i) {
		printf("\nSursa: %d \n", i);
		for (int j = 0; j < graph.get_n(); ++j)
		{
			if ( i != j )
			{
				cout << "Dest: " << j << " - Next hop: ";
				if (graph.get_edge(i,j) == 1)
					printf("%d - Cost : 1\n", j);
				else
					printf("%d - Cost : %d\n", graph.get_detour(i,j), graph.get_edge(i,j));
			}
		}
	}

}

void unweightedTasks() {
  /* Deschidem fisierul de intrare si citim numarul de noduri din graf. */
  std::ifstream in("src-lab8/GraphUnweighted.in");
  int n;
  in >> n;

  /* Cream un graf cu n noduri si apoi incarcam muchiile din fisier. */
  GraphAdjMat graph(n, true);
  in >> graph;
  in.close();

  /* Afisam graful initial. */
  std::cout << "Initial graph:" << std::endl << graph << std::endl;

  /* Efectuam inchiderea tranzitiva pe graf si apoi afisam rezultatul. */
  transitive_closure(graph);
  std::cout << "Transitive closure:" << std::endl << graph << std::endl;

  /* Pe graful cu inchiderea tranzitiva calculata, printam tabela de rutare. */
  std::cout << "Graph routing table:" << std::endl;
  print_routing_table(graph);
}

void dijkstra (GraphAdjMat& graph, int source, int dest)
{
  /* TODO: Calculati drumul optim de la sursa la destinatie.
   * ATENTIE! Trebuie ca de asemenea sa setati detour-urile corect pentru a
   * putea apoi sa va folositi de rezultate:
   *
   * Daca drumul (source,y) vine de la (source,x) U (x,y), atunci detour pentru
   * (source,y) TREBUIE sa fie x! */

  /* Cream si initailizam un vector de distante intre nodul sursa si toate
   * celelalte noduri. */


	int i, j, v[MAX];
	int n = graph.get_n();
	int min, poz, u;


	for ( i = 0; i < n; ++i )
		v[i] = 0;

	v[source] = 2;

	for ( i = 0; i < n; ++i )
		if ( graph.get_edge(source, i) != -1 )
		{
			d[i] = graph.get_edge(source, i);
	//		P[i] = source;
			v[i] = 1;
		}
		else
		{
			d[i] = INF;
	//		P[i] = -1;
		}

	int repeat = n - 1;

	while(repeat)
	{
		min = INF;
		for (i = 0; i < n; i++)
			if ( v[i]!=2 && d[i] < min)
			{
				min = d[i];
				poz = i;
			}
		u = poz;
		v[poz] = 2;
		repeat--;

		for (i = 0; i < n; ++i)
		{
			if (graph.get_edge(u,i) != -1 && v[i]!=2 && d[i] > d[u] + graph.get_edge(u, i) )
			{
				d[i] = d[u] + graph.get_edge(u,i);
		//		P[i] = u;
				graph.set_detour(source, i, u);
			}
		}
	}
}

GraphAdjMat::Path minimum_cycle(GraphAdjMat& graph, int u, int v)
{
  /* Stergem toate drumurile din graph, pentru ca nu mai sunt de actualitate.
   * (Dijkstra va recalcula drumurile). */
  graph.clear_paths();

  /* Stergem muchia directa dintre U si V. */
  int backup = graph.get_edge(u,v);
  graph.erase_edge(u, v);

  /* Rulam Dijkstra pentru a gasi cea mai buna alternativa intre U si V. */
  dijkstra(graph, v, u);
  GraphAdjMat::Path returnValue = graph.path(v, u);

  /* Adaugam muchia inapoi (atat in graf cat si in ciclu). */
  graph.set_edge(u, v, backup);
  if (returnValue.size()) {
    returnValue.push_back(std::pair< std::pair<int,int>, int>(
            std::pair<int, int>(u, v), backup));
  }

  /* Intoarcem ciclul de cost minim depistat intre U si V. */
  graph.clear_paths();
  return returnValue;
}

std::vector<GraphAdjMat::Path> get_min_cycles_cover(GraphAdjMat & graph)
{
  std::vector<GraphAdjMat::Path> returnValue;
  /* TODO: Folositi functia de ciclu minim pentru a obtine un set de cicluri de
   * cost minim care sa acopere muhciile grafului.
   * Puteti sa tineti cont in parcursul reBroken path! No edge from 0 to 6!zolvarii de urmatoarele hint-uri:
   *
   * a) Orice muchie din graf poate face parte din 0, 1 sau mai multe cicluri.
   * Dintre ciclurile din care face muchia parte, exista unul singur de cost
   * minim.
   *
   * b) Oricare ar fi doua cicluri diferite in graf, exista cel putin o muchie
   * in fiecare dintre ele care sa nu faca parte din celalalt ciclu.
   */

  /* Vom tine o matrice booleana din care sa reiasa daca o muchie a fost sau nu
   * folosita pana acum intr-un ciclu. */
  bool used[graph.get_n()][graph.get_n()];
  memset(used, 0x00, graph.get_n() * graph.get_n());
  graph.clear_paths();

	int n = graph.get_n();
	int i, j;

  /* TODO: rezolvarea aici. */
	for (i=0; i<n; i++)
		for (j=0; j<n; j++)
			if ( used[i][j] == false) {
				GraphAdjMat::Path x = minimum_cycle(graph, i, j);
			}




  return returnValue;
}

void weightedTasks() {
  /* Deschidem fisierul de intrare si citim numarul de noduri din graf. */
  std::ifstream in("src-lab8/GraphWeighted.in");
  int n;
  in >> n;

  /* Cream un graf cu n noduri si apoi incarcam muchiile din fisier. */
  GraphAdjMat graph(n);
  in >> graph;
  in.close();

  /* Afisam graful initial. */
  std::cout << "Initial graph:" << std::endl << graph << std::endl;

  /* Afisam la intamplare doua drumuri optime din graf. */
  if (graph.get_n() >= 7) {
    dijkstra(graph, 0, 6);
    std::cout << "Best path between 0 and 6 is: " << graph.path(0, 6)
        << std::endl;
    dijkstra(graph, 6, 3);
    std::cout << "Best path between 6 and 3 is: " << graph.path(6, 3)
        << std::endl;
  }

  /* Calculam un set de cicluri minime care sa acopere muchiile grafului. */
  std::vector<GraphAdjMat::Path> cover = get_min_cycles_cover(graph);
  std::cout << "A set of minimum cycles that covers the graph's edges is:"
      << std::endl;
  for (unsigned int i = 0; i < cover.size(); ++i) {
    std::cout << cover[i] << std::endl;
  }
}

int main()
{
  /* Punctele (a) si (c) se rezolva pe graful cu costuri uniforme egale cu 1. */
  unweightedTasks();

  /* Punctele (b) si (d) se rezolva pe graful cu costuri diferite, dar pozitive. */
  weightedTasks();

  return 0;
}

