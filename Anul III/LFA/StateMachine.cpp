#include "StateMachine.h"

StateMachine::StateMachine() { 
	type = 1;
}

StateMachine::~StateMachine() { 

}

TuringMachine* StateMachine::createState(int save) { 
	mState = new TuringMachine();
	mState->save = save;
	return mState;
}

void StateMachine::addToState(char S, bool match) {

	if (match == true)
		state[S] = mState;
	else
		notstate[S] = mState;
}

TuringMachine* StateMachine::run() {
	Parser *MParser = Parser::getInstance();

	char symb = MParser->getSymbol();

	it = state.find(symb);
	
	if (it != state.end()) {
		// cout << "\t State " << symb << endl;
		TuringMachine *T = state[symb];

		if (T->save)  {
			// cout << "Saved " << symb << " in " << T->save << endl;
			MParser->saved[T->save] = symb;
		}

		return T->next;
	}

	it = notstate.find(symb);
	
	if (it == notstate.end()) {
		// cout << "\t State not " << symb << endl;
		TuringMachine *T = notstate.begin()->second;

		if (T->save) {
			// cout << "Saved " << symb << " in " << T->save  << endl;
			MParser->saved[T->save] = symb;
		}

		return T->next;
	}

	return NULL;
}

void StateMachine::info() {

	cout << "========= State machine" << endl;

	cout << "Match this" << endl;

	for (it = state.begin(); it != state.end(); it++) {
		cout << it->first << " => " << endl;
		infoTuringMachine(it->second);
	}

	cout << "Do not match this" << endl;

	for (it = notstate.begin(); it != notstate.end(); it++) {
		cout << "not " << it->first << " => " << endl;
		infoTuringMachine(it->second);
	}

	cout << "========= Exit machine" << endl;

}