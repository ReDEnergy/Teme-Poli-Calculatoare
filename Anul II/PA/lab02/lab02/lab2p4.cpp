#include <iostream>
#include <vector>
#include <algorithm>

#include "VectorIO.h"
#include "PairIO.h"

#define UNDEF -1
using namespace std;

/* Un material este o pereche (greutate, valoare). */
typedef std::pair<int, int> Mobila;

int val[100];

int val_max(int t, std::vector<Mobila>& v)
{
	/* TODO: Caclulati valoarea maxima transportabila de catre camionul de
	* capacitate t. */
	int i,j,asd=0;


	for (i=0; i<v.size(); i++) {
		for (j=t; j>0; j--) {
			if ((j - v[i].first) >= 0)
				val[j] = max(val[j], val[j - v[i].first] + v[i].second);
				asd = max( asd, val[j]);
		}
	}
	return asd;
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

	system("PAUSE");
  return 0;
}

