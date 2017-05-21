#include "Definition.h"

Definition* Definition::instance = NULL;

Definition::Definition() {
	save_state_counter = 10;
	savetemp = 0;
	save_state_value = 0;
	eMmatch = true;
	stackMachine = new vector<TuringMachine*>;
}

Definition::~Definition() {

}
		
Definition* Definition::getInstance() {
	if (instance == NULL) {
		instance = new Definition(); 
	}
	return instance; 
}

void Definition::createTuringMachine() {
	machine[dname] = new TuringMachine();
	current = machine[dname];
	stackMachine->push_back(current);
}

void Definition::createStateMachine() {
	StateMachine *X = new StateMachine();
	saveJumpLocation(X);
	stackMachine->push_back(X);
}

void Definition::closeStateMachine() {
	stackMachine->pop_back();
	// cout << "Closed State Machine";
}

void Definition::createState() {

	StateMachine* stateM = (StateMachine*) stackMachine->back();

	if (save_state_value) {
		save_state_counter++;
		symb_addr[save_state_value] = save_state_counter;
		// cout << "Value " << save_state_value << " addr : " << save_state_counter << endl;
		save_state_value = 0;
	}

	current = stateM->createState(save_state_counter);

}

void Definition::addToState(char symb) {
	StateMachine* stateM = (StateMachine*) stackMachine->back();

	// cout << "Match : " << eMmatch << endl;
	stateM->addToState(symb, eMmatch);
}


void Definition::pushSetToState(char *name) {

	createState();

	string pname(name);	
	itsymbolSet = symbolSet.find(pname);

	if (itsymbolSet == symbolSet.end()) {
		// cout << " Nu a fost gasit setul de simboluri " << pname << endl;
	}
	else {

		StateMachine* stateM = (StateMachine*) stackMachine->back();

		set<char> sSet = itsymbolSet->second;
		set<char>::iterator its;

		for(its = sSet.begin(); its != sSet.end(); its++) {
			// cout << "Adaugat la set " << *its << endl;
			stateM->addToState(*its, eMmatch);
		}
	}
}

void Definition::pushPointerSymbToState(char *name) {
	
	string pname = string(name);

	itsymb = symbols.find(pname);

	if (itsymb == symbols.end()) {
		// cout << "Symbolul " << name << "nu a fost declarat" << endl;
	}
	else {
		addToState(itsymb->second);
	 	// cout << "Pointer " << pname << " = " << itsymb->second << endl; 
	}
}


void Definition::addSymbToSet(char x) {
	symbolSet[dname].insert(x);
}

void Definition::addSymbDeclaration(char x) {
	symbols[dname] = x;
}



void Definition::setDName(char *name) {
	dname = string(name);
}

void Definition::setTName(char *name) {
	tname = string(name);
	tname.erase(tname.size() - 1);
	savetemp = 1;
}


void Definition::setEMachineDir(char type) {
	if (type == 'L')
		eMdir = -1;
	if (type == 'R')
		eMdir = 1;
	eMmatch = true;
}

void Definition::saveJumpLocation(TuringMachine *T) {

	if (savetemp == 1) {
		// cout << "Pointer " << tname << endl;
		tempmachine[tname] = T;
		savetemp = 0;
	}

	current->next = T;
	current = T;
}


void Definition::pushAddrEMachine(char value) {
	// cout << "Value " << symb_addr[value] << endl;

	ElemMachine *X  = new ElemMachine(symb_addr[value]);

	saveJumpLocation(X);
}


void Definition::pushSimpleEMachine(char type) {

	ElemMachine *X;

	switch(type) {
		case 'L':
			X = new ElemMachine(-1);
			break;
		case 'R':
			X = new ElemMachine(1);
			break;
		default:
			X = new ElemMachine(type);
			break;
	}

	saveJumpLocation(X);

}

void Definition::pushAdvMachine(char symb) {

	ElemMachine *X = new ElemMachine(eMdir, symb, eMmatch);

	saveJumpLocation(X);

}

void Definition::pushPointerToEMachine(char *name) {

	string pname(name);
	pname.erase(pname.size() - 1);

	if (name[0] == '<') {
		pname.erase(pname.begin());

		int type = 0;
		if (pname[pname.size() - 1] == '>') {
			type = 1;
			pname.erase(pname.size() - 1);
		}

		itsymb = symbols.find(pname);

		if (itsymb == symbols.end()) {
			// cout << "Symbolul " << pname << "nu a fost declarat" << endl;
		}
		else {
			// cout << "Symbolul " << pname << " = " << itsymb->second << endl;

			if (type == 0)
				pushAdvMachine(itsymb->second);
			else 
				pushSimpleEMachine(itsymb->second);
		}
	}
	else {
		it = machine.find(pname);

		if (it == machine.end()) {
			// cout << "Masina " << name << " nu a fost declarata" << endl;
		}
		else {
			// cout << "Gasit masina " << pname << endl;

			ElemMachine *X = new ElemMachine(0);
			saveJumpLocation(X);
			current->callM = string(pname);
		}

	}
}


void Definition::jumpToMachine(char *name) {
	string label = string(++name);
	it = tempmachine.find(label);

	if (it == tempmachine.end()) {
//		cout << "Nu s-a gasit " << name << endl;
	}
	else {
//		cout << "Jump " << it->first << " = ";
		current->jumpName = label;
		current->jump = it->second;
	}
}

void Definition::printTuringMachines() {
	for (it = machine.begin(); it != machine.end(); it++) {
		cout << "\n\n\n ======================================== " << endl;
		cout << it->first << " => " << endl;
		TuringMachine::infoTuringMachine(it->second);
		cout << "\n ======================================== " << endl;
	}
}

void Definition::solve(char *name) {
	string label = string(name);
	TuringMachine* T = machine[label];
	TuringMachine* auxT;

//	cout << "\n\n\n ********************************** " << endl;
//	cout << "Incepe rezolvare" << endl;

	T = T->next;

	while(T) {

//		if (T->type == 0)
//			T->info();

		auxT = T->run();

		if (T->callM.size()) {
			solve((char*)T->callM.c_str());
		}

		if (T->jump)
			T = T->jump;
		else
			T = auxT;

	}

//	cout << "Finalizare" << endl;
}




