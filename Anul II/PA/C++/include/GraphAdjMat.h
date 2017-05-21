#ifndef __GRAPHADJMAT_H__
#define __GRAPHADJMAP_H__

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <cstring>

struct Edge {
  /* O muchie este caracterizata de doua capete si de un cost. */
  int u, v;
  int cost;

  Edge() { }

  Edge(int u, int v, int cost) : u(u), v(v), cost(cost) { }
};

/* Operator care sorteaza muchiile crescator dupa cost. */
bool operator< (const Edge& left, const Edge& right);
std::ostream& operator<< (std::ostream& out, const Edge& edge);

class GraphAdjMat {

 private:
  unsigned int n;

  bool directed;

  int** edges;
  int** original;
  int** detour;

 public:

  static const int NONE = -1;

  typedef std::vector< std::pair< std::pair<int, int>, int> > Path;

  /**
   * \brief Constructor
   * \param n numarul de noduri
   * \param directed specifica daca graful trebuie sa fie orientat (implicit,
   * este "false").
   */
  inline GraphAdjMat(unsigned int n, bool directed = false) :
      n(n),
      directed(directed)
  {
    edges = new int*[n];
    original = new int*[n];
    detour = new int*[n];

    for (unsigned int i = 0; i < n; i++) {
      edges[i] = new int[n];
      original[i] = new int[n];
      detour[i] = new int[n];
      for (unsigned int j = 0; j < n; j++) {
        edges[i][j] = NONE;
        original[i][j] = NONE;
        detour[i][j] = NONE;
      }
    }
  }

  /**
   * \brief Functie care reseteaza toate drumurile din graf
   * (folositi inainte de a rula din nou algoritmi de rutare).
   */
  inline void clear_paths(){
    for (unsigned int i = 0; i < n; i++) {
      for (unsigned int j = 0; j < n; j++) {
        detour[i][j] = NONE;
        edges[i][j] = original[i][j];
      }
    }
  }

  /**
   * \brief Functie care intoarce vectorul de muchii din graf.
   * \return Vectorul de muchii din graf.
   */
  inline std::vector<Edge> get_edges() {
    std::vector<Edge> returnValue;
    for (unsigned int i = 0; i < n; ++i) {
      for (unsigned int j = 0; j < n; ++j) {
        if (edges[i][j] != NONE && (i < j || directed == true)) {
          returnValue.push_back(Edge(i, j, edges[i][j]));
        }
      }
    }

    return returnValue;
  }

  /**
   * \brief Functie care returneaza numarul de noduri din Graph
   * \return Numarul de noduri din Graph.
   */
  inline unsigned int get_n() const {
    return n;
  }

  /**
   * \brief Functie care determina daca o muchie exista in graf
   * \return Costul muchiei, daca aceasta exista, sau NONE, daca ea nu exista.
   */
  inline int get_edge(int a, int b) const {
    return edges[a][b];
  }

  /**
   * \brief Functie care seteaza un cost in graf.
   */
  inline void set_edge(int a, int b, int cost) {
    edges[a][b] = cost;
    if (directed == false) {
      edges[b][a] = cost;
    }
  }

  /**
   * \brief Functie care sterge o muchie din graf.
   */
  inline void erase_edge(int a, int b) {
    set_edge(a,b, NONE);
  }

  /**
   * \brief Functie care returneaza urmatorul hop de pe drumul de la nodul a la
   * nodul b (hopul trebuie setat manual in prealabil in cadrul rularii unui
   * algoritm pe acest graf.
   */
  inline int get_detour(int a, int b) {
    return detour[a][b];
  }

  /**
   * \brief Functie care stabileste ca drumul optim de la a la b trebuie
   * sa treaca prin det (folosit la reconstructia de drumuri).
   */
  inline void set_detour(int a, int b, int det) {
    detour[a][b] = det;
    if (directed == false) {
      detour[b][a] = det;
    }
  }

  /**
   * \brief Functie care intoarce un drum intre doua noduri
   * \return Drumul dintre nodurile a si b.
   */
  inline Path path(int a, int b) const {
    Path returnValue;
    std::stack< std::pair<int,int> > track;
    track.push(std::pair<int,int>(a,b));
    while (!track.empty()){
      a = track.top().first;
      b = track.top().second;
      track.pop();
      if (detour[a][b] == NONE) {
        if (edges[a][b] == NONE) {
          std::cerr << "Broken path! No edge from " << a
              << " to " << b << "!" << std::endl;
          returnValue.clear();
          break;
        } else {
          returnValue.push_back(
              std::pair< std::pair<int,int>, int>(
                  std::pair<int,int>(a,b), edges[a][b]));
        }
      } else {
        track.push(std::pair<int,int>(detour[a][b], b));
        track.push(std::pair<int,int>(a, detour[a][b]));
      }
    }
    return returnValue;
  }

  friend std::ostream& operator<< (std::ostream& out, Path & path);
  friend std::ostream& operator<< (std::ostream& out, GraphAdjMat & graph);
  friend std::istream& operator>> (std::istream& in, GraphAdjMat & graph);

  ~GraphAdjMat() {
    for (unsigned int i = 0; i < n; i++) {
      delete[] edges[i];
      delete[] original[i];
      delete[] detour[i];
    }
    delete[] edges;
    delete[] original;
    delete[] detour;
  }
};

/** \brief Operator care afiseaza un drum din graf intr-un stream.
 * \param out stream-ul in care se doreste sa se faca afisarea.
 * \param path Drumul care se afiseazai.
 * \return O referinta catre stream-ul in care s-a facut afisarea
 * (pentru coerenta operatorilor in chaining).
 */
std::ostream& operator<< (std::ostream& out, const GraphAdjMat::Path& path);

/** \brief Operator care afiseaza un Graph intr-un stream.
 * \param out stream-ul in care se doreste sa se faca afisarea
 * \param Graph Graphul care se afiseaza
 * \return O referinta catre stream-ul in care s-a facut afisarea
 * (pentru coerenta operatorilor in chaining)
 */
std::ostream& operator<< (std::ostream& out, const GraphAdjMat& graph);

/** \brief Operator care citeste un Graph dintr-un stream. <b>ATENTIE!</b>.
 * Se citeste doar structura Graphului, numarul de noduri fiind cel de la
 * instantiere!
 * \param in Stream-ul din care se doreste sa se faca citirea.
 * \param Graph Graphul in care se doreste sa se citeasca datele.
 * \return O referinta catre stream-ul din care se face citirea
 * (pentru coerenta operatorilor in chaining).
 */
std::istream& operator>> (std::istream& out, const GraphAdjMat& graph);

#endif

