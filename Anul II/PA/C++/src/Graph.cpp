#include <iostream>

#include "Graph.h"

std::istream& operator>> (std::istream& in, Graph & graph)
{
  unsigned int neigh_no, neigh;
  for (unsigned int i = 0; i < graph.get_n(); i++) {
    in >> neigh_no;
    for (unsigned int j = 0; j < neigh_no; j++) {
      in >> neigh; 
      graph.neighbours[i].push_back(neigh);
      graph.transposed_neighbours[neigh].push_back(i);
    }
  }
  return in;
}

std::ostream& operator<< (std::ostream& out, Graph & graph)
{
  out << "Number of nodes in graph: " << graph.get_n() << std::endl;
  for (unsigned int i = 0; i < graph.get_n(); i++){
    out << "neighbours(" << i << "): {";
    for (unsigned int j = 0; j < graph.neighbours[i].size(); j++){
      out << " " << graph.neighbours[i][j];
    }
    out << " }" << std::endl;
  }
  out << std::endl;
  for (unsigned int i = 0; i < graph.get_n(); i++){
    out << "transpose_neighbours(" << i << "): {";
    for (unsigned int j = 0; j < graph.transposed_neighbours[i].size(); j++){
      out << " " << graph.transposed_neighbours[i][j];
    }
    out << " }" << std::endl;
  }
  return out << std::endl;
}

