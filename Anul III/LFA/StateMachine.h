#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>

#include "Turing.h"
#include "Definition.h"

using namespace std;

class StateMachine : public TuringMachine {

	public:
		StateMachine();

		~StateMachine();

		TuringMachine* run();

		void info();

		void addToState(char S, bool match);

		TuringMachine* createState(int save);

	public:
		map<char, TuringMachine*> state;
		map<char, TuringMachine*> notstate;
		map<char, TuringMachine*>::iterator it;
		TuringMachine *mState;

};