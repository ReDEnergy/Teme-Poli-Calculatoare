
	Ivanica Gabriel 333CA


	LFA - Parsarea si executia unei masini Turing


	1. Implementare


	Pentru Masini turing am creat o clasa de baza TuringMachine care extinde toate
masinile Turing mai avansate.
	Masina Turing are urmatoarele facilitati :
		- run() executa masina turing curenta
		- jump() sare la masina turing salvata precedent ( loop@ )
		- call() executa un apel catre o alta masina Turing


	Orice masina turing are un pointer:
		- next ce indica umatoarea masina ce trebuie apelata


	O declaratie de masina turing ::= va reprezentata un lant de masini Turing
cu next() , jump() si call();

	Tipuri masini turing folosite:
		- msini elementare : clasa ElemMachine
				-	deplaseaza stanga/dreapta banda
				-	scrie un element pe bada
				-	deplaseaza staga/dreapta pana la primul symbol gasit/diferit de cel dat

		- masini cu stari : clasa StateMachine
				-	fiecare stare reprezinta o alta masina Turing elementara
				-	starile retinute pe 2 seturi pentru potrifire/diferit de symbolul curent
				
	Exe :

	Copy ::= loop@[R] (
		x@<letter> -> [#] [R(#)] [R(#)] [&x] [L(#)] [L(#)] [&x] &loop ;
		{<void>} -> [L(<void>)] ;
	) ;;

	Masina Turing Copy => 
		next -> [R]
	
	[R]-> masina turing cu starile (<letter> <void>) ce vor fi la randul lor alte masini turing



	2.	Parsare

		Parsarea am facut-o utilizand stari exclusive pentru a trece dintr-o stare in alta si a
	fi foarte simplu sa construiesc Definitiile de masini turing si apelurile necesare cat si salvarea
	lor.

		%x alphabetdecl
		%x declarationType
		%x symbolSet
		%x symbolDecl
		%x turingMachine
		%x stateMachine
		%x stateElem
		%x elementaryMachine
		%x headRepositioningMachine


		Pentru a salva tranzitiile x@  y@ am folosit un map si am generat cate un id unic pentru
	fiecare variabila gasita pentru a nu incurca variabilele x@ in cadrul apelarii recursive a mai multor
	masini.
		