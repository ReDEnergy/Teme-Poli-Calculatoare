#ifndef __GRAPH_H__
#define __GRAPH_H__

#pragma once

#include <iostream>
#include <fstream>
#include <vector>

class Graph;

/** \brief Operator care afiseaza un Graph intr-un stream.
 * \param out stream-ul in care se doreste sa se faca afisarea
 * \param Graph Graphul care se afiseaza
 * \return O referinta catre stream-ul in care s-a facut afisarea (pentru coerenta operatorilor in chaining)
 */
std::ostream& operator<< (std::ostream& out, const Graph & graph);

/** \brief Operator care citeste un Graph dintr-un stream. <b>ATENTIE!</b> Se citeste doar structura Graphului, numarul de noduri fiind cel de la instantiere!
 * \param in stream-ul din care se doreste sa se faca citirea
 * \param Graph Graphul in care se doreste sa se citeasca datele
 * \return O referinta catre stream-ul din care se face citirea (pentru coerenta operatorilor in chaining)
 */
std::istream& operator>> (std::istream& out, const Graph & graph);

class Graph {

 private:
  unsigned int n;

  std::vector< std::vector<int> > neighbours;
  std::vector< std::vector<int> > transposed_neighbours;

 public:  
  /**
   * \brief Constructor
   * \param n numarul de noduri
   */
  inline Graph(unsigned int n) :
      n(n)
  {
    neighbours.resize(n);
    transposed_neighbours.resize(n);
  }

  /**
   * \brief Functie care returneaza numarul de noduri din Graph
   * \return Numarul de noduri din Graph
   */
  inline unsigned int get_n() const {
    return n;
  }

  /**
   * \brief Functie care returneaza lista vecinilor unui nod din Graph 
   * \param nod Nodul pentru care se cere lista vecinilor. <b>ATENTIE!</b> Este responsabilitatea voastra sa dati un nod intre 0 si dimensiunea Graphului - 1!
   * \return Lista vecinilor unui nod din Graph
   */
  inline std::vector<int> & get_neighbours(int nod) {
    return neighbours[nod];
  }

  /**
   * \brief Functie care returneaza lista vecinilor unui nod din Graphul transpus
   * \return Lista vecinilor unui nod din Graphul transpus. <b>ATENTIE!</b> Este responsabilitatea voastra sa dati un nod intre 0 si dimensiunea Graphului - 1!
   */
  inline std::vector<int> & get_transposed_neighbours(int nod) {
    return transposed_neighbours[nod];
  }

  friend std::ostream& operator<< (std::ostream& out, Graph & graph);
  friend std::istream& operator>> (std::istream& in, Graph & graph);
};

#endif

