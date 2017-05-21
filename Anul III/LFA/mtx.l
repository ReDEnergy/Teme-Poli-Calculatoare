
%{

#include <iostream>
#include <fstream>
#include <cstdio>
#include "Definition.h"

using namespace std;

class FlexParser : public yyFlexLexer {
	public:
		virtual int yylex();
};


Definition *Def = Definition::getInstance();

%}

%option c++
%option noyywrap
%option stack
%option yyclass="FlexParser"


EOL				[\n]
SPACES			[ \t]*			
DIGIT			[0-9]
LETTER			[a-zA-Z]
OTHER			[#$@*]
NAME			({LETTER}|{DIGIT}|_)+
SYMBOL			{LETTER}|{DIGIT}|{OTHER}
ELEM			{NAME}|<{NAME}>
EMACHINE		L|R


ALPHABET 		alphabet{SPACES}::
DEFFINITION		{NAME}{SPACES}::=



%x alphabetdecl
%x declarationType
%x symbolSet
%x symbolDecl
%x turingMachine
%x stateMachine
%x stateElem
%x elementaryMachine
%x headRepositioningMachine
%%

{EOL}*				;
;{SPACES}*{EOL}		;
^;.*{EOL}			;

{ALPHABET}			{ 
	BEGIN(alphabetdecl);
	cout << "Apthabet : ";
}
					
<alphabetdecl>{
	; { 
		BEGIN(INITIAL);
		cout << endl;
	}
	{SPACES}	;
	{SYMBOL}	{
		cout << yytext << " ";;
	}
}

^{NAME}	{
	yy_push_state(declarationType);
	Def->setDName(yytext);
}

<declarationType>{
	;			yy_pop_state();

	{SPACES}	;

	=			{
		yy_push_state(symbolDecl);
		// cout << "Symbol declaration: "<< Def->dname << endl;
	}

	:=			{
		yy_push_state(symbolSet);
		// cout << "Symbol set: "<< Def->dname << endl;
	}

	::=			{
		yy_push_state(turingMachine);
		Def->createTuringMachine();
	}

}

<symbolDecl>{
	{SPACES}	;

	{SYMBOL}	{
		yy_pop_state();
		Def->addSymbDeclaration(yytext[0]);
	}

}

<symbolSet>{
	\}			yy_pop_state();	

	{SPACES}	;

	[{,]		;

	{SYMBOL}	Def->addSymbToSet(yytext[0]);
}

<turingMachine>{
	;			yy_pop_state();
	{SPACES}	;

	{NAME}@		Def->setTName(yytext);

	&{NAME}		Def->jumpToMachine(yytext);

	\[			yy_push_state(elementaryMachine);

	\(			{
		yy_push_state(stateMachine);
		Def->createStateMachine();
	}
}

<stateMachine>{
	\)			{
		yy_pop_state();
		Def->closeStateMachine();
		Def->eMmatch = true;
	}

	{SPACES}	;

	[<>]		;

	!			Def->eMmatch = false;

	{SYMBOL}@	Def->save_state_value = yytext[0];

	{NAME}		Def->pushSetToState(yytext);

	\{			{
		Def->createState();
		yy_push_state(stateElem);
	}

	->			yy_push_state(turingMachine);
}

<stateElem>{
	\}			{
		yy_pop_state();
		Def->eMmatch = true;
	}

	{SPACES}	;

	[,<>]		;

	{SYMBOL}	Def->addToState(yytext[0]);

	{NAME}		Def->pushPointerSymbToState(yytext);

}

<elementaryMachine>{

	{SPACES}	;

	&{SYMBOL}\]	{
		Def->pushAddrEMachine(yytext[1]);
		yy_pop_state();
	}

	{SYMBOL}\]	{
		Def->pushSimpleEMachine(yytext[0]);
		yy_pop_state();
	}

	{ELEM}\]	{
		Def->pushPointerToEMachine(yytext);
		yy_pop_state();
	}

	{EMACHINE}	{
		yy_push_state(headRepositioningMachine);
		Def->setEMachineDir(yytext[0]);
	}
}

<headRepositioningMachine>{
	\]			{
		yy_pop_state();
		yy_pop_state();
		Def->eMmatch = true;
	}

	!			Def->eMmatch = false;

	{ELEM}		Def->pushPointerToEMachine(yytext);

	{SYMBOL}	Def->pushAdvMachine(yytext[0]);
	.			;
}

%%

int main(int argc, char** argv) {

	streambuf *backup;
	ifstream file;


	backup = cout.rdbuf();

	ofstream out;
	out.open ("turing.txt");
	cout.rdbuf(out.rdbuf());

	file.open(argv[1]);

	FlexParser *FlexP = new FlexParser();

	FlexP->switch_streams(&file);

	Parser *MParser = Parser::getInstance();
	MParser->setTape(argv[3]);
	MParser->printTape();

	while(FlexP->yylex() != 0);

//	Def->printTuringMachines();

	Def->solve(argv[2]);

	cout.rdbuf(backup);

		MParser->correctTape();

	out.close();
	file.close();
	return 0;	
}
