#ifndef __SUDOKUBOARD_H__
#define __SUDOKUBOARD_H__

#pragma once

#include <iostream>
#include <fstream>

class SudokuBoard{
 private:
  class BitSet{
   private:

    int mem;

   public:
    BitSet() : mem((9 << 16) | (0x000003FEl)) { }

    inline int size(){
      return mem >> 16;
    }

    inline bool allows(int i){
      return (mem & (1 << i));
    }

    inline void erase(int i){
      mem &= (~(1 << i));
      mem -= (1 << 16);
    }

    inline void reduce_to(int i){
      mem = (1 << 16) | (1 << i);
    }

    friend class SudokuBoard;
  };

  int filled_in;

  char fill[9][9];

  BitSet row_stats[9];
  BitSet col_stats[9];
  BitSet reg_stats[9];

 public:
  SudokuBoard();

  /** \brief Functie care spune daca pentru linia <b>row</b> si coloana <b>col</b> din careu nu mai avem nici o posibilitate de completare.
   *  \param row Linia pentru care verificam
   *  \param col Coloana pentru care verificam
   *  \return Functia intoarce <b>true</b> daca este imposibil sa mai completezi celula de coordonate (row,col)
   */
  inline bool impossible(int row, int col){
    return 0 == (0x000003FEl & row_stats[row].mem & col_stats[col].mem & reg_stats[row/3*3+col/3].mem);
  }

  /** \brief Functie care verifica daca nu cumva pentru linia <b>row</b> si coloana <b>col</b> nu exista decat o singura metoda de completare.
   *  \param row Linia pentru care verificam
   *  \param col Coloana pentru care verificam
   *  \return Functia intoarce o cifra nenula intre 1 si 9 daca exista o singura varianta de completare, si 0 daca exista mai multe variante sau nici o varianta.
   */  
  inline int unique_possibility(int row, int col){
    int sol = 0;
    for (int i = 1; i <= 9; i++){
      if (allows(row, col, i)){
        if (sol == 0){
          sol = i;
        } else {
          return 0;
        }
      }
    }
    return sol;
  }

  /** \brief Functie care verifica daca este posibil sa se completeze valoarea <b>i</b> la celula de coordonate <b>row</b>, <b>col</b>
   *  \param row Linia pentru care verificam
   *  \param col Coloana pentru care verificam
   *  \param i Valoarea despre care intrebam daca este permisibila
   *  \return Functia intoarce <b>true</b> daca completarea este corecta.
   */  
  inline bool allows(int row, int col, int i){
    return row_stats[row].allows(i) && col_stats[col].allows(i) && reg_stats[row/3*3+col/3].allows(i);
  }

  /** \brief Functie care completeaza o valoare intr-o celula din careu. <b>ATENTIE!</b> Este responsabilitatea voastra sa va asigurati ca nu suprascrieti o valoare deja pusa acolo!
   *  \param row Linia pentru care verificam
   *  \param col Coloana pentru care verificam
   *  \param i Valoarea pe care dorim sa o trecem in celula
   */
  inline void put(int row, int col, int i){
    if (!is_empty(row,col)){
      std::cerr << "ERROR! You must never overwrite values in the Sudoku board!\n";
      std::cerr << "   Make a copy of the board so you can go back to a previous version!\n";
      std::cerr << "   Ignoring \"put\" operation!\n";
      return;
    }
    fill[row][col] = '0' + i;
    row_stats[row].erase(i);
    col_stats[col].erase(i);
    reg_stats[row/3*3+col/3].erase(i);
    filled_in++;
  }

  /** \brief Functie care verifica daca o celula din careu nu a fost completata 
   *  \return Functia intoarce <b>true</b> daca celula respectiva este libera si <b>false</b> altfel.
   */  
  inline bool is_empty(int row, int col){
    return fill[row][col] == 0;
  }

  /** \brief Functie care verifica daca un careu s-a terminat de completat
   *  \return Functia intoarce <b>true</b> daca s-a terminat completarea careului.
   */
  inline bool is_done(){
    return filled_in == 81;
  }

  friend std::ostream& operator<< (std::ostream& out, SudokuBoard& right);
};

/** \brief Operator care citeste o grila de Sudoku dintr-un stream de intrare */
std::istream& operator>> (std::istream& in, SudokuBoard& right);

/** \brief Operator care scrie o grila de Sudoku intr-un stream de iesire */
std::ostream& operator<< (std::ostream& out, SudokuBoard& right);

#endif

