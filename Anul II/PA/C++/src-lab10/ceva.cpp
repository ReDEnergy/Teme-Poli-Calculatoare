#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cstring>
#include <algorithm>

#include "FlowGraph.h"

#define NONE -1

std::vector<unsigned int> bfs(FlowGraph& graph,
                              unsigned int source,
                              unsigned int sink)
{
  /* Ne vom folosi de vectorul de parinti pentru a spune daca un nod a fost
   * adaugat sau nu in coada. */
  std::vector<unsigned int> parent(graph.size(), NONE);

  std::deque<unsigned int> q;
  q.push_back(source);

  while (parent[sink] == NONE && q.size() > 0) {
    unsigned int node = q.front();
    q.pop_front();

    for (unsigned int i = 0; i < graph.size(); ++i) {
      if (graph[node][i] > 0 && parent[i] == NONE) {
        parent[i] = node;
        q.push_back(i);
      }
    }
  }

  /* Daca nu s-a atins destinatia, atunci nu mai exista drumuri de crestere. */
  if (parent[sink] == NONE) {
    return std::vector<unsigned int>();
  }

  /* Reconstituim drumul de la destinatie spre sursa. */
  std::vector<unsigned int> returnValue;
  for (unsigned int node = sink; true; node = parent[node]) {
    returnValue.push_back(node);
    if (node == source) {
      break;
    }
  }

  /* Inversam drumul pentru a incepe cu sursa si a se termina cu destinatia. */
  std::reverse(returnValue.begin(), returnValue.end());
  return returnValue;
}

unsigned int saturate_path(FlowGraph& graph,
                           std::vector<unsigned int>& path)
{
  /* Niciodata nu ar trebui sa se intample asta pentru ca sursa si destinatia
   * sunt noduri distincte si cel putin unul dintre ele se afla in path. */
  if (path.size() < 2) {
    return 0;
  }

  /* Determinam fluxul maxim prin drum. */
  unsigned int flow = graph[path[0]][path[1]];
  for (unsigned int i = 0; i < path.size() - 1; ++i) {
    unsigned int u = path[i];
    unsigned int v = path[i + 1];
    flow = (flow < graph[u][v] ? flow : graph[u][v]);
  }

  /* Si il saturam in graf. */
  for (unsigned int i = 0; i < path.size() - 1; ++i) {
    unsigned int u = path[i];
    unsigned int v = path[i + 1];
    graph[u][v] -= flow;
    graph[v][u] += flow;
  }

  /* Raportam fluxul cu care am saturat graful. */
  return flow;
}


unsigned int maximum_flow(FlowGraph& graph,
                          unsigned int source,
                          unsigned int sink) {
  unsigned int returnValue = 0;

  /* Vom incerca in mod repetat sa determinam drumuri de crestere folosind
   * BFS si sa le saturam pana cand nu mai putem determina un astfel de drum in
   * graf. */
  while (true) {
    /* Determina drum de crestere. */
    std::vector<unsigned int> saturation_path = bfs(graph, source, sink);

    /* Daca nu exista drum de crestere, atunci intoarce fluxul maxim gasit. */
    if (saturation_path.size() == 0) {
      break;
    }

    /* Altfel satureaza drumul. */
    returnValue += saturate_path(graph, saturation_path);
  }

  return returnValue;
}

void min_cut(FlowGraph& graph,
             std::vector<unsigned int>& source_set,
             std::vector<unsigned int>& sink_set,
             unsigned int source)
{
  /* Facem o parcurgere BFS din nodul sursa si punem nodurile atinse de
   * parcurgere in source_set. Toate celelalte noduri se vor afla in
   * sink_set. */
  std::vector<bool> in_queue(graph.size(), false);
  std::deque<unsigned int> q;
  q.push_back(source);
  in_queue[source] = true;

  /* Rulam BFS din sursa si marcam nodurile atinse. */
  while (q.size()) {
    unsigned int node = q.front();
    q.pop_front();

    for (unsigned int i = 0; i < graph.size(); ++i) {
      if (in_queue[i] == false && graph[node][i] > 0) {
        in_queue[i] = true;
        q.push_back(i);
      }
    }
  }

  /* Traversam inca odata si impartim nodurile intre cele doua multimi. */
  for (unsigned int i = 0; i < graph.size(); ++i) {
    if (in_queue[i]) {
      source_set.push_back(i);
    } else {
      sink_set.push_back(i);
    }
  }
}

void disjoint_paths(FlowGraph& graph,
                    unsigned int source,
                    unsigned int sink)
{
  /* Caclculam fluxul prin graf. */
  unsigned int nr_paths = maximum_flow(graph, source, sink);
  std::cout << "There are " << nr_paths << " disjoint paths in the graph from "
      << source << " to " << sink << "." << std::endl;

  for (unsigned int i = 0; i < nr_paths; ++i) {
    /* Extragem drumuri mergand doar pe muchii complet saturate, pe care apoi le
     * desaturam. */
    std::vector<unsigned int> parent(graph.size(), NONE);
    std::deque<unsigned int> q;
    q.push_back(source);

    while (parent[sink] == NONE && q.size() > 0) {
      unsigned int node = q.front();
      q.pop_front();

      for (unsigned int i = 0; i < graph.size(); ++i) {
        if (graph.is_saturated(node, i)) {
          parent[i] = node;
          q.push_back(i);
        }
      }
    }

    if (parent[sink] == NONE) {
      std::cout << "Error! No path found." << std::endl;
      break;
    }

    /* Reconstruim calea. */
    std::vector<unsigned int> path;
    for (unsigned int node = sink; true; node = parent[node]) {
      path.push_back(node);
      if (node == source) {
        break;
      }
    }
    std::reverse(path.begin(), path.end());

    /* O desaturam in graf. */
    for (unsigned int i = 0; i < path.size() - 1; ++i) {
      graph[path[i]][path[i + 1]]++;
      graph[path[i + 1]][path[i]]--;
    }

    /* Si o afisam. */
    for (unsigned int i = 0; i < path.size(); ++i) {
      std::cout << (i == 0 ? "" : " - ") << path[i];
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void approximateAndPrint(FlowGraph& graph)
{
  /* Rulam flux in graf. */
  maximum_flow(graph, 0, graph.size() - 1);

  /* Si apoi printam matricea in functie de ce s-a saturat efectiv. */
  std::cout << "The approximated matrix is: " << std::endl;
  for (unsigned int i = 0; i < (graph.size() / 2) - 1; ++i) {
    for (unsigned int j = 0; j < (graph.size() / 2) - 1; ++j) {
      std::cout << (j == 0 ? "" : " ")
          << graph[graph.size() / 2 + j][1 + i];
    }
    std::cout << std::endl;
  }
}

int main()
{
  /* Deschidem fisierul de intrare si citim un graf din el. */
  std::ifstream in("src-lab10/Graph.in");
  FlowGraph graph;
  in >> graph;
  in.close();

  unsigned int source = 0;
  unsigned int sink = 5;

  /* Calculam si afisam fluxul maxim de date care poate fi suportat de retea
   * intre doua noduri oarecare: de exemplu, 0 si 5. */
  unsigned int flow = maximum_flow(graph, source, sink);
  std::cout << "The maximum flow in the graph between nodes 0 and 5 is: "
      << flow << "." << std::endl;

  /* Calculam si afisam o taietura minimala a grafului. */
  std::vector<unsigned int> source_set;
  std::vector<unsigned int> sink_set;
  min_cut(graph, source_set, sink_set, source);
  std::cout << "The minimum cut associated with the flow yields:" << std::endl;
  std::cout << "Set of the source: {";
  for (unsigned int i = 0; i < source_set.size(); ++i) {
    std::cout << (i == 0 ? " " : ", ") << source_set[i];
  }
  std::cout << " }" << std::endl;
  std::cout << "Set of the sink: {";
  for (unsigned int i = 0; i < sink_set.size(); ++i) {
    std::cout << (i == 0 ? " " : ", ") << sink_set[i];
  }
  std::cout << " }" << std::endl << std::endl;

  /* Deschidem un fisier din care citim topologia originala a grafului, */
  std::ifstream topo("src-lab10/GraphBinary.in");
  FlowGraph graphTopo;
  topo >> graphTopo;
  in.close();

  /* Printam un numar maximal de drumuri disjuncte intre aceleasi doua noduri
   * din graf. */
  disjoint_paths(graphTopo, source, sink);

  /* Deschidem un fisier din care sa incarcam un graf pentru o matrice. */
  std::ifstream matrix("src-lab10/GraphMatrix.in");
  FlowGraph graphMatrix;

  /* Graful incarcat din matricea N x N din fisier va arata astfel:
   * (a) Exista un nod sursa, cu numarul 0.
   * (b) Exista cate un nod pentru fiecare linie, numerotate cu 1, 2, ... N.
   *      Capacitatea de la 0 la nodurile liniilor este egala cu partea
   *      intreaga a sumei pe linia respectiva + 1.
   * (c) Exista cate un nod pentru fiecare coloana, numerotate cu N + 1, N + 2,
   *      ... 2 * N.
   *      Capacitatea de la fiecare nod pentru linie la fiecare nod pentru
   *      coloana este egala cu partea intreaga a respectivei celule + 1.
   * (d) Exista un nod destinatie, cu numarul 2 * N - 1.
   *      Capacitatea de la un nod pentru coaloana la nodul destinatie este
   *      egala cu partea intreaga a sumei pe coloana respectiva + 1.
   *
   * Se poate observa usor ca o aproximare valida este egala cu fluxul care
   * trece prin muchiile care reprezinta celulele din matrice in cazul saturarii
   * maxime a retelei de transport.
   */
  loadMatrixGraph(matrix, graphMatrix);
  matrix.close();

  /* Aproximam graful si printam matricea aproximata. */
  approximateAndPrint(graphMatrix);

  return 0;
}

