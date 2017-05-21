#include "Parser.h"


Parser* Parser::instance = NULL;

Parser::Parser() {
	cursor = -1;
	Tape = new vector<char>;
};

Parser::~Parser(){
}

Parser* Parser::getInstance() {
	if (instance == NULL) {
		instance = new Parser(); 
	}
	return instance; 
}

void Parser::setTape(char *tape) {

	unsigned int i = 0;

	while(tape[i]) {

		if (tape[i] == '>')
			cursor = i;
		else
			Tape->push_back(tape[i]);

		i++;
	}
}

void Parser::printTape() {
	cout<<"Tape: ";
	for (unsigned int i = 0; i<Tape->size(); i++)
		cout<<(*Tape)[i];
	cout<<endl;

	cout<<"Head: "<<cursor<<endl;
}

void Parser::writeSymbol(char symb) {
	(*Tape)[cursor] = symb;
}

char Parser::getSymbol() {
	return (*Tape)[cursor];
}

void Parser::moveHead(int direction) {
	cursor += direction;

	if (cursor > (int)Tape->size() - 1)
		Tape->push_back('#');

	if (cursor < 0) {
		cursor = 0;
		Tape->insert(Tape->begin(), '#');
	}
}

bool Parser::cmpSymbAtCursor( char symb) {

	return (*Tape)[cursor] == symb;
}


void Parser::correctTape() {
	unsigned int i;
	unsigned int size;

	Tape->push_back('#');

	Tape->insert(Tape->begin() + cursor, '>');
	Tape->insert(Tape->begin(), '#');

	i=0;
	while((*Tape)[i+1] == '#') {
		Tape->erase(Tape->begin());
	}

	size = Tape->size();

	for (i = size - 1; i > 0; i--)
		if((*Tape)[i] == '#' && (*Tape)[i-1] == '#')
			Tape->pop_back();
		else
			break;

	size = Tape->size();

	for (i = 0; i < size; i++)
		cout<<(*Tape)[i];
	cout<<endl;
}