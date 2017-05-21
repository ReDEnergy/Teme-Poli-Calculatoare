#include <iostream>
#include <cstring>

#include "GraphAdjMat.h"

void transitive_closure(GraphAdjMat & graph)
{
  /* TODO update the graph to find the paths of maximum minimum-badwith */
  bool mat[101][101];
  
	int i,j;
	int n = graph.get_n();
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			if (graph.get_edge(i,j) != -1 )
				mat[i][j] = true;
			else
				mat[i][j] = false;
		}

	int k;
	for (k = 0; k < n; ++k)
		for (i = 0; i < n; ++i)
			for (j = 0; j < n; ++j)
			{
				mat[i][j] = mat[i][j] || (mat[i][k] && mat[k][j]);
				graph.set_detour(i,j,k);
			}
			
	
}

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

