#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <queue>

#include "GraphAdjMat.h"
#include "DisjointSets.h"

using namespace std;


typedef pair<int, int> ii;

std::vector<Edge> prim(GraphAdjMat& graph, int seed)
{
  std::vector<Edge> returnValue;

  /* Initial, nici unul dintre noduri in afara de frunza nu apartine de
   * arbore. */
  std::vector<bool> in_tree(graph.get_n(), false);
  in_tree[seed] = true;

  /* Cream un vector de distante si il initializam cu distantele fata de seed. */
  int dist[graph.get_n()];
  int parent[graph.get_n()];

  for (unsigned int i = 0; i < graph.get_n(); ++i) {
    if (i != seed) {
      dist[i] = 1000000;//graph.get_edge(seed, i);
      parent[i] = seed;
    } else {
      dist[i] = 0;
      parent[i] = GraphAdjMat::NONE;
    }
  }

  /* TODO: Puneti in vectorul returnValue acele muchii care formeaza APM in urma
   * rularii algoritmului lui Prim din nodul psursa seed. */

	vector < bool > viz(graph.get_n(), false);
	priority_queue <ii, vector <ii>, greater<ii> > H;

	H.push(make_pair(0, seed));
	while(!H.empty()) {
		ii x = H.top();
		H.pop();

		if (viz[x.second])
			continue;
		returnValue.push_back(Edge(x.second, parent[x.second], graph.get_edge(x.second, parent[x.second])));

		viz[x.second] = true;

		for (int i=0; i<graph.get_n(); i++) {
			if (graph.get_edge(x.second, i) == GraphAdjMat::NONE || viz[i])
				continue;

			if (dist[i] > graph.get_edge(x.second, i)) {
				dist[i] = graph.get_edge(x.second, i);
				parent[i] = x.second;
				H.push(make_pair(dist[i], i));
			}
		}
	}



  return returnValue;
}


bool cmp(const Edge &a, const Edge &b) {
	return a.cost < b.cost;
}

std::vector<Edge> kruskall(GraphAdjMat& graph)
{
  std::vector<Edge> returnValue;

  /* Presupunem initial ca fiecare nod in graf este in propriul sau arbore. */
  DisjointSets tree_set(graph.get_n());

  /* TODO: Puneti in vectorul returnValue acele muchii care formeaza APM in urma
   * rularii algoritmului lui Kruskall. */

	std::vector< Edge > E = graph.get_edges();

	sort(E.begin(), E.end(), cmp );

	for (int i=0; i< E.size(); i++ ) {
		if (!tree_set.same_set(E[i].u, E[i].v) ) {
			returnValue.push_back(E[i]);
			tree_set.merge_sets_of(E[i].u, E[i].v);
		}
	}


  return returnValue;
}



int main()
{
  /* Deschidem un fisier si citim din el un graf neorientat. */
  std::ifstream in("src-lab9/Graph.in");
  int n;
  in >> n;
  GraphAdjMat graph(n);
  in >> graph;

  std::cout << "Initial graph: " << std::endl << graph << std::endl;
  std::vector<Edge> edges;

  /* Calculam muchiile pe care le alege algoritmul lui Prim. */
  edges = prim(graph, 0);
  std::cout << "Algoritmul lui Prim construieste pornind la nodul 0"
      << " arborele format din urmatoarele muchii:" << std::endl;
  for (unsigned int i = 0; i < edges.size(); ++i) {
    std::cout << edges[i] << std::endl;
  }

  /* Calculam muchiile pe care le alege algoritmul lui Kruskall. */
  edges = kruskall(graph);
  std::cout << "Algoritmul lui Kruskall construieste o padure de arbori"
      << " formata din urmatoarele muchii:" << std::endl;
  for (unsigned int i = 0; i < edges.size(); ++i) {
    std::cout << edges[i] << std::endl;
  }

  return 0;
}

