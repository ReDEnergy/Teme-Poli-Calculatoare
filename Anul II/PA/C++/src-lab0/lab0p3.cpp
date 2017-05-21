#include <iostream>
#include <queue>

#include "VectorIO.h"

int main()
{
  /* Declaram un vector de numere si il citim de la tastatura. */
  std::vector<float> v;
  std::cin >> v;

  /* Declaram un min-heap. */
  std::priority_queue<float, std::vector<float>, std::greater<float> > pq;

  /* Umplem min-heap-ul cu elementele din vector. */
  for (unsigned int i = 0; i < v.size(); ++i) {
    pq.push(v[i]);
  }

  /* Scoatem elementele din min-heap si le punem din nou in vector. */
  v.clear();
  while (!pq.empty()) {
    float element = pq.top();
    pq.pop();
    v.push_back(element);
  }

  /* Afisam elementele sortate. */
  std::cout << v << std::endl;

  return 0;
}

