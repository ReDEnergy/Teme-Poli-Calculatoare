#ifndef __GRAPHEDGELIST_H__
#define __GRAPHEDGELIST_H__

#pragma once

#include <iostream>
#include <fstream>
#include <vector>

class GraphEdgeList;

/** \brief Operator care afiseaza un GraphEdgeList intr-un stream.
 * \param out stream-ul in care se doreste sa se faca afisarea
 * \param GraphEdgeList GraphEdgeListul care se afiseaza
 * \return O referinta catre stream-ul in care s-a facut afisarea (pentru coerenta operatorilor in chaining)
 */
std::ostream& operator<< (std::ostream& out, const GraphEdgeList & graph);

/** \brief Operator care citeste un GraphEdgeList dintr-un stream. <b>ATENTIE!</b> Se citeste doar structura GraphEdgeListului, numarul de noduri fiind cel de la instantiere!
 * \param in stream-ul din care se doreste sa se faca citirea
 * \param GraphEdgeList GraphEdgeListul in care se doreste sa se citeasca datele
 * \return O referinta catre stream-ul din care se face citirea (pentru coerenta operatorilor in chaining)
 */
std::istream& operator>> (std::istream& out, const GraphEdgeList & graph);

class GraphEdgeList {

 private:
  unsigned int n;

  std::vector< std::vector<std::pair<int,int> > > neighbours;

  std::vector< std::pair< std::pair<int,int>, int> > edgeList;

 public:  
  /**
   * \brief Constructor
   * \param n numarul de noduri
   */
  inline GraphEdgeList(unsigned int n) :
      n(n)
  {
    neighbours.resize(n);
  }

  /**
   * \brief Functie care returneaza numarul de noduri din GraphEdgeList
   * \return Numarul de noduri din GraphEdgeList
   */
  inline unsigned int get_n() const {
    return n;
  }

  /**
   * \brief Functie care returneaza lista vecinilor unui nod din GraphEdgeList, ca vector de perechi (vecin, cost_muchie)
   * \param nod Nodul pentru care se cere lista vecinilor. <b>ATENTIE!</b> Este responsabilitatea voastra sa dati un nod intre 0 si dimensiunea GraphEdgeListului - 1!
   * \return Lista vecinilor unui nod din GraphEdgeList
   */
  inline std::vector< std::pair<int,int> > & get_neighbours(int nod) {
    return neighbours[nod];
  }

  /** 
   * \brief Functie care returneaza lista de muchii din graf ca vector de perechi (muchie, cost)
   * \return Lista de muchii din graf, ca vector de perechi (muchie, cost)
   */
  inline std::vector< std::pair< std::pair<int,int>, int> > get_edges() {
    return edgeList;
  }

  friend std::ostream& operator<< (std::ostream& out, GraphEdgeList & graph);
  friend std::istream& operator>> (std::istream& in, GraphEdgeList & graph);
};

#endif

