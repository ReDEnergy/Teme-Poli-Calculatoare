#include <iostream>
#include <cstring>
#include <vector>

#include "GraphAdjMat.h"

using namespace std;

#define MAX 100
#define INF 10000

int P[MAX];
int d[MAX];
int next_hop;

void dijkstra(GraphAdjMat & graph, int source, int dest)
{
  /* TODO 1: compute the best path from source to dest.
     REMEMBER to also set the detours properly!
     If (source,y) comes from (source,x) U (x,y), then the detour of (source,y)
     MUST be x!
   */

	vector<int> Q;

	int i,j,k;
	int n = graph.get_n();
	bool selected[MAX];
	for ( i = 0; i < n; ++i )
		selected[i] = false;

	selected[source] = true;

	for ( i = 0; i < n; ++i )
		if ( graph.get_edge(source, i) != -1 )
		{
			d[i] = graph.get_edge(source, i);
			Q.push_back(i);

			P[i] = source;
		}
		else
		{
			d[i] = INF;
			P[i] = -1;
		}

	int dimQ = Q.size();

	while(dimQ)
	{
		int min = INF, poz;
		int u;
		for (i = 0; i < Q.size(); ++i)
			if ( Q[i] != -1 && Q[i] < min)
			{
				min = Q[i];
				poz = i;
			}
		u = Q[poz];
		Q[poz] = -1;
		dimQ--;

		selected[u] = true;

		for (i = 0; i < n; ++i)
		{
			if (graph.get_edge(u,i) != -1 &&
				!selected[i] && d[i] > d[u] + graph.get_edge(u, i) )
			{
				d[i] = d[u] + graph.get_edge(u,i);
				P[i] = u;
			}
		}
	}

	next_hop = dest;
	while (P[next_hop]!=source && P[next_hop] != -1)
		next_hop = P[next_hop];

}

GraphAdjMat::Path minimum_cycle(GraphAdjMat & graph, int u, int v)
{
  /* erase the direct edge between u and v */
  int backup = graph.get_edge(u,v);
  graph.erase_edge(u,v);

  /* clear all paths in th graph (dijkstra will compute other paths) */
  graph.clear_paths();

  /* run Dijkstra to find the best alternative path from u to v */
  dijkstra(graph, u, v);
  GraphAdjMat::Path returnValue = graph.path(u,v);

  /* put the edge back (both in the graph and in the cycle) */
  graph.set_edge(u,v,backup);
  returnValue.push_back(std::pair< std::pair<int,int>, int>(std::pair<int, int>(u,v), backup));

  /* return the cycle in the graph */
  return returnValue;
}

std::vector<GraphAdjMat::Path> get_all_minimum_cycles(GraphAdjMat & graph)
{
  std::vector<GraphAdjMat::Path> returnValue;
  /* TODO 2: Use the minimum_cycle function to solve this task.
     REMEMBER not to consider the same cycle twice!
   */

	int n = graph.get_n();

	int matr[1000][1000];
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			matr[i][j] = graph.get_edge(i,j);

	for (int k = 0; k < n; ++k)
		for (int i = 0; k < n; ++k)
			for (int j = 0; j < n; ++j)
				matr[i][j] = min( matr[i][j], matr[i][k] + matr[k][j] );

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
		{
			if( matr[j][i] + graph.get_edge(i,j) < INF)
				cout << "Ciclu la " << i << " si " << j << endl;
		}

  return returnValue;
}

int main()
{
  /* read a number of nodes from the input */
  int n;
  std::cin >> n;

  /* create an UNDIRECTED graph and proceed to load it from the input */
  GraphAdjMat graph(n, false);
  std::cin >> graph;

  /* print out the initial graph */
  std::cout << "Initial graph:" << std::endl << graph << std::endl;


	for (int i = 0; i < graph.get_n(); ++i) {
		printf("\nSursa: %d \n", i);
		for (int j = 0; j < graph.get_n(); ++j)
		{
			if ( i != j )
			{
				next_hop = j;
				dijkstra(graph, i, j);
				cout << "Dest: " << j << " - Next hop: ";
				if (d[j] == INF)
					printf("* - Cost : INF\n");
				else
					printf("%d - Cost : %d\n", next_hop, d[j]);
			}
		}
	}

  /* TODO 1: look for the minimum cycle between some given nodes */

  /* TODO 2: look for all minimum cycles in the graph */

  return 0;
}
