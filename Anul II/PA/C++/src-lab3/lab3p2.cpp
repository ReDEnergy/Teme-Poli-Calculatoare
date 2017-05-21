#include <iostream>
#include <vector>
#include <cstring>

#include "VectorIO.h"
#include "PairIO.h"

/* Vom considera o matrice ca o pereche de intregi (nr_linii, nr_coloane). */
typedef std::pair<int, int> Matrice;

void print_parantheses(int* last, int n, int begin, int end)
{
  if (begin == end) {
    std::cout << "m" << begin;
  } else {
    std::cout << "(";
    print_parantheses(last, n, begin, *(last + (begin * n) + end));
    std::cout << " x ";
    print_parantheses(last, n, *(last + (begin * n) + end) + 1, end);
    std::cout << ")";
  }
}

int min_multiplications(std::vector<Matrice>& v)
{
  /* Vom folosi aceeasi abordare bottom-up ca la problema 1. Notam:
   * minm[i][j] = numarul de minim operatii pentru a inmulti subsecventa de
   *              matrice (v[i], v[i+1], ... v[j]).
   *
   * Deoarece inmultirea matricelor este asociativa, practic operatiile se pot
   * efectua in orice ordine, rezultatul final fiind o matrice de dimensiuni:
   *              (#linii(v[i]), #coloane(v[j]))
   *
   * Pentru a calcula valorile minm[i][j], ne vom folosi de urmatoarea
   * recurenta:
   *
   * minm[i][j] = MIN{
   *                minm[i][k] +
   *                minm[k + 1][j] +
   *                #linii(v[k]) * #coloane(v[k]) * #coloane(v[k + 1])
   *              }, pentru toti i <= k < j.
   *
   * Cu alte cuvinte daca avem lantul de matrice:
   *              (m1 m2 m3 m4 m5 m6, ... mn)
   * vom incerca toate modalitatile sa il spargem in doua lanturi mai scurte si
   * apoi sa efectua produsul celor doua rezultate partiale:
   *              (m1 m2 ... mk) (mk+1 ... mn)
   *
   * Pentru tiparirea rezultatului, vom folosi si o matrice auxiliara
   * last[i][j] in care sa retinem unde se face ultima inmultire din lantul
   * (mi mi+1 ... mj).
   */

  int minm[v.size()][v.size()];
  int last[v.size()][v.size()];
  memset(minm, 0x00, v.size() * v.size() * sizeof(int));

  for (unsigned int len = 2; len <= v.size(); ++len) {
    for (unsigned int i = 0; i <= v.size() - len; ++i) {
      unsigned int j = i + len - 1;

      /* Presupunem initial impartirea (mi) (mi+1 ... mj) */
      minm[i][j] = minm[i + 1][j] + v[i].first * v[i].second * v[j].second;
      last[i][j] = i;

      for (unsigned int k = i; k < j; ++k) {
        /* Daca impartirea (mi ... mk) (mk+1 ... mj) da rezultate mai bune,
         * atunci alegem aceasta impartire. */
        int alternative = minm[i][k] + minm[k + 1][j] +
            v[i].first * v[k].second * v[j].second;
        if (alternative < minm[i][j]) {
          minm[i][j] = alternative;
          last[i][j] = k;
        }
      }
    }
  }

  /* Tiparim efectiv lantul parantezat. */
  print_parantheses(&last[0][0], v.size(), 0, v.size() - 1);
  std::cout << std::endl;

  /* Rezultatul final este minm pentru tot lantul. */
  return minm[0][v.size() - 1];
}

int main()
{
  /* Declaram si citim un vector de matrice de la consola. */
  std::vector<Matrice> v;
  std::cin >> v;

  /* Verificam intai ca lantul de matrice chiar se poate inmulti. */
  for (unsigned int i = 0; i < v.size() - 1; ++i) {
    if (v[i].second != v[i + 1].first) {
      std::cerr << "Fail! Nu se pot inmulti matricele " << i << " si "
          << (i + 1) << " de dimensiuni: " << v[i] << ", respectiv "
          << v[i + 1] << std::endl;
      return 0;
    }
  }

  /* Afisam numarul minim de operatii. */
  std::cout << "Numarul minim de operatii de inmultire elementare este: "
      << min_multiplications(v) << std::endl;

  return 0;
}

