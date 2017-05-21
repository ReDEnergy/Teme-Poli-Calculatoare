
#include "SudokuBoard.h"

#include <cstring>

SudokuBoard::SudokuBoard() : filled_in(0)
{
  memset(fill, 0, 81);
}

std::istream& operator>> (std::istream& in, SudokuBoard& right)
{
  char x;
  for (int row = 0; row < 9; row++){
    for (int col = 0; col < 9; col++){
      do{
        in.get(x);
        if (!in.good()){
          std::cerr << "Error: Input file is incorrect!\n";
          return in;
        }
        if ('0' <= x && x <= '9') {
          right.put(row,col,x - '0');        
        }
      } while (x == ' ' || x == '\t' || x == '\n');
    }
  }
  return in;
}

std::ostream& operator<< (std::ostream& out, SudokuBoard& right)
{
  for (int i = 0; i < 9; i++){
    for (int j = 0; j < 9; j++){
      out << ((right.fill[i][j] == 0) ? '_' : right.fill[i][j]) << ((j == 2 || j == 5) ? " " : "") << " ";
    }
    out << "\n" << ((i == 2 || i == 5) ? "\n\n" : "");
  }
  return out;
}

