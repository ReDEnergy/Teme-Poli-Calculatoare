#ifndef __NIM_H__
#define __NIM_H__

#include <iostream>
#include <vector>

class NimGameConf {
  std::vector<int> heaps;
  int n;

 public:
  NimGameConf(int n) : heaps(n + 1, 0), n(n) {
    heaps[n] = 1;
  }

  /*! \brief Functie care intoarce dimensiunea celei mai mari gramezi din joc.
   * \return Dimensiunea celei mai mari gramezi din joc (cea initiala).
   */
  size_t size() {
    return n;
  }

  /*! \brief Functie care spune daca jocul s-a terminat. Jocul se termina
   * atunci cand nu mai exista nici o gramada cu cel putin trei pietricele.
   * \return <b>true</b> daca jocul s-a terminat, sau <b>false</b> altfel.
   */
  bool gameOver() {
    for (unsigned int i = 3; i < heaps.size(); ++i) {
      if (heaps[i]) {
        return false;
      }
    }

    return true;
  }

  /*! \brief Operator care permite accesul la orice gramada din joc. */
  int& operator[] (const unsigned int index) {
    return heaps[index];
  }

  /*! \brief Functie care imparte o gramada in alte doua gramezi mai mici.
   * \param heap Gramada ce trebuie impartita.
   * \param a Dimensiunea primei gramezi.
   * \param b Dimensiunea celei de-a doua gramezi.
   */
  void split(int heap, int a, int b) {
    if (heap > 2 && heap <= n && heaps[heap] > 0 && a > 0 && b > 0 &&
        heap == a + b) {
      heaps[heap]--;
      heaps[a]++;
      heaps[b]++;
    }
  }

  /*! \brief Inversul functiei split. Reasambleaza doua gramezi in gramada
   * originala. Nu este o miscare valida de gameplay, dar poate fi utila la
   * expandarea arborelui.
   */
  void unsplit(int heap, int a, int b) {
    if (heap > 2 && heap <= n && a > 0 && b > 0 &&
        heaps[a] > 0 && heaps[b] > 0 && heap == a + b) {
      heaps[heap]++;
      heaps[a]--;
      heaps[b]--;
    }
  }

  friend std::ostream& operator<< (std::ostream&, const NimGameConf&);
};

#endif
