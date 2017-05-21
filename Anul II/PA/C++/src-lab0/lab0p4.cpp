#include <iostream>
#include <vector>
#include <algorithm>

#include "VectorIO.h"
#include "PairIO.h"

typedef std::pair<float,float> Point;

int main()
{
  /* Declaram un vector de puncte si il citim. */
  std::vector<Point> v;
  std::cin >> v;

  /* Il sortam. */
  std::sort(v.begin(), v.end());

  /* Si il afisam. */
  std::cout << v << std::endl;

  return 0;
}

