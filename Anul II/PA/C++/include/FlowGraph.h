#ifndef __FLOW_GRAPH__
#define __FLOW_GRAPH__

#pragma once

#include <iostream>
#include <fstream>

struct FlowGraph {
 private:
  unsigned int n;
  int** capacity;
  int** edge;

 public:
  FlowGraph() : n(0), capacity(NULL), edge(NULL) { }

  ~FlowGraph() {
    if (capacity != NULL) {
      for (unsigned int i = 0; i < n; ++i) {
        delete[] capacity[i];
        delete[] edge[i];
      }
      delete[] capacity;
      delete[] edge;
    }
  }

  /** \brief Functie care intoarce numarul de noduri din graf.
   * \return Numarul de noduri din graf.
   */
  inline unsigned int size() {
    return n;
  }

  /** \brief Functie care permite accesul la graf ca la o matrice de adiacenta.
   * \param index Un nod in graf.
   * \return Un pointer catre linia din matricea de adiacenta a nodului index.
   */
  inline int* operator[] (int index) {
    return capacity[index];
  }

  /** \brief Functie care spune daca muchia (u, v) din graf este sau nu
   * saturata.
   * \return <b>True</b> daca muchia este saturata, sau <b>False</b> altfel.
   */
  inline bool is_saturated(unsigned int u, unsigned int v) {
    return capacity[u][v] == 0 && edge[u][v];
  }

  friend std::istream& operator>> (std::istream& in, FlowGraph& graph);
  friend void loadMatrixGraph(std::istream& in, FlowGraph& graph);
};

std::istream& operator>> (std::istream& in, FlowGraph& graph);

#endif

