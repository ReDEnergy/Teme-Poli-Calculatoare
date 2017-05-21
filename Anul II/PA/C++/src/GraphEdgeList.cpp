#include <iostream>

#include "GraphEdgeList.h"

std::istream& operator>> (std::istream& in, GraphEdgeList & graph)
{
  unsigned int edge_no;
  in >> edge_no;
  for (unsigned int i = 0; i < edge_no; i++) {
    int nodea, nodeb, cost;
    in >> nodea >> nodeb >> cost; 
    graph.neighbours[nodea].push_back(std::pair<int,int>(nodeb,cost));
    graph.neighbours[nodeb].push_back(std::pair<int,int>(nodea,cost));
    graph.edgeList.push_back(std::pair<std::pair<int,int>, int>(std::pair<int,int>(nodea, nodeb), cost));
  }
  return in;
}

std::ostream& operator<< (std::ostream& out, GraphEdgeList & graph)
{
  out << "Number of nodes in graph: " << graph.get_n() << std::endl;
  for (unsigned int i = 0; i < graph.get_n(); i++){
    out << "neighbours(" << i << "): {";
    for (unsigned int j = 0; j < graph.neighbours[i].size(); j++){
      out << " (" << graph.neighbours[i][j].first << "=" << graph.neighbours[i][j].second << "$)";
    }
    out << " }" << std::endl;
  }
  return out << std::endl;
}

