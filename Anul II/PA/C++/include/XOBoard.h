#ifndef __XO_H__
#define __XO_H__

#pragma once

#include <iostream>
#include <fstream>

#define PACK(x,y) ((x)*3+(y))

class XOBoard{

 private:
  int movesX, movesO, moves;

  static const int winConfigs[8];

 public:

  enum Player{
    PlayerX,
    PlayerO
  };

  /** \brief Constructor fara parametri care instantiaza o tabla goala pe
   * care nu este marcat nimic. */
  XOBoard() : movesX(0), movesO(0), moves(0) { }

  /** \brief Metoda care verifica daca o celula este libera sau nu.
   * \param x Linia X de pe tabla (intre 0 si 2)
   * \param y Coloana Y de pe tabla (intre 0 si 2)
   * \return Functia intoarce <b>true</b> daca celula este libera sau
   * <b>false</b> daca celula este completata.
   */
  inline bool is_empty(int x, int y) const {
    return (moves & (1 << PACK(x,y))) ? false : true;
  }

  /** \brief Metoda care intoarce caracterul de pe tabla
   * \param x Linia X de pe tabla (intre 0 si 2)
   * \param y Linia Y de pe tabla (intre 0 si 2)
   * \return Dupa caz, intoarce </b>'X'</b>, <b>'O'</b> sau <b>'_'</b>
   */
  inline char get(int x, int y) const {
    if (x < 0 || x > 2 || y < 0 || y > 2) {
      std::cerr << "ERROR! The cell at (" << x << "," << y
          << ") does not exist!\n   Ignoring GET operation!";
      return '\0';
    } else {
      return (moves & (1 << PACK(x,y))) == 0 ?
          '_' :
          ((movesX & (1 << PACK(x,y))) == 0 ? 'O' : 'X');
    }
  }

  /** \brief Metoda care bifeaza o celula de pe tabla in numele unui jucator
   * \param player Jucatorul in numele caruia se bifeaza
   * (valori constante din clasa XOBoard)
   * \param x Linia X de pe tabla (intre 0 si 2)
   * \param y Linia Y de pe tabla (intre 0 si 2)
   */
  inline void put(Player player, int x, int y) {
    if (x < 0 || x > 2 || y < 0 || y > 2) {
      std::cerr << "ERROR! The cell at (" << x << "," << y
          << ") does not exist!\n   Ignoring PUT operation!";
    } else {
      moves |= (1 << (PACK(x,y)));
      if (player == PlayerX){
        movesX |= (1 << (PACK(x,y)));
      } else {
        movesO |= (1 << (PACK(x,y)));
      }
    }
  }

  /** \brief Metoda care sterge valoarea dintr-o celula de pe tabla.
   * \param x Linia X de pe tabla (intre 0 si 2)
   * \param y Linia Y de pe tabla (intre 0 si 2)
   */
  inline void erase(int x, int y) {
    if (x < 0 || x > 2 || y < 0 || y > 2) {
      std::cerr << "ERROR! The cell at (" << x << "," << y
          << ") does not exist!\n   Ignoring ERASE operation!";
    } else {
      moves &= ~(1 << (PACK(x,y)));
      movesX &= ~(1 << (PACK(x,y)));
      movesO &= ~(1 << (PACK(x,y)));
    }
  }

  /** \brief Metoda care spune daca tabla s-a terminat de completat sau nu 
   * \return Functia intoarce <b>true</b> daca tabla este completata la maxim
   * sau <b>false</b> altfel.
   */
  inline bool is_full() const {
    return (moves & 0x1FF) == 0x1FF;
  }

  /** \brief Metoda care intoarce scorul tablei din perspectiva jucatorului
   * dat ca parametru.
   * \param player Jucatorul din perspectiva caruia se calculeaza scorul.
   * \return Functia intoarce numarul de linii, coloane si diagonale complete
   * ale jucatorului dat ca parametru.
   */
  inline int get_score(Player player) const {
    int myMoves  = (moves & (player == PlayerX ? movesX : movesO ));
    int score = 0;
    for (int i = 0; i < 8; i++){
      if ((myMoves & winConfigs[i]) == winConfigs[i]) {
        score++;
      }
    }
    return score;
  }

  /** \brief Metoda care spune daca jocul s-a terminat. 
   * \return Functia intoarce <b>true</b> daca jocul s-a terminat sau
   * <b>false</b> daca se poate juca in continuare.
   */
  inline bool game_over() const {
    return is_full();
  }

  friend std::ostream& operator<< (std::ostream&, const XOBoard&);
};

class XOGame{

 private:
  typedef XOBoard ComputerAI(XOBoard::Player, XOBoard);

  ComputerAI * computerAI;

  typedef XOBoard (XOGame::*PlayerInterface)(XOBoard::Player, XOBoard);

  XOBoard humanInterface(XOBoard::Player player, XOBoard board);
  XOBoard computerInterface(XOBoard::Player player, XOBoard board);

  inline void gameEngine(XOBoard board,
                         PlayerInterface interfacePlayerX,
                         PlayerInterface interfacePlayerO) {
    std::cout << "Start Game:" << std::endl << std::endl << board;
    do {
      board = (this->*interfacePlayerX)(XOBoard::PlayerX, board);
      if (board.game_over()) return;
      board = (this->*interfacePlayerO)(XOBoard::PlayerO, board);
      if (board.game_over()) return;
    } while(1);
  }

 public:
  /** \brief Constructor care instantiaza un joc in care AI-ul calculatorului
   * gandeste folosindu-se de functia data ca parametru.
   * \param computerAI Functia pe care o va apela engine-ul de joc cand pune
   * calculatorul sa gandeasca. <b><u>TREBUIE</u></b> sa respecte urmatoarea
   * semnatura: XOBoard ComputerAI(XOBoard::Player, XOBoard). Functia data ca
   * parametru poate fi si <b>NULL</b>, dar in acest caz nu veti putea sa
   * porniti jocul decat folosind metoda <b>humanVShuman()</b>.
   */ 
  XOGame(ComputerAI computerAI) : computerAI(computerAI) { }

  /** \brief Lanseaza in executie un joc intre un om si un calculator.
   * Omul muta primul.
   */
  inline void humanVScomputer() {
    if (computerAI == NULL) {
      std::cout << "ERROR! You can't launch a Human VS Computer game"
          << " without a game AI!\n";
    } else {
      gameEngine(XOBoard(),
                 &XOGame::humanInterface,
                 &XOGame::computerInterface);
    }
  }

  /** \brief Lanseaza in executie un joc intre un calculator si un om.
   * Calculatorul muta primul.
   */
  inline void computerVShuman() {
    if (computerAI == NULL) {
      std::cout << "ERROR! You can't launch a Computer VS Human game without"
          << "a game AI!\n";
    } else {
      gameEngine(XOBoard(),
                 &XOGame::computerInterface,
                 &XOGame::humanInterface);
    }
  }

  /** \brief Lanseaza in executie un joc intre doi oameni.
   */
  inline void humanVShuman() {
    gameEngine(XOBoard(),
               &XOGame::humanInterface,
               &XOGame::humanInterface);
  }

  /** \brief Lanseaza in executie un joc intre doua calculatoare.
   * Amandoua gandesc cu acelasi AI. Trebuie sa obtineti mereu remiza daca ati
   * implementat optim si exhaustiv.
   */
  inline void computerVScomputer() {
    if (computerAI == NULL) {
      std::cout << "ERROR! You can't launch a Computer VS Computer game"
          << " without a game AI!\n";
    } else {
      gameEngine(XOBoard(),
                 &XOGame::computerInterface,
                 &XOGame::computerInterface);
    }
  }
};

std::ostream& operator<< (std::ostream&, const XOBoard&);

#endif

