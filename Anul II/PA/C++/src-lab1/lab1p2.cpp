#include <iostream>

#include "VectorIO.h"

int partitioneaza(std::vector<int>& v, int begin, int end)
{
  /* Exista mai multe metode de a partitiona vectorul.
   * Voi implementa una oarecare
   */

  /* Pivotul il pun initial in stanga si sa stea pe loc. */
  int left = begin, left_inc = 0;

  /* Compar progresiv cu elementele de la sfarsit spre inceput. */
  int right = end, right_inc = -1;

  int aux;
  while (left < right){
    if (v[left] > v[right]){
      /* Pivotul nu se afla in pozitia corecta fata de elementul cu care il
       * compar, inseamna ca pivotul trebuie interschimbat cu acel element
       * (ca sa le aducem in ordinea corecta).
       */
      aux = v[left]; 
      v[left] = v[right];
      v[right] = aux;
      /* De acum incolo, comparatiile decurg in sens invers, pentru ca pivotul
       * sta pe loc si ne apropiem cu capatul celalalt de el. */
      aux = left_inc;
      left_inc = -right_inc;
      right_inc = -aux;      
    }
    /* Pivotul se va incrementa cu 0 (va sta pe loc, indiferent in care capat a
     * ajuns), iar celalalt capat, care nu este pivotul, se va apropia de pivot
     * cu o unitate (+1 sau -1, depinde cate interschimbari am facut pana acum. */
    left = left + left_inc;
    right = right + right_inc;
  }

  /* In final, intoarcem pozitia pe care a ajuns poviotul. */
  return left;
}

/* Pentru a explica mai bine cum functioneaza partitionarea, voi lua si un exemplu:

   Voi pune pivotul in paranteze [patrate], iar capatul opus in paranteze <unghiulare>

   Pas 1: initial, pivotul e in stanga, capatul opus e in dreapta
   [4] 7 3 8 2 5 <0>

   Pas 1-2: nu respecta relatia de ordine, le interschimb
   <0> 7 3 8 2 5 [4]

   Pas 2: avansez cu capatul liber la urmatorul element in directia pivotului
   0 <7> 3 8 2 5 [4]

   Pas 2-3: nu respecta relatia de ordine, le interschimb
   0 [4] 3 8 2 5 <7>

   Pas 3: avansez cu capatul liber la urmatorul element in directia pivotului
   0 [4] 3 8 2 <5> 7

   Pas 4: avansez cu capatul liber la urmatorul element in directia pivotului
   0 [4] 3 8 <2> 5 7 

   Pas 4-5: nu respecta relatia de ordine, le interschimb
   0 <2> 3 8 [4] 5 7

   Pas 5: avansez cu capatul liber la urmatorul element in directia pivotului
   0 2 <3> 8 [4] 5 7

   Pas 6: avansez cu capatul liber la urmatorul element in directia pivotului
   0 2 3 <8> [4] 5 7

   Pas 6-7: nu respecta relatia de ordine, le interschimb
   0 2 3 [4] <8> 5 7

   Pas 7: avansez cu capatul liber la urmatorul element in directia pivotului
   0 2 3 <[4]> 8 5 7

   In acest moment, pivotul 4 se suprapune cu capatul liber. Tot ce e in stanga lui
   a ajuns mai mic, iar tot ce e in dreapta lui a ajuns mai mare. Sigur se afla pe
   pozitia lui finala in vector (daca am sorta acum, 4 ar ramane pe aceeasi
   pozitie).
 */

int find_kth_smallest(std::vector<int>& v, int k, int begin, int end)
{
  /* Ordonam partial vectorul intre begin si end astfel incat sa obtinem o
   * pozitie numita pivot cu proprietatea ca tot ce se afla in stanga pivotului
   * este mai mic decat el si tot ce se afla in dreapta pivotului este mai mare
   * decat el. Pivotul se afla pe aceeasi pozitie pe care s-ar afla si daca am
   * fi sortat vectorul.
   */
  int pivot = partitioneaza(v, begin, end);

  if (pivot == k){
    /* Elementul cautat este chiar pivotul, care se afla pe aceeasi pozitie
     * daca am sorta vectorul, deci am aflat raspunsul la problema.
     */
    return v[pivot];
  } else if (pivot < k){
    /* Inseamna ca trebuie sa fie undeva in dreapta. */
    return find_kth_smallest(v, k, pivot+1, end);
  } else {
    /* Altfel, e undeva in stanga. */
    return find_kth_smallest(v, k, begin, pivot-1);
  }
}

int find_kth_smallest(std::vector<int>& v, int k)
{
  /* Vom aplica metoda divide et impera. Daca am putea gasi o metoda de a
   * imparti vectorul in doua jumatati [oarecum] apropiate de dimensiune,
   * astfel incat in stanga lui sa avem doar elemente mai mici decat mijlocul,
   * iar in dreapta lui sa avem doar elemente mai mari decat mijlocul, atunci
   * in functie de pozitia acestui pseudo-mijloc fata de pozitia k pe care o
   * cautam, putem reface cautarea recusiv doar pe o portiunea din vector.
   *
   * Vom folosi in acest sens, functia de partitionare de la quicksort.
   */
  return find_kth_smallest(v, k, 0, v.size() - 1);
}

int main()
{
  /* Declaram si citim un vector de numere intregi de la tastatura. */
  std::vector<int> v;
  std::cin >> v;

  /* Afisam elementul care se afla pe pozitia mediana in vectorul sortat. */
  std::cout << "Elementul median din multime este: "
      << find_kth_smallest(v, v.size()/2) << std::endl;

  return 0;
}

