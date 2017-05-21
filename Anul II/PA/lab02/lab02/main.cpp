#include <iostream>
#include <vector>
#include <algorithm>

#include "VectorIO.h"
#include "PairIO.h"

/* Un material este o pereche (greutate, valoare). */
typedef std::pair<int, double> Material;

double val_max(int t, std::vector<Material>& v)
{
  /* TODO: Valoarea maxima care se poate transporta. */
  return 0;
}

bool compare(const Material &a, const Material &b) {
	return a.first * b.second < b.first * a.second;
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

  std::sort(v.begin(), v.end(), compare);


  std::cout << v << std::endl;

  int val = 0, i=0;
  while (t) {
	  if(t >= v[i].first) {
		  val +=v[i].second;
		  t -= v[i].first;
	  }
	  else  {
		  val += t * v[i].second/v[i].first;
		  t=0;
	  }
	  i++;
  }

  std::cout << "Valoare maxima : "<< val << std::endl;
  std::cout << "Vectorul sortat: "<< v << std::endl;
  /* Afisam valoarea maxima transportabila de catre camion. */
  system("PAUSE");
  return 0;
}