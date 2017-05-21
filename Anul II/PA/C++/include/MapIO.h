#ifndef __MAP_IO__
#define __MAP_IO__

#pragma once

#include <map>
#include <iostream>

template<typename T, typename V>
std::ostream& operator<< (std::ostream& out, std::map<T, V>& m)
{
  out << "{" << std::endl;
  for (typename std::map<T, V>::iterator it = m.begin(); it != m.end(); ++it) {
    out << "\t" << (it->first) << " : " << (it->second) << std::endl;
  }
  return out << "}" << std::endl;
}

#endif

