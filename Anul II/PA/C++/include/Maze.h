#ifndef __MAZE_H__
#define __MAZE_H__

#pragma once

#include <iostream>
#include <fstream>

typedef std::pair<int, int> Coord;

class Maze {
 private:
  int height, width;
  char** cell;

  void allocate() {
    cell = new char*[height];
    for (int i = 0; i < height; i++) {
      cell[i] = new char[width];
      for (int j = 0; j < width; j++) {
        cell[i][j] = '#';
      }
    }
  }

 public:

  /**
   * \brief Empty constructor.
   */
  Maze() : height(0), width(0), cell(NULL) {
  }

  /** 
   * \brief Constructor.
   * \param width Latimea labirintului (nr. coloane)
   * \param height Inaltimea labirintului (nr. linii)
   */
  Maze(int height, int width) : height(height), width(width) {
    if (height > 3 && width > 3) {
      allocate();
    } else {
      this->height = 0;
      this->width = 0;
    }
  }

  ~Maze() {
    if (cell) {
      for (int i = 0; i < height; i++) {
        delete[] cell[i];
      }
      delete[] cell;
    }
  }

  /** 
   * \brief Functie pentru determinarea latimii labirintului.
   * \return Latimea labirintului
   */
  inline int get_width() {
    return width;
  }

  /** 
   * \brief Functie pentru determinarea inaltimii labirintului.
   * \return Inaltimea labirintului
   */
  inline int get_height() {
    return height;
  }

  /**
   * \brief Functie care spune daca o celula din labirint este accesibila.
   * \param line Linia din labirint.
   * \param column Coloana din labirint.
   * \return Functia intoarce <b>true</b> daca celula din labirint de la (line,
   * column) este accesibila, sau <b>false</b> daca respectiva celula este zid.
   */
  inline bool is_walkable(int line, int column) {
    if (line >= 0 && line < height && column >= 0 && column < width) {
      return cell[line][column] != '#';
    } else {
      return false;
    }
  }

  /**
   * \brief Functie care spune daca o celula din labirint este accesibila.
   * \param coord Linia si coloana din labirint.
   * \return Functia intoarce <b>true</b> daca celula din labirint de la coord
   * este accesibila, sau <b>false</b> daca respectiva celula este zid.
   */
  inline bool is_walkable(Coord coord) {
    return is_walkable(coord.first, coord.second);
  }

  /**
   * \brief Functie care spune daca o celula din labirint este "colt".
   * \param line Linia din labirint.
   * \param column Coloana din labirint.
   * \return Functia intoarce <b>true</b> daca celula din labirint de la (line,
   * column) este colt, sau <b>false</b> altfel.
   */
  inline bool is_corner(int line, int column) {
    int i = line;
    int j = column;
    /* E colt spre NE daca sunt walkable celulele de la N, NE si E. */
    if (is_walkable(i - 1, j) &&
        is_walkable(i - 1, j + 1) &&
        is_walkable(i, j + 1)) {
      return true;
    }
    /* E colt spre SE daca sunt walkable celulele de la E, SE si S. */
    if (is_walkable(i, j + 1) &&
        is_walkable(i + 1, j + 1) &&
        is_walkable(i + 1, j)) {
      return true;
    }
    /* E colt spre SV daca sunt walkable celulele de la S, SV si V. */
    if (is_walkable(i + 1, j) &&
        is_walkable(i + 1, j - 1) &&
        is_walkable(i, j - 1)) {
      return true;
    }
    /* E colt spre NV daca sunt walkable celulele de la V, NV si N. */
    if (is_walkable(i, j - 1) &&
        is_walkable(i - 1, j - 1) &&
        is_walkable(i - 1, j)) {
      return true;
    }

    /* Altfel nu e colt. */
    return false;
  }

  /**
   * \brief Functie care spune daca o celula din labirint este punct de iesire.
   * \param line Linia din labirint.
   * \param column Coloana din labirint.
   * \return Functia intoarce <b>true</b> daca celula din labiring de la (line,
   * column) este acceisbila, sau <b>false</b> daca respectiva celula este zid.
   */
  inline bool is_exit_point(int line, int column) {
    if (line >= 0 && line < height && column >= 0 && column < width) {
      return is_walkable(line, column) &&
          (line == 0 || line == height - 1 || column == 0 || column == width - 1);
    } else {
      return false;
    }
  }

  /**
   * \brief Functie care spune daca o celula din labirint este punct de iesire.
   * \param coord Linia si coloana din labirint.
   * \return Functia intoarce <b>true</b> daca celula din labiring de la coord
   * este acceisbila, sau <b>false</b> daca respectiva celula este zid.
   */
  inline bool is_exit_point(Coord coord) {
    return is_exit_point(coord.first, coord.second);
  }

  /**
   * \brief Functie care marcheaza un pas din solutie.
   * \param line Linia din labirint.
   * \param column Coloana din labirint.
   */
  inline void mark_solution_step(int line, int column) {
    if (line >= 0 && line < height && column >= 0 && column < width) {
      cell[line][column] = ' ';
    }
  }

  /**
   * \brief Functie care marcheaza un pas din solutie.
   * \param coord Linia si coloana din labirint.
   */
  inline void mark_solution_step(Coord coord) {
    mark_solution_step(coord.first, coord.second);
  }

  friend std::istream& operator>> (std::istream&, Maze& maze);
  friend std::ostream& operator<< (std::ostream&, const Maze&);
};

#endif

