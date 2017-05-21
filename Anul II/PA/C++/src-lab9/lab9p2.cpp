#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "Maze.h"

using namespace std;

const int vi[4] = { -1, 0, 1, 0 };
const int vj[4] = { 0, 1, 0, -1 };
char maze[100][100];
char costA[] = "123456789";


typedef struct {
	int a;
	int b;
	int x;
	int y;
	int cost;
} Ceva;

vector <Ceva> COST;



void dig_out_maze(Maze& maze, int line, int column) {
  /* TODO: Sapati galerii prin maze astfel incat:
   * (a) orice celula dintr-o galerie sa aiba acces la exterior.
   * (b) galeriile sa fie "inguste" (adica sa nu se formeze camere de 2x2).
   */
}


void printNods() {
	int i;
	for (i=0; i<COST.size(); i++) {
		printf(" (%d,%d) <=> (%d,%d) : %d\n", COST[i].a, COST[i].b, COST[i].x, COST[i].y, COST[i].cost );
	}
}

int m[5][7];

void initNods() {
	int i,j;
	for (i=0; i<COST.size(); i++) {
		m[COST[i].a][COST[i].b] = 1;
		m[COST[i].x][COST[i].y] = 1;
	}

	for (i=0; i<7; i++) {
		for (j=0; j<5; j++) {
			cout << m[i][j]<< " ";
		}
		cout<<endl;
	}

}




void initMaze (int width, int height) {
	int k = 0, r;
	Ceva nod;
	for (int i=0; i<height; i++)
		for (int j=0; j<width; j++) {
			maze[i][j] = '#';
			if (i%2==1 && j%2==1 ) {
				maze[i][j] = ' ';
			}


			if (i%2==1 && j%2==0 && i!=0 && i!=height-1 && j && j!=width-1) {
				r = rand() % 9;
				maze[i][j] = costA[r];
			}

			if (maze[i][j] == ' ' && maze[i-1][j]!='#' && i>2) {
				nod.a = i;
				nod.b = j;
				nod.x = i-2;
				nod.y = j;
				nod.cost = (int) maze[i-1][j] - 48;
				COST.push_back(nod);
			}


			if (i%2==0 && j%2==1 && i!=0 && i!=height-1) {
				r = rand() % 9;
				maze[i][j] = costA[r];
			}

			if (maze[i][j] == ' ' && maze[i][j-1]!='#' && j>2) {
				nod.a = i;
				nod.b = j;
				nod.x = i;
				nod.y = j-2;
				nod.cost = r+1;
				COST.push_back(nod);
			}


		}

	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++)
			cout<<maze[i][j];
		cout<<endl;
	}


printNods();
initNods();
}



int main()
{
  int width, height;

  /* Citeste dimensiunile labirintului si creaza o parcela de pamant de
   * dimensiunile respective. */
  std::cin >> width >> height;
  initMaze(width, height);
 // Maze maze(width, height);

  /* Sapa un labirint din respectiva parcela de pamant. */
//  dig_out_maze(maze);

  /* Si afiseaza-l. */
//  std::cout << "The resulting maze is:" << std::endl << maze;
  return 0;
}

