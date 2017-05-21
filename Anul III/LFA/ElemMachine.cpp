#include "ElemMachine.h"

ElemMachine::ElemMachine(char symbol)
	: symbol(symbol)
{
	type = 0;
	direction = 0;
	matchType = false;
	step = true;
}

ElemMachine::ElemMachine(int direction) 
	: direction(direction)
{
	type = 0;
	symbol = 0;
	matchType = false;
	step = true;
}

ElemMachine::ElemMachine(int direction, char symbol, bool matchType)
	: direction(direction),
	  symbol(symbol),
	  matchType(matchType)
{ 
	type = 0;
	step = false;
}

ElemMachine::~ElemMachine() {
}

TuringMachine* ElemMachine::run() {

	Parser *MParser = Parser::getInstance();

	if (step == true) {

		if (direction > 1)
			MParser->writeSymbol(MParser->saved[direction]); 	
		else {
			MParser->moveHead(direction);

			if (symbol)
				MParser->writeSymbol(symbol);
		}

		// MParser->printTape();

		return next;
	}

	if (matchType == true) {
		MParser->moveHead(direction);
		while (MParser->cmpSymbAtCursor(symbol) == false)
			MParser->moveHead(direction);
	}

	else {
		MParser->moveHead(direction);
		while (MParser->cmpSymbAtCursor(symbol) == true)
			MParser->moveHead(direction);
	}

	// MParser->printTape();

	return next;
}

void ElemMachine::info() {

	cout << "\t";

	if (direction == 0 && symbol == 0) {
		cout << "Call " << callM << endl;
		return;
	}

	if (direction == -1)
		cout << "left ";
	if (direction == 1)
		cout << "right ";

	if (step == true) {
		if (direction == 0)
			cout <<	"write " << symbol;
	}
	else {
		if (matchType)
			cout << "to " << symbol;
		else
			cout << "while " << symbol;
	}

	cout << endl;

}
