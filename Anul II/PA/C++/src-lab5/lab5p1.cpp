#include <iostream>
#include <fstream>
#include <vector>

#include "XOBoard.h"

#define INF 8

#define OPPONENT(x) ((x) == XOBoard::PlayerX ? \
                     XOBoard::PlayerO : XOBoard::PlayerX)

int x,y;

int negMax(int depth, XOBoard::Player player, XOBoard &board) {

	if (board.is_full() == true )
		return board.get_score(player) - board.get_score(OPPONENT(player));

 	int i, j, ans = -INF;
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++)  {
			if (board.get(i,j) == '_')	{
			
				board.put(player, i, j);
				
				int sol = -negMax(depth-1, OPPONENT(player), board);
				if (sol > ans) {
					ans=sol;
					if (depth == 8) {
						x=i;
						y=j;
					}
				}
					
				board.erase(i, j);
			}	
		}
	}
	return ans;
}


int negMax2(int depth, XOBoard::Player player, XOBoard &board, int alfa, int beta) {

	if (board.is_full() == true )
		return board.get_score(player) - board.get_score(OPPONENT(player));

 	int i, j, ans = -INF;
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++)  {
			if (board.get(i,j) == '_')	{
			
				board.put(player, i, j);
				
				int sol = -negMax2(depth-1, OPPONENT(player), board, -beta, -alfa);
				if (sol >= beta) {
					x = i;
					y = j;
				    return beta;
				}
				if (sol > alfa) {
					alfa = sol;
				}
					
				board.erase(i, j);
			}	
		}
	}
	return alfa;
}


/** Un exemplu de functie de gandire care foloseste rezultatele furnizate de
 * o abordare negaMax exhaustiva. */
XOBoard negaMaxThink(XOBoard::Player player, XOBoard board)
{
  /* TODO: Fa o miscare si returneaza tabla dupa aceasta miscare. Aceasta
   * functie de AI trebuie sa respecte acest format pentru ca este data in
   * constructorul jocului, dar puteti apela aici o functie scrisa de voi. */

  /* TODO: Stergeti linia de mai jos dupa ce rezolvati. */
	int a;
	x=-1;
	y=-1;
//	a = negMax(8, player, board);  
	a = negMax2(8, player, board, -INF, INF);  
	board.put(player, x, y);
	return board;
}

int main()
{
  /** Cream un engine de joc si specificam ca am vrea ca AI-ul sa gandeasca
   * cu functia data ca parametru (o functie de tipul:
   *       XOBoard ComputerAI(XOBoard::Player, XOBoard)).
   *
   * Engine-ul de joc va apela intern aceasta functie atunci cand pune
   * calculatorul sa gandeasca. Parametrii au urmatoarea semnificatie:
   *
   * player = jucatorul care trebuie sa mute in continuare (identitatea
   *          calculatorului care gandeste cu aceasta functie).
   *           Valorile posibile sunt { XOBoard::PlayerX, XOBoard::PlayerO }
   *
   * board = tabla pe care o vede jucatorul care trebuie sa mute in
   *         continuare.
   */
  XOGame game(negaMaxThink);

  /** Lansam un joc intre un om (joaca cu X) si un calculator (joaca cu O) 
   * Puteti lansa si computerVShuman, humanVShuman, dar si computerVScomputer.
   * Mai multe detalii in documentatie.
   * (codeBase/C++/doc/html/index.html -> classes)
   */
  game.humanVScomputer();

  return 0;
}

