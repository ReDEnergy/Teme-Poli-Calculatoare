#pragma once
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

/*
 *	Clasa Parser
 *		se ocupa cu prelucrarea benzii
 *		scrie / citeste / afiseaza / deplaseaza banda
 */
class Parser {

	private:
		Parser();

	public:

		~Parser();

		// instanta curenta
		static Parser* getInstance();

		// seteaza banda cu un nou sir
		void setTape(char *tape);

		// printeaza banda curenta si capul de citire
		void printTape();

		// restructureaza banda pentru afisarea finala
		void correctTape();

		// scrie un simbol pe banda la pozitia curenta
		void writeSymbol(char symb);

		// citeste simbolul de pe banda la pozitia curenta
		char getSymbol();

		// muta stanga / dreapta cursorul de citire pe banda
		void moveHead(int direction);

		// compara simbulul curent de pe banda cu un alt simbol
		bool cmpSymbAtCursor(char symb);

	public:

		// instanta
		static Parser* instance;

		// pozitie cursor
		int cursor;

		// banda
		vector<char> *Tape;

		// variabile salvate pentru folosire ulterioara
		map<int, char> saved;
};
