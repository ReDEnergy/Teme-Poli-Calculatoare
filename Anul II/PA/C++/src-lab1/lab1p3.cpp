#include <iostream>

#include "VectorIO.h"

#define MAX(a,b) ((a)<(b)?(b):(a))

/* Chiar daca solutia optima se poate obtine folosind un greedy iterativ in
 * O(N), pentru a exemplifica Divide et Impera vom folosi o abordare top-down.
 * (Aceasta solutie are complexitatea O(3N) si este recursiva)
 *
 *   A) Impartim vectorul initial in doua jumatati [aproximativ] egale.
 *
 *   B) Aflam pentru fiecare dintre jumatati:
 *      1) subsecventa de suma maxima lipita de capatul din drta al jumatatii
 *      2) subsecventa de suma maxima lipita de capatul din stga al jumatatii
 *      3) suma elementelor din respectiva jumatate
 *
 *   C) Dupa ce avem itemii 1)...3) calculati pentru fiecare jumatate, ii
 *      folosim ca sa asamblam aceleasi statistici pentru pentru subsecventa
 *      curenta. 
 *
 *   D) functia intoarce in acelasi timp si solutia pentru intervalul de vector
 *      pe care calculeaza.
 */

/* Vom face urmatoarele notatii:
 *
 *   max_ss_left = suma maxima a unei subsecvente care incepe din capatul din
 *                 stanga al portiunii analizate:
 *
 *    [max_ss_left) ..............
 *
 *   max_ss_right = suma maxima a unei subsecvente care incepe din capatul din
 *                  dreapta al portiunii analizate:
 *
 *    ............. (max_ss_right]
 *
 *   sum_ss = suma elem din portiunea analizata (care atinge ambele capete)
 *
 *    [<<<<<<<<< sum_ss >>>>>>>>>]
 */
int max_ss(std::vector<int>& v,
           int begin,
           int end,
           int& max_ss_left,
           int& max_ss_right,
           int& sum_ss)
{
  if (begin == end){

    /* 0) Cazul trivial: scanam o portiune de lungime 1. */
    return sum_ss = max_ss_left = max_ss_right = v[begin];

  } else {

    /* A) Impartim vectorul initial in doua jumatati aproximativ egale. */
    int m = (begin + end)/2;

    /* B) Calculam pentru jumatatea stanga, recursiv. */
    int max_ss_left_left, max_ss_right_left, sum_ss_left, best_ss_left;
    best_ss_left = max_ss(v, begin, m,
                          max_ss_left_left, max_ss_right_left, sum_ss_left);

    /* B) Calculam pentru jumatatea din dreapta, recursiv. */
    int max_ss_left_right, max_ss_right_right, sum_ss_right, best_ss_right;
    best_ss_right = max_ss(v, m+1, end,
                           max_ss_left_right, max_ss_right_right, sum_ss_right);

    /* C) Reasamblam din bucati valorile corecte la nivelul curent. */
    sum_ss = sum_ss_left + sum_ss_right;
    max_ss_right = MAX(max_ss_right_right, max_ss_right_left + sum_ss_right);
    max_ss_left  = MAX(max_ss_left_left,   sum_ss_left + max_ss_left_right);

    /* D) Tot din ce intorc apelurile recursive, gasim si solutia cea mai buna
     *    pe subsecventa analizata la nivelul curent
     */
    int return_value;

    /*    1) Presupunem ca subsecventa cea mai buna vine in intregime din jumatatea
     *       stanga sau dreapta :
     *        ... (best_ss_left) ... | ... (best_ss_right) ...
     */
    return_value = MAX(best_ss_left, best_ss_right);

    /*    2) Verificam daca nu cumva putem face o solutie si mai buna incalecand
     *       mijlocul cu secvente din stanga si din dreapta:
     *        ... (max_ss_right_left] | [max_ss_left_right) ...
     *
     *       Evident, secventele respective trebuie sa fie lipite de mijloc cu
     *       cate un capat.
     */
    return_value = MAX(return_value, max_ss_right_left + max_ss_left_right);

    /*    3) Alte cazuri nu mai sunt: ori este in intregime intr-una din
     *    jumatati, ori incaleca mijlocul.
     */
    return return_value;

  }
}

int max_sum_subsequence(std::vector<int>& v)
{
  int max_ss_left, max_ss_right, sum_ss;
  return max_ss(v, 0, v.size() - 1, max_ss_left, max_ss_right, sum_ss);
}

int main()
{
  /* Declaram si citim un vector de numere intregi. */
  std::vector<int> v;
  std::cin >> v;

  /* Afisam rezultatul. */
  std::cout << "Subsecventa de suma maxima din vector are suma = "
      << max_sum_subsequence(v) << std::endl;

  return 0;
}

