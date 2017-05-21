#ifndef __POINT_H__
#define __POINT_H__

#include <iostream>
#include <fstream>

struct Point {
  /* Un punct este caracterizat de coordonatele
   * sale si clusterul din care face parte. */
  float x, y;
  int cluster;

  Point() : cluster(-1) { }

  /* Adunarea a 2 puncte. */
  inline Point operator+ (const Point& other) const {
    Point result;
    result.x = this->x + other.x;
    result.y = this->y + other.y;
    return result;
  }

  /* Inmultirea unui punct cu un scalar. */
  Point operator* (float scalar) const {
    Point result;
    result.x = this->x * scalar;
    result.y = this->y * scalar;
    return result;
  }
};

std::ostream& operator<< (std::ostream& out, const Point& point);
std::istream& operator>> (std::istream& in, Point& point);

#endif

