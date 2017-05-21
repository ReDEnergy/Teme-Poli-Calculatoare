#ifndef __REBUS_H__
#define __REBUS_H__

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include "VectorIO.h"

typedef std::vector<std::string> Dictionar;

class Rebus {
 public:
  unsigned int rows, columns;

  char** m;
  char** ref;

  /*! \brief Constructor pentru un careu de Rebus.
   * \param rows Numarul de linii ale careului.
   * \param columns Numarul de coloane ale careului.
   * \param in Un flux din care sa se citeasca configuratia careului.
   */
  Rebus() : rows(0), columns(0), m(NULL), ref(NULL) { }

  virtual ~Rebus() {
    if (m && ref) {
      for (unsigned int i = 0; i < rows; ++i) {
        delete[] m[i];
        delete[] ref[i];
      }
      delete[] m;
      delete[] ref;
    }
  }

  /*! \brief Functie care returneaza o litera din careu.
   * \param row Linia din careu.
   * \param column Coloana din careu.
   * \return Caracterul de la linia si coloana respectiva din careu. Pentru o
   * celula de careu goala se va intoarce '_', iar pentru o celula de careu
   * blocata se va intoarce '*'. Daca apelul este unul invalid (in afara
   * careului), se intoarce '\0'.
   */
  inline char get(unsigned int row, unsigned int column) {
    if (m && row < rows && row >= 0 && column < columns && column >= 0) {
      return m[row][column];
    } else {
      return '\0';
    }
  }

  /*! \brief Functie care pune o litera in careu. Functia nu are nici un efect
   * daca respectiva celula din careu contine '*'. Daca apelul este unul invalid
   * (in afara careului), el nu are nici un efect.
   * \param row Linia din careu.
   * \param column Coloana din careu.
   * \param c Caracterul care va fi scris in careu.
   */
  inline void put(unsigned int row, unsigned int column, char c) {
    if (m && row < rows && row >= 0 && column < columns && column >= 0) {
      if (m[row][column] != '*' && c >= 'A' && c <= 'Z') {
        m[row][column] = c;
      }
    }
  }

  /*! \brief Functie care pune un string orizontal in careu. Functia nu are nici
   * un efect daca nu exista spatiu suficient.
   * \param row Linia din careu.
   * \param column Coloana din careu.
   * \param s Sttring care va fi scris in careu.
   */
  inline void putString(unsigned int row, unsigned int column, std::string s) {
    unsigned int free = 0;
    for (unsigned int j = column; j < columns && is_empty(row, j); ++j) {
      free++;
    }
    if (free == s.length()) {
      for (unsigned int i = 0; i < s.length(); ++i) {
        put(row, column + i, s[i]);
      }
    }
  }

  /*! \brief Functie care sterge un string orizontal din careu (pana la marginea
   * careului sau pana la urmatorul caracter '*'.
   * \param row Linia locatiei de unde incepe stringul orizontal.
   * \param column Coloana locatiei de unde incepe stringul orizontal.
   */
  inline void eraseString(unsigned int row, unsigned int column) {
    for (unsigned int j = column; j < columns && m[row][j] != '*'; ++j) {
      erase(row, j);
    }
  }

  /*! \brief Functie care sterge o litera din careu. Functia nu are nici un
   * efect daca respectiva celula din careu contine '*'. Daca apelul este unul
   * invalid (in afara careului), el nu are nici un efect.
   * \param row Linia din careu.
   * \param column Coloana din careu.
   */
  inline void erase(unsigned int row, unsigned int column) {
    if (m && row < rows && row >= 0 && column < columns && column >= 0) {
      if (m[row][column] != '*') {
        m[row][column] = '_';
      }
    }
  }

  /*! \brief Functie care verifica daca o celula din careu este libera (trebuie
   * sa nu fie completata si sa nu fie '*'. Daca apelul este unul invalid (in
   * afara careului), se intoarce <b>false</b>.
   * \param row Linia din careu.
   * \param column Coloana din careu.
   * \return <b>true</b> daca respectiva celula este libera si <b>false</b> in
   * caz contrar.
   */
  inline bool is_empty(unsigned int row, unsigned int column) {
    if (m && row < rows && row >= 0 && column < columns && column >= 0) {
      return m[row][column] == '_';
    } else {
      return false;
    }
  }

  /*! \brief Functie care verifica daca careul este completat valid.
   * \return <b>true</b> daca careul este unul valid si corect completat, sau
   * <b>false</b> altfel.
   */
  bool is_done() {
    if (m == NULL) {
      return false;
    }

    for (unsigned int i = 0; i < rows; ++i) {
      for (unsigned int j = 0; j < columns; ++j) {
        if (ref[i][j] != m[i][j]) {
          return false;
        }
      }
    }

    return true;
  }

  friend std::ostream& operator<< (std::ostream&, const Rebus&);
  friend std::istream& operator>> (std::istream&, Rebus&);

};

#endif

