#include "Maze.h"

std::ostream& operator<< (std::ostream& out, const Maze& maze)
{
  for (int i = 0; i < maze.height; i++) {
    for (int j = 0; j < maze.width; j++) {
      out << (char)maze.cell[i][j];
    }
    out << std::endl;
  }
  return out;
}

std::istream& operator>> (std::istream& in, Maze& maze)
{
  in >> maze.height >> maze.width;
  maze.allocate();
  std::string line;
  std::getline(in, line);
  for (int i = 0; i < maze.height; ++i) {
    std::getline(in , line);
    for (int j = 0; j < maze.width; ++j) {
      switch (line[j]) {
        case '.':
          maze.cell[i][j] = '.';
          break;
        case '#':
        default:
          maze.cell[i][j] = '#';
      }
    }
  }
  return in;
}

