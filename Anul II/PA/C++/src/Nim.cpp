#include <iostream>
#include <vector>

#include "Nim.h"

std::ostream& operator<< (std::ostream& out, const NimGameConf& game) {
  out << "Gramezile:" << std::endl << "\t";
  for (unsigned int i = 1; i < 3; ++i) {
    for (int size = game.heaps[i]; size > 0; --size) {
      out << " " << "(" << i << ")";
    }
  }
  for (unsigned int i = 3; i < game.heaps.size(); ++i) {
    for (int size = game.heaps[i]; size > 0; --size) {
      out << " " << i;
    }
  }
  return out << std::endl;
}

