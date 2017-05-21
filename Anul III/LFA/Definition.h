#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <set>

#include "Parser.h"
#include "Turing.h"
#include "ElemMachine.h"
#include "StateMachine.h"

using namespace std;

/*
 *	Class Definition
 *		parsare si generare de Masini Turing in urma apelurilor din FLEX
 */
class Definition {
	private:
		Definition();

	public:
		~Definition();
		
		static Definition* getInstance();

		// crearea unei noi masini Turing
		void createTuringMachine();

		// crearea unei masini Turing cu stari
		void createStateMachine();

		// terminarea definitiei unei masini
		void closeStateMachine();

		// crearea unei stari noi in cadrul masinii curente
		void createState();

		// adaugare stare nou la masina curenta
		void addToState(char symb);

		// adaugarea Set stari la masina curenta
		void pushSetToState(char *name);

		// adaugarea symbol in cadrul unui set de stari
		void addSymbToSet(char x);

		// setare pointer spre un symbol dupa nume
		void pushPointerSymbToState(char *name);

		// adaugare declaratie simbol
		void addSymbDeclaration(char x);

		// setare nume Masina Turing / Set / symbol
		void setDName(char *name);

		// setare nume temporar pentru Jump
		void setTName(char *name);

		// inregistrare jump pe Masina curenta
		void saveJumpLocation(TuringMachine *X);

		// adaugarea unei masini Elementare in continuarea liste
		void pushSimpleEMachine(char type);

		// adaugarea masina elementara ce foloseste symbol salvat in cadrul masinii cu stari multiple
		void pushAddrEMachine(char value);

		// adaugarea unei masini cu jump / call / symbol	
		void pushAdvMachine(char symb);

		// seteaza directia de deplasare pentru urmatoarea masina declarata
		void setEMachineDir(char type);

		// seteaza masina cu pointer
		void pushPointerToEMachine(char *name);

		// construieste lista inlantuita de masini Turing
		void jumpToMachine(char *name);

		// afiseaza informatia despre masina Turing
		void printTuringMachines();

		// apeleaza o masina Turing pe banda actuala
		void solve(char *name);

	public:

		// instanta curenta a clasei
		static Definition* instance;

		// masini turing
		map<string, TuringMachine*> machine;

		// interator
		map<string, TuringMachine*>::iterator it;

		// masini turing temporale
		map<string, TuringMachine*> tempmachine;

		// map pentru simboluri
		map<string, char> symbols;

		// iterator map simboluri
		map<string, char>::iterator itsymb;

		// map set de simboluri
		map<string, set<char> > symbolSet;

		// interator map set simboluri
		map<string, set<char> >::iterator itsymbolSet;

		// map symboluri curente salvate in cadrul executiei starilor
		map<char, int> symb_addr;

		// masina curenta folosita
		TuringMachine* current;

		// stiva de masini turing in cadrul unei masini
		vector<TuringMachine*> *stackMachine;

		// Definition Name
		string dname;

		// Temporal Name
		string tname;

		// Salvare masina temporala
		int savetemp;

		// Salvare valoarea symbol de pe banda in variabila curenta
		char save_state_value;

		// Numarul definit de variabile de salavare
		int save_state_counter;

		// Directia urmatoarei masini turing declarate
		int eMdir;

		// Tipul de deplasare al masinii turing pana la / cat timp #
		bool eMmatch;
};
