#ifndef __VECTORIO__
#define __VECTORIO__

#pragma once

#include <iostream>
#include <vector>

/*! \brief Operator de afisare al continutului unui vector. Pentru a functiona
 * corect, este necesar ca toate elementele din vector sa fie la randul lor
 * afisabile.
 * \param out Stream-ul in care se face afisarea.
 * \param v Vectorul care se afiseaza.
 * \return Stream-ul in care se face afisarea (pentru chaining).
 */
template<typename T>
std::ostream& operator<< (std::ostream& out, std::vector<T>& v)
{
  for (unsigned int i = 0; i < v.size(); ++i) {
    out << (i > 0 ? " " : "") << v[i];
  }
  
  return out;
}

/*! \brief Operator de citire al continutului unui vector. Se citeste mai intai
 * dimensiunea vectorului exprimara in numarul de elemente, apoi se citesc pe
 * rand toate elementele. Pentru a functiona corect, este necesar ca elementele
 * din vector sa fie la randul lor citibile. Citirea unui vector suprascrie tot
 * continutul acelui vector.
 * \param in Stream-ul din care se face citirea.
 * \param v Vectorul in care se face citirea.
 * \return Stream-ul din care se face citirea (pentru chaining).
 */
template<typename T>
std::istream& operator>> (std::istream& in, std::vector<T>& v)
{
  T element;
  unsigned int size;
  in >> size;
  
  v.clear();
  for (unsigned int i = 0; i < size; ++i) {
    in >> element;
    v.push_back(element);
  }

  return in;
}

#endif

