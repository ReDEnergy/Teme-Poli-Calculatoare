#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <cstring>

#include "Turing.h"
#include "Parser.h"
#include "Definition.h"

using namespace std;

/*
 *	Masina elementara Turing
 */
class ElemMachine : public TuringMachine {

	public:

		ElemMachine(char symbol);

		ElemMachine(int direction);

		ElemMachine(int direction, char symbol, bool matchType);

		~ElemMachine();

		TuringMachine* run();

		void info();

	private:

		bool step;

		int direction;

		char symbol;

		bool matchType;
};



/*
	ElemMachine *L = new ElemMachine(-1);
	ElemMachine *R = new ElemMachine(1);

	ElemMachine *a = new ElemMachine('a');
	ElemMachine *b = new ElemMachine('b');

	ElemMachine *La = new ElemMachine(-1, 'a', true);
	ElemMachine *Ra = new ElemMachine(1, 'a', true);
	ElemMachine *nLa = new ElemMachine(-1, 'a', false);
	ElemMachine *nRa = new ElemMachine(1, 'a', false);

	ElemMachine *Lb = new ElemMachine(-1, 'b', true);
	ElemMachine *Rb = new ElemMachine(1, 'b', true);
	ElemMachine *nLb = new ElemMachine(-1, 'b', false);
	ElemMachine *nRb = new ElemMachine(1, 'b', false);

	ElemMachine *L_ = new ElemMachine(-1, '#', true);
	ElemMachine *R_ = new ElemMachine(1, '#', true);
	ElemMachine *nL_ = new ElemMachine(-1, '#', false);
	ElemMachine *nR_ = new ElemMachine(1, '#', false);

*/
