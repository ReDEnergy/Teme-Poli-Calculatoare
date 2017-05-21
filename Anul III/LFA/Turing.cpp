#include "Turing.h"


TuringMachine::TuringMachine() {
	next = NULL;
	jump = NULL;
	type = 0;
	callM = string("");
	jumpName = string("");
	save = 0;
}

TuringMachine::~TuringMachine() {
}

TuringMachine* TuringMachine::run() {
	return NULL;
}

void TuringMachine::info() {
}

void TuringMachine::infoTuringMachine(TuringMachine *T) {
	while (T) {
		T->info();

		if (T->jump)
			cout << "Jump : " << T->jumpName << endl;

		T = T->next;
	}
}