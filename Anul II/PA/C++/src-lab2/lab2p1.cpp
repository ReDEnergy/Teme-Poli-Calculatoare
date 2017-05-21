#include <iostream>
#include <vector>
#include <algorithm>

#include "VectorIO.h"
#include "PairIO.h"

/* Un material este o pereche (greutate, valoare). */
typedef std::pair<int, double> Material;

bool compare_unit_value(const Material& left, const Material& right)
{
  /* Vom sorta crescator dupa valoarea unitara. */
  return (left.second/left.first) > (right.second/right.first);
}

double val_max(int t, std::vector<Material>& v)
{
  /* Deoarece putem lua si cantitati fractionare de materiale, vom sorta
   * descrescator dupa pretul per cantitate si vom alege cele mai scumpe
   * produse pana cand vom umple camionul. 
   */
  std::sort(v.begin(), v.end(), compare_unit_value);

  double val_max = 0;
  /* Parcurgem vectorul sortat si alegem produse pana cand nu mai este loc
   * in camion sau pana cand nu mai avem produse. 
   */
  for (int i = 0; i < v.size() && t > 0; ++i){
    if (t >= v[i].first){
      /* Daca putem lua produsul in intregime. */
      val_max += v[i].second;
      t -= v[i].first;
    } else {
      /* Nu avem destul loc, luam doar cat mai este loc in camion */
      val_max += (v[i].second/v[i].first)*t;
      t = 0;
    }
  }

  /* Valoarea maxima a produselor care se poate obtine este val_max */
  return val_max;
}

int main()
{
  /* Declaram capacitatea camionului si un vector care sa retina tipurile
   * de material sub forma de perechi (greuate, valoare) si citim datele
   * de intrare.
   */
  int t;
  std::vector<Material> v;
  std::cin >> t >> v;

  /* Afisam valoarea maxima transportabila de catre camion. */
  std::cout << "Valoarea maxima a unui transport: "
      << val_max(t, v) << std::endl;

  return 0;
}

