#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstring>
#include <map>

#include "Turing.h"

using namespace std;

/*
 *	Masina Turing
 *		masina turing elementara
 *		taote masinile implementeaza aceasta clasa
 */

class TuringMachine {

	public:
	
		TuringMachine();
		~TuringMachine();

		// Executarea masinii turing curente
		virtual TuringMachine* run();

		// Afisare informatii despre masina turing completa
		virtual void info();

		// Afisare informatii despre o masina turing recursiv
		static void infoTuringMachine(TuringMachine *T);

	public:

		// Urmatoarea Masina Turing ce trebui apelata
		TuringMachine *next;

		// Apelul unei alte Masina Turing
		TuringMachine *jump;

		// Numele Masinii Turing la care sare
		string jumpName;

		// Numele loop la care se intoarce Masina Turing
		string callM;

		// Tipul masinii Turing - folosit pentru afisare
		int type;

		// Variabila in care se va salva symbolul citit de masina
		int save;
};


