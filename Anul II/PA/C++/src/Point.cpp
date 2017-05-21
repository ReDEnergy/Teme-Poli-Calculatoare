#include "Point.h"

std::ostream& operator<< (std::ostream& out, const Point& point) {
  return out << "(" << point.x << ", " << point.y
      << ", CLUSTER = " << point.cluster << ")";
}

std::istream& operator>> (std::istream& in, Point& point) {
  float x, y;
  in >> x >> y;
  point.x = x;
  point.y = y;
  return in;
}

