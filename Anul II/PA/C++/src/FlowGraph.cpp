#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cstring>

#include "FlowGraph.h"

std::istream& operator>> (std::istream& in, FlowGraph& graph)
{
  /* Citim numarul de noduri din graf. */
  in >> graph.n;

  /* Alocam graful. */
  graph.capacity = new int*[graph.n];
  graph.edge = new int*[graph.n];
  for (unsigned int i = 0; i < graph.n; ++i) {
    graph.capacity[i] = new int[graph.n];
    graph.edge[i] = new int[graph.n];
    memset(graph.capacity[i], 0x00, graph.n * sizeof(int));
    memset(graph.edge[i], 0x00, graph.n * sizeof(int));
  }

  /* Si punem capacitatile de pe fiecare muchie. */
  unsigned int edges;
  in >> edges;
  for (unsigned int i = 0; i < edges; ++i) {
    unsigned int u, v, capacity;
    in >> u >> v >> capacity;
    graph[u][v] = capacity;
    graph.edge[u][v] = 1;
  }

  return in;
}

void loadMatrixGraph(std::istream& in, FlowGraph& graph)
{
  /* Citim dimensiunile matricei si alocam graful. */
  unsigned int n;
  in >> n;
  graph.n = n * 2 + 2;
  graph.capacity = new int*[graph.n];
  graph.edge = new int*[graph.n];
  for (unsigned int i = 0; i < graph.n; ++i) {
    graph.capacity[i] = new int[graph.n];
    graph.edge[i] = new int[graph.n];
    memset(graph.capacity[i], 0x00, graph.n * sizeof(int));
    memset(graph.edge[i], 0x00, graph.n * sizeof(int));
  }

  double inc[n];
  double outc[n];
  memset(inc, 0x00, n * sizeof(int));
  memset(outc, 0x00, n * sizeof(int));
  for (unsigned int i = 0; i < n; ++i) {
    for (unsigned int j = 0; j < n; ++j) {
      double x;
      in >> x;
      inc[i] += x;
      graph[1 + i][1 + n + j] = ((int)x) + 1;
      outc[j] += x;
    }
  }
  for (unsigned int i = 0; i < n; ++i) {
    graph[0][1 + i] = ((int)inc[i]) + 1;
    graph[1 + n + i][2 * n + 1] = ((int)outc[i]) + 1;
  }
}

