#include <iostream>
#include <cstring>

#include "GraphAdjMat.h"
#define INF 10000

using namespace std;

void transitive_closure(GraphAdjMat & graph)
{
  /* TODO update the graph to find the paths of maximum minimum-badwith */
  int mat[101][101];

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

/**
foreach (i, j) in (1..n,1..n)
    d[i, j] = w[i,j] // costul muchiei, sau infinit
for k = 1 to n
    foreach (i,j) in (1..n,1..n)
        d[i, j] = min(d[i, j], d[i, k] + d[k, j])
 **/

int main()
{
  /* read a number of nodes from the input */
  int n;
  std::cin >> n;

  /* create a graph and proceed to load it from the input */
  GraphAdjMat graph(n);
  std::cin >> graph;

  /* print out the initial graph */
  std::cout << "Initial graph:" << std::endl << graph << std::endl;

  /* perform transitive closure on the graph */
 transitive_closure(graph);

  /* print out the result */
  std::cout << "After computing distances, accesibility is:" << std::endl << graph << std::endl;

  /* print out all paths in the graph */
  for (unsigned int i = 0; i < graph.get_n(); i++){
    for (unsigned int j = 0; j < graph.get_n(); j++){
      std::cout << "Path from " << i << " to " << j << ": " << graph.path(i,j) << std::endl;
    }
  }

  return 0;
}

