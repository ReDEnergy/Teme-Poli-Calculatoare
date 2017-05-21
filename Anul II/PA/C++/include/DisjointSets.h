#ifndef __DISJOINTSETS_H__
#define __DISJOINTSETS_H__

#pragma once

#include <iostream>
#include <fstream>
#include <vector>

class DisjointSets {

 private:
  unsigned int n;
  unsigned int no_sets;

  std::vector<int> set;
  std::vector<int> rank;

  inline int get_ID(int n) {
    int query = n;
    while (set[n] != n) {
      n = set[n];
    }
    set[query] = n;
    return n;
  }

  inline void merge_sets(int ID_set_a, int ID_set_b) {
    if (rank[ID_set_a] < rank[ID_set_b]) {
      set[ID_set_a] = ID_set_b;
    } else if (rank[ID_set_a] > rank[ID_set_b]) {
      set[ID_set_b] = ID_set_a;
    } else {
      set[ID_set_a] = ID_set_b;
      rank[ID_set_b]++;
    }
    no_sets--;
  }

 public:
  /**
   * \brief Constructor. Initial, fiecare element se afla in propria sa multime.
   * \param n Numarul de multimi initiale.
   */
  inline DisjointSets(unsigned int n)
      : n(n), no_sets(n)
  {
    for (unsigned int i = 0; i < n; i++) {
      set.push_back(i);
      rank.push_back(0);
    }
  }

  /**
   * \brief Functie care returneaza numarul de noduri din set.
   * \return Numarul de noduri din set.
   */
  inline unsigned int get_n() const {
    return n;
  }

  /**
   * \brief Functie care returneaza numarul de seturi distincte.
   * \return Numarul de seturi distincte.
   */
  inline unsigned int number_of_disjoint_sets() const {
    return no_sets;
  }

  /** 
   * \brief Functie care reuneste multimile din care fac parte doua noduri.
   * \param a Primul <b>nod</b> 
   * \param b Al doilea <b>nod</b>.
   */
  inline void merge_sets_of(int ID_node_a, int ID_node_b) {
    int ID_set_a = get_ID(ID_node_a);
    int ID_set_b = get_ID(ID_node_b);
    if (ID_set_a != ID_set_b) {
      merge_sets(ID_set_a, ID_set_b);
    }
  }

  /** 
   * \brief Functie care determina daca doua noduri fac parte din acelasi set.
   * \param a Primul <b>nod</b>.
   * \param b Al doilea <b>nod</b>.
   */
  inline bool same_set(int ID_node_a, int ID_node_b) {
    return get_ID(ID_node_a) == get_ID(ID_node_b);
  }

  friend class Maze;
};

#endif

