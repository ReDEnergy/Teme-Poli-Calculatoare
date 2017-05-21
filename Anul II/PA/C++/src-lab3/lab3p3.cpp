#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "VectorIO.h"

void greedy_format(unsigned int l, std::vector<std::string>& v)
{
  std::cout << "Greedy format:" << std::endl;
  unsigned int cost = 0;
  unsigned int line_fill = 0;

  /* Parcurgem textul si afisam cuvintele linie pe linie. */
  for (unsigned int i = 0; i < v.size(); ++i) {
    if (l - line_fill >= v[i].length() + (line_fill == 0 ? 0 : 1)) {
      /* Mai avem loc pe linie, afisam cuvantul si eventual si un spatiu. */
      std::cout << (line_fill == 0 ? "" : " ") << v[i];
      line_fill += v[i].length() + (line_fill == 0 ? 0 : 1);
    } else {
      /* Nu mai avem loc pe linie, trebuie sa punem | si sa trecem la linia
       * urmatoare. */
      std::cout << std::string(l - line_fill, ' ') << "|" << std::endl << v[i];
      cost += (l - line_fill) * (l - line_fill);
      line_fill = v[i].length();
    }
  }

  /* Inchidem si ultima linie si actualizam costul. */
  cost += (l - line_fill) * (l - line_fill);
  std::cout << std::string(l - line_fill, ' ') << "|" << std::endl
      << "TOTAL COST: " << cost << std::endl;
}

void tex_format(unsigned int l, std::vector<std::string>& v)
{
  std::cout << "Tex format:" << std::endl;

  /* Daca ne dorim sa minimizam costul, nu mai putem folosi un algoritm in O(N)
   * deoarece suma de patrate perfecte nu este o functie optimizabila Greedy.
   *
   * In schimb ne vom folosi de o recurenta PD. Sa pornim cu urmatoarea
   * constatare: daca am considera ca word[i] este primul cuvant de pe linia sa,
   * pentru un "i" oarecare, costul minim care se poate obtine pentru textul
   * format din cuvintele:
   *      word[i] word[i+1] ... word[n-1]
   * este:
   * cost_optim[i] = MIN{ line_padding(word[i] word[i+1] ... word[k]) +
   *                      cost_optim[k + 1]
   *                 }, unde i <= k < n
   *
   * Cu alte cuvinte, putem sa incercam sa plasam inca 0, 1, 2, 3, ... etc.
   * cuvinte pe linie cu word[i] si sa adunam costul respectivei linii cu costul
   * optim al formatarii textului care incepe de la cuvantul imediat urmator.
   *
   * Cum word[0] va fi mereu primul cuvant de pe linia sa si textul incepe cu
   * word[0], solutia problemei va fi data cost_optim[0]. Completarea vectorului
   * se face de la sfarsit spre inceput.
   *
   * Pentru a reconstitui impartirea textului, vom pastra inca un vector
   * line_break[i] = indexul cuvantului unde incepe o linie noua in formatarea
   *                 optima, daca word[i] este primul cuvant de pe linia sa.
   */

  int cost_optim[v.size() + 1];
  int line_break[v.size() + 1];
  memset(cost_optim, 0x00, (v.size() + 1) * sizeof(int));
  memset(line_break, 0x00, (v.size() + 1) * sizeof(int));

  /* Dupa ultimul cuvant nu mai poate urma nimic. */
  cost_optim[v.size() - 1] = (l - v[v.size() - 1].length()) *
      (l - v[v.size() - 1].length());
  line_break[v.size() - 1] = v.size();

  /* Completam de la sfarsit spre inceput. */
  for (int i = v.size() - 2; i >= 0; --i){
    /* Pornim de la prezumtia ca lasam doar word[i] pe linia sa. */
    unsigned int j = i + 1;
    unsigned int line_fill = v[i].length();
    cost_optim[i] = (l - line_fill) * (l - line_fill) + cost_optim[j];
    line_break[i] = j;

    /* Dar incercam si variantele in care adaugam cu el pe linie pe (i + 1),
     * (i + 2), ... etc. pana se umple linia. */
    for (j = i + 2;
         j < v.size() + 1 && line_fill + 1 + v[j - 1].length() <= l;
         ++j) {
      line_fill += 1 + v[j - 1].length();
      if (cost_optim[i] > (l - line_fill) * (l - line_fill) + cost_optim[j]) {
        cost_optim[i] = (l - line_fill) * (l - line_fill) + cost_optim[j];
        line_break[i] = j;
      }
    }
  }

  /* Printam textul formatat, din line break in line break. */
  for (int i = 0; i != v.size(); i = line_break[i]) {
    unsigned int line_fill = 0;
    /* Afisam cuvintele de pe linie si calculam lungimea totala. */
    for (int j = i; j < line_break[i] && j < v.size(); ++j) {
      std::cout << (j == i ? "" : " ") << v[j];
      line_fill += (j == i ? 0 : 1) + v[j].length();
    }
    /* Si inchidem linia cu "|". */
    std::cout << std::string(l - line_fill, ' ') << "|" << std::endl;
  }

  /* Afisam costul total. */
  std::cout << "TOTAL COST: " << cost_optim[0] << std::endl;
}

int main()
{
  /* Declaram si citim lungimea unei linii si un vector de cuvinte. */
  unsigned int l;
  std::vector<std::string> word;
  std::cin >> l >> word;

  /* Verificam sa nu avem cuvinte mai lungi de o linie. */
  for (unsigned int i = 0; i < word.size(); ++i) {
    if (word[i].length() > l) {
      std::cerr << "Imposibil. Anumite cuvinte sunt mai lungi de o linie!"
          << std::endl;
      return 0;
    }
  }

  /* Afisam impartirea greedy. Se vor afisa L caractere pe o linie, urmate de un
   * caracter pipe ('|') la final. */
  greedy_format(l, word);
  std::cout << std::endl;

  /* Afisam impartirea folosind functia de cost din TeX. Se vor afisa L
   * caractere pe o linie, urmate de un caracter pipe ('|') la final.*/
  tex_format(l, word);

  return 0;
}

