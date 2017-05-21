#include <iostream>

#include "Nim.h"

#define UNDEF -1

int negaMax(NimGameConf& game, int depth) {
  /* Daca s-a terminat jocul, atunci scorul este 0. */
  if (game.gameOver()) {
    return 0;
  }

  /* TODO: Altfel, fa o miscare pe tabla astfel incat sa castigi. Stergeti linia
   * de mai jos cand rezolvati, ea inseamna ca nu se face nici o miscare, si ca
   * AI-ul spune ca a pierdut deja. */
   
   int i;
   int aiurea = game.size(); 
   for (i=3; i<= aiurea; i++) {
   if (game[i]) {
   		for ( int j=1; j<=i/2; j++) {
   			game.split(i, j, i-j);
   			int ceva = negaMax(game, depth-1);
   			if (ceva == 0) {
   				if (depth)
   					game.unsplit(i, j, i-j);
   				return 1;
   				}
   			else
   				game.unsplit(i, j, i-j);
   		}
	}
   }
   
   if (depth == 0)
   		game.split(aiurea, 1, aiurea-1);
   
  return 0;
}

void computerPlayer(NimGameConf& game) {
  std::cout << game << "Computer starts thinking... ";
  negaMax(game, 0);
  std::cout << "done." << std::endl;
}

void humanPlayer(NimGameConf& game) {
  std::cout << game << "Alege o gramada de impartit, precum si dimensiunile"
      << " in care vrei sa spargi gramada: ";
  int heap, a, b;
  do {
    /* Citim o miscare de la consola. */
    std::cin >> heap >> a >> b;
    if (game[heap] == 0) {
      /* Verificam sa existe o gramada de atatea pietricele. */
      std::cout << "Error! Nu exista nici o gramada de " << heap
          << " pietricele. Try again: ";
    } else if (heap != a + b || a <= 0 || b <= 0) {
      /* Verificam ca miscarea sa fie valida. */
      std::cout << "Error! O gramada de " << heap << " pietricele nu se "
          << "poate imparti in " << a << " + " << b << " pietricele."
          << std::endl << "Try again: ";
    } else if (heap >= 0 && heap <= 2) {
      /* Alta verificare de miscare valida. */
      std::cout << "Error! Nu se poate imparti o gramada de " << heap
          << " pietricele in gramezi mai mici. Try again: ";
    } else {
      /* Efectuam miscarea si iesim din functie. */
      game.split(heap, a, b);
      return;
    }
  } while(1);
}

int main()
{
  /* Citim un numar initial de pietricele si pornim un joc. */
  int n;
  std::cout << "Game size: ";
  std::cin >> n;
  NimGameConf game(n);

  do {
    /* First player takes a move. */
    computerPlayer(game);
    std::cout << std::endl;
    if (game.gameOver()) {
      std::cout << std::endl << "Player 1 wins!" << std::endl;
      break;
    }

    /* Second player takes a move. */
    humanPlayer(game);
    std::cout << std::endl;
    if (game.gameOver()) {
      std::cout << std::endl << "Player 2 wins!" << std::endl;
      break;
    }
  } while(1);

  return 0;
}

