#include "GraphAdjMat.h"

bool operator< (const Edge& left, const Edge& right) {
  return left.cost < right.cost;
}

std::ostream& operator<< (std::ostream& out, const Edge& edge) {
  return out << "{ edge: (" << edge.u << ", " << edge.v << "), cost: "
      << edge.cost << "}";
}

std::istream& operator>> (std::istream& in, GraphAdjMat& graph) {
  int no_edges, a, b, cost;
  in >> no_edges;
  for (int i = 0; i < no_edges; i++) {
    in >> a >> b >> cost;
    graph.set_edge(a,b,cost);
    graph.original[a][b] = cost;
    if (graph.directed == false) {
      graph.original[b][a] = cost;
    }
  }
  return in;
}

std::ostream& operator<< (std::ostream& out, GraphAdjMat& graph) {
  out << "Nr of nodes: " << graph.n << std::endl;
  for (unsigned int i = 0; i < graph.n; i++){
    out << "Accessible( " << i << " ) = {";
    for (unsigned int j = 0; j < graph.n; j++){
      if (graph.get_edge(i,j) != GraphAdjMat::NONE){
        out << " " << j;
      }
    }
    out << " }" << std::endl;
  }
  return out;
}

std::ostream& operator<< (std::ostream& out,
                          const GraphAdjMat::Path & path) {
  out << "The path in the graph (according to detours) is: " << std::endl;
  for (unsigned int i = 0; i < path.size(); i++) {
    out << "\t" << "(" << path[i].first.first << "," << path[i].first.second
        << ") =cost=> " << path[i].second << std::endl;
  }
  return out << std::endl;
}

