#include <iostream>
#include <vector>
#include <algorithm>

#include "VectorIO.h"
#include "PairIO.h"

#define UNDEF -1

/* Un material este o pereche (greutate, valoare). */
typedef std::pair<int, int> Mobila;

int val_max(int t, std::vector<Mobila>& v)
{
  /* De aceasta daca nu mai putem lua bucati fractionare de mobila, ci doar
   * piese intregi, asa ca nu mai putem folosi o abordare de tip greedy. O
   * solutie este sa recurgem la urmatoarea relatie de recurenta:
   *      val_max(t) = max { val_max(t-x.greutate) + x.valoare }
   * unde x este o piesa de mobilier si (x.greutate < t).
   *
   * Se observa ca pentru a calcula val_max(t), avem nevoie doar de
   * val_max (1, ... t-1), deci putem reine valorile intr-un array si
   * putem calcula iterativ valorile functiei.
   *
   * Pentru a nu considera de doua ori aceeasi piesa de mobilier, vom
   * considera treptat toate solutiile care tin cont de primele {1, 2, ... n}
   * piese de mobilier si vom adauga doar cate un nou produs pentru a
   * urmari evolutia.
   */

  /* UNDEF inseamna ca respectiva incarcare a camionului este imposibila;
   * Camionul gol e posibil, dar nu transporta valoare. */
  std::vector<int> val_max(t + 1, UNDEF);
  val_max[0] = 0;

  /* Consideram pe rand toate produsele. */
  for (int produs = 0; produs < v.size(); ++produs) {
    int greutate = v[produs].first;
    int valoare = v[produs].second;

    for (int incarcare = t; incarcare >= greutate; --incarcare) {
      int restul_marfii = incarcare - greutate;

      if (val_max[restul_marfii] != UNDEF &&
          val_max[restul_marfii] + valoare > val_max[incarcare]) {
        /* Daca se poate realiza restul incarcarii din produse analizate
         * anterior si daca obtinem o valoare mai mare folosind acest produs
         * pentru incarcarea curenta atunci foloseste "produs" pentru a genera
         * incarcarea curenta. */
        val_max[incarcare] = val_max[restul_marfii] + valoare;
      }
    }
  }

  /* Rezultatul este valoarea maxima obtinuta pentru orice incarcare <= t */
  int rezultat = UNDEF;
  for (int incarcare = 0; incarcare <= t; ++incarcare) {
    if (rezultat == UNDEF || val_max[incarcare] > rezultat) {
      rezultat = val_max[incarcare];
    }
  }

  return rezultat;
}

int main()
{
  /* Declaram capacitatea camionului si un vector care sa retina tipurile de
   * mobila sub forma de perechi (greutate, valoare) si citim datele de intrare.
   */
  int t;
  std::vector<Mobila> v;
  std::cin >> t >> v;

  /* Afisam valoarea maxima transportabila de catre camion. */
  std::cout << "Valoarea maxima a mobilierului transportabil: "
      << val_max(t, v) << std::endl;

  return 0;
}

