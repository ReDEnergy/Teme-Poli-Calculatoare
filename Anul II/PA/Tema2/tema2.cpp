#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "tema2.h"

#define DEBUG
#define INF 10000


char buffer[256];
char read_msg_size;
int sockfd;
int num;

void error(const char *msg)
{
	perror(msg);
	exit(0);
}


void sendMessage(int sockfd, char* message, int length) {
	int sent_so_far = 0, ret = 0;
	while (sent_so_far < length) {
		ret = send(sockfd, message + sent_so_far, length - sent_so_far, 0);
		if (ret == -1) {
		  error("Can't send message");
			return;
		}
		sent_so_far += ret;
	}
}

void readMessage(int sockfd, char* message, int length) {
	int read_so_far = 0, ret = 0;
	while (read_so_far < length) {
		ret = recv(sockfd, message + read_so_far, length - read_so_far, 0);
		if (ret == -1) {
		  error("Can't read message");
			return;
		}
		read_so_far += ret;
	}
}


int main(int argc, char *argv[])
{
	if (argc < 5) {
		printf("Usage: %s server_hostname server_port opponent_level"
		    "(1=dumb, 5, 7, 8) own_level (1=dumb, 5, 7, 8)\n", argv[0]);
		exit(0);
	}

	//Datele pentru conexiune
	int portno;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	//Initializari & creare socket
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0){
		error("ERROR opening socket");
	}

	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
	serv_addr.sin_port = htons(portno);


	//Conectare la server
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		error("ERROR connecting");
	}

	//Trimitem primul mesaj - dificultatea adversarului

	bzero(buffer,256);
	buffer[0] = atoi(argv[3]);
	char message_size = 1; // Trimitem 1 octet
	sendMessage(sockfd, &message_size, 1);
	sendMessage(sockfd, buffer, 1);

	// primesc raspuns cu culoarea
	bzero(buffer,256);

	readMessage(sockfd, &read_msg_size, 1);
	readMessage(sockfd, buffer, read_msg_size);

	if (buffer[0] == 0) {
		// sunt jucatorul alb
		color = 0;
		printf("Sunt alb!\n");
	}
	else if (buffer[0] == 1) {
		// sunt jucatorul negru
		color = 1;
		printf("Sunt negru!\n");
	}
	else {
		// mesaj invalid; eroare!
	}

	/**	Debug file **/
//	out = fopen("table.txt", "w");

	/**	Set start table **/
	setStartTable();
	int x, y;

	while (true)
	{
		// Primesti un mesaj de la server care contine miscarile adversarului si zarul tau
		// Daca sunteti primii la mutare, atunci nu primiti mutari din partea adversarului
		// Daca adversarul nu a putut sa mute, atunci nu primiti mutari din partea adversarului
		bzero(buffer,256);

		// Cum scrie in enunt, se primeste: [D][mesaj_util].
		readMessage(sockfd, &read_msg_size, 1); // D
		readMessage(sockfd, buffer, read_msg_size); // mesaj_util


		updateTable((int)read_msg_size);		// update TableGame
		printReceiveMessage();

//		turn++;
//		fprintf(out,"\n\nTurn %d ************************** \n\n",turn);

		isEndGame();
		resetMoves();

		/**	Get Moves based on dice
		 **/
		if (Player[1].dice[0] == Player[1].dice[1])
			get4ValidMoves(Player[1], Table);
		else
			get2ValidMoves(Player[1], Table, 2);

		/**	Some infos
		 **/
		printTable(Table, stdout);
		printf("Rolled: %d %d\n", Player[1].dice[0], Player[1].dice[1]);
		printMoves();

		/**	Send moves
		 **/
		saveMoves();
		sleep(1);
		printTable(Table, stdout);
		printSendMessage();

		// Cum scrie in enunt, trimit [D][mesaj_util]
		sendMessage(sockfd, (char*) &num, 1); // D
		sendMessage(sockfd, buffer, num); // mesaj_util
		printf("Message sent!\n");
	}

	close(sockfd);
	return 0;
}

/**	Genereaza toate posibilitatile de a muta cu 2 zaruri diferite
 ** Functia va calcula cea mai buna mutare posibila din 2 zaruri
 **/
void get2ValidMoves (Stats PlayerX, TableGame *Table, int dice) {
	TableGame TableX[DIM];

	PlayerX.moved++;

	if (PlayerX.moved == 3) {
		compareMoves(Table, PlayerX);
		return;
	}

	int canMove = 0;

	for (int i=25; i>0; i--) {
		memcpy(TableX, Table, DIM * sizeof(TableGame));

		if (TableX[PlayerX.bar].tokens && i<25)
			break;

		if (isValidMove(TableX, i, PlayerX.dice[0], 1) && dice!=0) {
			canMove = 1;
			PlayerX.moves[2*(PlayerX.moved-1)] = i;
			PlayerX.moves[2*(PlayerX.moved-1)+1] = PlayerX.dice[0];
			makeMove(TableX, i, PlayerX.dice[0], 1);
			get2ValidMoves(PlayerX, TableX, 0);
		}

		memcpy(TableX, Table, DIM * sizeof(TableGame));

		if (isValidMove(TableX, i, PlayerX.dice[1], 1) && dice!= 1) {
			canMove = 1;
			PlayerX.moves[2*(PlayerX.moved-1)] = i;
			PlayerX.moves[2*(PlayerX.moved-1)+1] = PlayerX.dice[1];
			makeMove(TableX, i, PlayerX.dice[1], 1);
			get2ValidMoves(PlayerX, TableX, 1);
		}
	}

	if (canMove == 0)
		compareMoves(Table, PlayerX);
}


/**	Genereaza toate posibilitatile de a muta cu 4 zaruri toate la fel
 ** Functia va calcula cea mai buna mutare posibila din 4 zaruri
 **/
void get4ValidMoves (Stats PlayerX, TableGame *Table) {
	int dice = PlayerX.dice[0];
	TableGame TableX[DIM];

	PlayerX.moved++;

	if (PlayerX.moved == 5) {
		compareMoves(Table, PlayerX);
		return;
	}

	int canMove = 0;

	for (int i=25; i>0; i--) {
		memcpy(TableX, Table, DIM * sizeof(TableGame));

		if (TableX[PlayerX.bar].tokens && i<25)
			break;

		if (isValidMove(TableX, i, dice, 1)) {
			canMove=1;
			PlayerX.moves[2*(PlayerX.moved-1)] = i;
			PlayerX.moves[2*(PlayerX.moved-1)+1] = dice;
			makeMove(TableX, i, PlayerX.dice[0], 1);
			get4ValidMoves(PlayerX, TableX);
		}
	}

	if (canMove == 0)
		compareMoves(Table, PlayerX);
}

/**	Functie ce calculeaza valoarea unei configuratii de tabela
 ** Valoarea configuratiei maxime va reprezenta decizia de mutare a jetoanelor
 **/
int evalTable (TableGame *TableX, Stats PlayerX) {
	int i;
	int value = 0;
	int safe = 0;
	int enemy = 3 - PlayerX.player;

	/** Fiecare jeton luat de la inamic se considera puncte suplimentare **/
	for (i=0; i<TableX[0].tokens; i++)
		value += 5*(i+1);

	/** Calcularea pozitiei safe **/
	for (i=0; i<=24; i++) {
		if (TableX[i].player == enemy) {
			safe = i;
			break;
		}
	}

	/** Scoaterea celor mai bune piese **/
	if (PlayerX.canMoveOut == 1)
		value += (TableX[31].tokens * 10);

	/** Valoarea tablei de joc**/
	for (i=24; i>0; i--) {
		if (TableX[i].player != PlayerX.player)
			continue;

		if (TableX[i].tokens == 1 && i>safe)
			value -= CValue[PlayerX.player][i];

		if (TableX[i].tokens >= 2)
			value += CValue[PlayerX.player][i];

		if (TableX[i].tokens > 3)
			value -= (TableX[i].tokens - 3);
	}

	return value;
}


/** Evalueaza si compara tabela curenta cu cea mai buna valoare retinuta pana
 ** in acest moment
 **/
void compareMoves(TableGame *TableX, Stats PlayerX) {
/*
	fprintf(out,"Solution => ");
	for (int i=0; i< 2 * (PlayerX.moved - 1); i++)
		fprintf(out,"%d ", PlayerX.moves[i]);
	fprintf(out," => table value : %d\n", evalTable(TableX, PlayerX));
	fflush(out);
*/

	if (PlayerX.moved < newMoves.nr)
		return;

	int tableValue = evalTable(TableX, PlayerX);

	if (tableValue > newMoves.tableValue)
	{
		memcpy(newMoves.Table, TableX, DIM * sizeof(TableGame));
		newMoves.nr = PlayerX.moved;
		memcpy(newMoves.moves, PlayerX.moves, 8 * sizeof(int));
		newMoves.tableValue = tableValue;
	}
}

/**	Face update pe Tabela de joc X executand mutarea respectiva
 **/
void makeMove (TableGame *TableX, int source, int steps, int player) {
	int dir = 1;
	if (player == 1) dir = -1;
	int enemy = 3 - player;
	int dest = source + dir * steps;

	/**	Remove 1 token from the source TableGame
	 **/
	TableX[source].tokens--;
	if (TableX[source].tokens == 0)
		TableX[source].player = 0;

	if (dest > 24)
		dest = 32;
	if (dest < 1)
		dest = 31;

	/**	Take out enemy token when moving
	 **/
	if (TableX[dest].player != player &&
		TableX[dest].player && dest <= 24 && dest >=1)
	{
		TableX[dest].tokens = 0;
		TableX[(24 + TableX[dest].player) % 26].tokens++;
	}

	/**	Put the token in the destination TableGame
	 **/
	TableX[dest].player = player;
	TableX[dest].tokens++;
}

/**	Intoarce 1 daca mutarea se poate executa si este valida, altfel 0
 **/
int isValidMove(TableGame *TableX, int source, int steps, int player) {
	canMoveOut(TableX);

	int dir = 1;
	if (player == 1) dir = -1;
	int enemy = 3 - player;

	int dest = source + dir * steps;

	if (TableX[source].tokens == 0)
		return 0;

	if (source != 25)
		if (TableX[source].player != player)
			return 0;

	if ( dest < 1) {
		if (Player[1].canMoveOut == 0)
			return 0;
		else {
			if (source == steps)
				return 1;

			for (int i=6; i>source; i--)
				if (TableX[i].tokens && TableX[i].player == player)
					return 0;

			return 1;
		}
	}

	if (TableX[dest].player == enemy && TableX[dest].tokens >1)
		return 0;

	return 1;
}

/**	Verifica daca se poat scoate piese de pe tabla de joc
 **/
void canMoveOut(TableGame *TableX) {
	Player[1].canMoveOut = 0;

	if (TableX[25].tokens)
		return;

	for (int i=24; i>6; i--) {
		if (TableX[i].player == 1)
			return;
	}

	Player[1].canMoveOut = 1;
}

/**	Salvarea mutarilor pentru a fi trimise serverului
 **/
void saveMoves() {
	int x;
	memcpy(Table, newMoves.Table, DIM * sizeof(TableGame));
	num = (newMoves.nr-1) * 2;
	for (int i=0; i<num; i++) {
		x = newMoves.moves[i];
		if (i%2==0)
			x = translateMove(x);
		buffer[i] = (char) x;
	}
}

/**	Resetarea mutarilor pe 0 pentru a fi calculate
 **/
void resetMoves() {
	newMoves.tableValue = -INF;
	newMoves.nr = 0;
}

/**	Translateaza orice mutare pentru noua tabla de joc
 **/
int translateMove(int col) {
	if (col == 25 || col == 26)
		return 30;

	if (color)
		return 25 - col;

	return col;
}


/******************************************/
/** Functii de setat si update pe tabele **/
/******************************************/

/**	Dupa primirea mutarilor adversarului se face update la tabla de joc
 **/
void updateTable(int nr) {
	int i, source = 0, steps;

	fprintf(stdout," Player 2 \n");

	for (i=0; i<nr-2; i+=2) {
		if (buffer[i] != 30)
			source = translateMove((int) buffer[i]);

		steps = (int) buffer[i+1];
		fprintf(stdout," Move %d => %d \n", source, steps);
		makeMove(Table, source, steps, 2);
	}

	Player[1].dice[0] = buffer[nr-2];
	Player[1].dice[1] = buffer[nr-1];
}

/**	Setarea configuratiei initiale a tablei de joc si a altor variabile statice
 **/
void setStartTable() {
	int i;

	for (i=1; i<=24; i++) {
		CValue[1][i] = 25 - i;
		CValue[2][i] = i;
		Table[i].player = 0;
	}

	Player[1].bar = 25;
	Player[2].bar = 0;
	Player[1].player = 1;
	Player[2].player = 2;
	Player[1].canMoveOut = 0;
	Player[2].canMoveOut = 0;


	setTableGame(0,  0, 2);
	setTableGame(1,  2, 2);
	setTableGame(6,  5, 1);
	setTableGame(8,  3, 1);
	setTableGame(12, 5, 2);
	setTableGame(13, 5, 1);
	setTableGame(17, 3, 2);
	setTableGame(19, 5, 2);
	setTableGame(24, 2, 1);
	setTableGame(25, 0, 1);
}

/**	Functia seteaza coloana col cu numarul respectiv de tokens pentru un player
 **/
void setTableGame (int col, int tokens, int player) {
	Table[col].player = player;
	Table[col].tokens = tokens;
}

/** Afiseaza mesaj de final de joc
 **/
void isEndGame () {
	if (read_msg_size == 1)
	{
		if (buffer[0] == 'W')
			printf("WE WON\n");
		else
			printf("WE LOST\n");

		exit(0);
	}
}

/*****************************************************************/
/**	Functii de debbuging si printare de mutari si tabele de joc **/
/*****************************************************************/

void printMoves() {
	for (int i=0; i<2*(newMoves.nr-1); i++ )
		printf("%d ",newMoves.moves[i]);
	printf("\n");
	printf("Cost : %d\n", newMoves.tableValue);
}

void printSendMessage () {
	printf("%d : [", num);
	for (int i = 0; i < num; ++i) {
	  printf("%d ", buffer[i]);
	}
	printf("]\n");
}

void printReceiveMessage () {
	int i, x;
	if (read_msg_size == 2)
		return;

	printf("%d : [", read_msg_size - 2);
	for (i = 0; i < read_msg_size - 2; i+=2) {
		x = translateMove((int) buffer[i]);
		printf("%d => %d; ", x, buffer[i+1]);
	}
	printf("]\n");

}

void printTable (TableGame *TableX, FILE *out) {
	int i;
	char c;

	fprintf(out, "\n\n");
	fprintf(out, "+13-14-15-16-17-18------19-20-21-22-23-24-+\n|");

	for (i=13; i<=24; i++) {
		switch(TableX[i].player) {
			case 1:
				c = 'X';	break;
			case 2:
				c = 'O';	break;
			default:
				c = ' ';	break;
		}
		fprintf(out, " %c ", c);

		if (i==18)
			fprintf(out, "|   |", c);
	}
	fprintf(out, "|\n|");

	for (i=13; i<=24; i++) {
		if (TableX[i].tokens)
			fprintf(out, " %d ", TableX[i].tokens);
		else
			fprintf(out, "   ");
		if (i==18)
			fprintf(out, "|   |", c);
	}

	fprintf(out, "|\n");
	fprintf(out, "|                  |   |                  |\n");
	fprintf(out, "|                  |   |                  |\n");
	fprintf(out, "|==================|   |==================|\n");
	fprintf(out, "|                  |   |                  |\n");
	fprintf(out, "|                  |   |                  |\n");
	fprintf(out, "|");

	for (i=12; i>=1; i--) {
		if (TableX[i].tokens)
			fprintf(out, " %d ", TableX[i].tokens);
		else
			fprintf(out, "   ");
		if (i==7)
			fprintf(out, "|   |", c);
	}

	fprintf(out, "|\n|");

	for (i=12; i>=1; i--) {
		switch(TableX[i].player) {
			case 1:
				c = 'X';	break;
			case 2:
				c = 'O';	break;
			default:
				c = ' ';	break;
		}
		fprintf(out, " %c ", c);

		if (i==7)
			fprintf(out, "|   |", c);
	}

	fprintf(out, "|\n");
	fprintf(out, "+12-11-10--9--8--7-------6--5--4--3--2--1-+\n");
	fprintf(out, "\n");

	fprintf(out, "Bar 25: %d \t Bar   0: %d\n", TableX[25].tokens, TableX[0].tokens);
	fprintf(out, "Out 31: %d \t Out  32: %d\n", TableX[31].tokens, TableX[32].tokens);
	fprintf(out, "\n");

	fflush(out);
}


