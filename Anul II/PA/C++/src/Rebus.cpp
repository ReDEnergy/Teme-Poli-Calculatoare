#include "Rebus.h"

std::ostream& operator<< (std::ostream& out, const Rebus& rebus)
{
  out << std::string(2 * rebus.columns - 1, '-') << std::endl;
  for (unsigned int i = 0; i < rebus.rows; ++i) {
    for (unsigned int j = 0; j < rebus.columns; ++j) {
      out << (j == 0 ? "" : " ") << rebus.m[i][j];
    }
    out << std::endl;
  }        
  return out << std::string(2 * rebus.columns - 1, '-') << std::endl;
}

std::istream& operator>> (std::istream& in, Rebus& rebus)
{
  in >> rebus.rows >> rebus.columns;
  rebus.m = new char*[rebus.rows];
  rebus.ref = new char*[rebus.rows];
  for (unsigned int i = 0; i < rebus.rows; ++i) {
    rebus.m[i] = new char[rebus.columns];
    rebus.ref[i] = new char[rebus.columns];
    for (unsigned int j = 0; j < rebus.columns; ++j) {
      in >> rebus.ref[i][j];
      rebus.m[i][j] = (rebus.ref[i][j] == '*') ? '*' : '_';
    }
  }
  return in;
}

