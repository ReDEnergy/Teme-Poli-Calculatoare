#ifndef __PAIRIO__
#define __PAIRIO__

#pragma once

#include <iostream>

/*! \brief Operator de afisare al continutului unei perechi. Pentru a functiona
 * corect, este necesar ca ambele elemente din pereche sa fie la randul lor
 * afisabile.
 * \param out Stream-ul in care se face afisarea.
 * \param pair Perechea pare se afiseaza
 * \return Stream-ul in care se face afisarea (pentru chaining).
 */
template<typename T, typename V>
std::ostream& operator<< (std::ostream& out, const std::pair<T, V>& pair)
{
  return (out << "(" << pair.first << ", " << pair.second << ")");
}

/*! \brief Operator de citire al continutului unei perechi. Pentru a functiona
 * corect, este necesar ca elementele perechii sa fie la randul lor citibile.
 * \param in Stream-ul din care se face citirea.
 * \param pair Perechea in care se face citirea.
 * \return Stream-ul din care se face citirea (pentru chaining).
 */
template<typename T, typename V>
std::istream& operator>> (std::istream& in, std::pair<T, V>& pair)
{
  return (in >> pair.first >> pair.second);
}

#endif

