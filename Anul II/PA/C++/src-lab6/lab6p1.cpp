#include <iostream>
#include <fstream>
#include <queue>

#include "Maze.h"






typedef std::pair<int, int> Coord;
using namespace std;
const Coord NOWHERE(-1, -1);

const int dirLin[4] = { -1, 0, 1, 0 };
const int dirCol[4] = { 0, 1, 0, -1 };

void find_exit(Maze& maze, Coord source)
{
  /* Pentru a reconstitui drumul inainte de iesirea din functie,
   * vom folosi o matrice de parinti. */
  Coord parent[maze.get_height()][maze.get_width()];
  for (unsigned int i = 0; i < maze.get_height(); ++i) {
    for (unsigned int j = 0; j < maze.get_width(); ++j) {
      parent[i][j] = NOWHERE;
    }
  }

  /* TODO: Implementati o cautare care sa depisteze drumul cel mai scurt de la
   * pozitia "source" la o iesire din labiring (vezi functia "is_exit_point").
   *
   * Dupa ce ati calculat drumul, inainte de iesirea din functie, trebuie sa il
   * marcati celula cu celula folosind functia "mark_solution_step" din clasa
   * Maze. */

    queue < Coord > Labirint;
    Coord A;
	int success = 0;
	Labirint.push(source);
	maze.mark_solution_step(source);


    while (!Labirint.empty() && success == 0) {
        A = Labirint.front();
        Labirint.pop();
        unsigned int x = A.first;
        unsigned int y = A.second;
        Coord B;
		int j;
        for (j=0;j<4;j++) {
            B.first = x + dirLin[j];
            B.second = y + dirCol[j];

            if (maze.is_walkable(B) && parent[B.first][B.second] == NOWHERE) {
				parent[B.first][B.second] = A;

            	if (maze.is_exit_point(B)) {
					maze.mark_solution_step(B);
					while (B.first != source.first || B.second!=source.second) {
						maze.mark_solution_step(B);
						A = parent[B.first][B.second];
						B.first = A.first;
						B.second = A.second;
					}
					success = 1;
            	}
				Labirint.push(B);

            }
        }


    }
}


int main()
{
  /* Citim o harta din fisierul de intrare. */
  std::ifstream in("src-lab6/Labirint.txt");
  Maze maze;
  unsigned int lineTrudy, columnTrudy;
  in >> maze >> lineTrudy >> columnTrudy;

  /* Calculam pe ea drumul din labirint. */
  find_exit(maze, Coord(lineTrudy, columnTrudy));

  /* Si afisam drumul final. */
  std::cout << "Labirintul cu drumul marcat spre iesire este: " << std::endl
      << maze;

  return 0;
}

