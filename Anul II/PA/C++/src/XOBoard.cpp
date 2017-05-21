#include <iostream>
#include <fstream>

#include "XOBoard.h"

const int XOBoard::winConfigs[8] = { 
  0x007, //< Prima linie
  0x038, //< A doua linie
  0x1C0, //< A treia linie
  0x049, //< Prima coloana
  0x092, //< A doua coloana
  0x124, //< A treia coloana
  0x054, //< Diagonala secundara
  0x111  //< Diagonala principala
};

std::ostream& operator<< (std::ostream& out, const XOBoard& right)
{
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 3; j++){
      out << (j == 0 ? "" : " ") << right.get(i,j);
    }
    out << "\n";
  }
  return out;
}

XOBoard XOGame::computerInterface(XOBoard::Player player, XOBoard board)
{
  std::cout << "Computer is thinking...\n";
  board = (*computerAI)(player, board);
  std::cout << "Computer finished move.\n\n" << board;
  /* Mark the move and return */
  if (board.is_full()) {
    XOBoard::Player other = (player == XOBoard::PlayerX) ?
        XOBoard::PlayerO : XOBoard::PlayerX;
    if (board.get_score(other) < board.get_score(player)){
      std::cout << "Player" << (player == XOBoard::PlayerX ? 'X' : 'O')
          << " wins!\n";
    } else if (board.get_score(player) < board.get_score(other)) {
      std::cout << "Player" << (other == XOBoard::PlayerX ? 'X' : 'O')
          << " wins!\n";
    } else {
      std::cout << "Tie. Nobody wins!\n";
    }
  }
  return board;
}

XOBoard XOGame::humanInterface(XOBoard::Player player, XOBoard board)
{
  int humanI, humanJ;
  /* Get a move from the human */
  do {
    std::cout << "Where do you want to place a mark? ";
    std::cin >> humanI >> humanJ;
    if (humanI < 0 || humanI > 2 || humanJ < 0 || humanJ > 2){
      std::cout << "ERROR! Coordinates must be in {0, 1, 2}.\nTry again: ";
    } else if (!board.is_empty(humanI, humanJ)){
      std::cout << "ERROR! Cell is not empty.\nTry again: ";
    } else {
      break;
    }
  } while (1);
  /* Mark the move and return */
  board.put(player, humanI, humanJ);
  std::cout << "\n" << board;
  if (board.is_full()) {
    XOBoard::Player other = (player == XOBoard::PlayerX) ?
        XOBoard::PlayerO : XOBoard::PlayerX;
    if (board.get_score(other) > board.get_score(player)){
      std::cout << "Player" << (other == XOBoard::PlayerX ? 'X' : 'O')
          << " wins!\n";
    } else if (board.get_score(player) > board.get_score(other)) {
      std::cout << "Human wins. Keep working on that AI :-\"...\n";
    } else {
      std::cout << "Tie. Nobody wins!\n";
    }
  }
  return board;
}

