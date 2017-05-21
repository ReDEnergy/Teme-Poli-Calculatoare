;	Ivanica Gabriel
;	323CA
;	Tema 3 - PP



;	Path - Reprezinta drumul sarit de minge peste oameni
;		 - Uneste 2 World
;		 - Format din : World + directie de deplasare
		 
(deftemplate path
	(multislot limit)	; dimensiune harta
	(multislot ball)	; pozitia mingii
	(multislot men) 	; pozitiile oamenilor
	(slot id) 			; id lume
	(multislot moves)	; mutari
	(multislot dir)		; directia de deplasare
)

;	Directiile de deplasare
;		 - 8 directii de deplasare pozibile	

(deffacts directions
	;N-V, N, N-E
	(dir -1 -1)
	(dir -1  0)
	(dir -1  1)

	;Nord, Sus
	(dir  0  1)
	(dir  0 -1)

	;S-V, S, S-E
	(dir  1 -1)
	(dir  1  0)
	(dir  1  1)
)


;	New-Path - Selecteaza un world si verifica posibiltatea de a crea un Path
;			 - Daca in directia de deplazare ?D este om, se construieste un Path
;			 - Se suprascrie pozitia omului cu pozitia bilei
	
(defrule new-path
	(declare (salience 10))
	(dir ?dx ?dy)
	(world (limit $?L) (id ?W) (ball ?bx ?by) (men $?s ?mx ?my - $?f) (moves $?M)) 
	(test (= ?mx (+ ?bx ?dx)))
	(test (= ?my (+ ?by ?dy)))
	=>
		(assert (path 
					(dir ?dx ?dy) 
					(id ?W) 
					(limit ?L)
					(ball (+ ?bx ?dx) (+ ?by ?dy)) 
					(men ?s ?f)
					(moves ?M)
				) 
		)
;	(printout t ?mx ?my crlf)
)


;	Continue-Path - Orice Path creat cu functia precenedenta este continuat
;				  - Directia de plecare dicteaza continuarea Path
;				  - Path se continua atat timp cat pe pozitia urmatoare se afla un om
;				  - Se suprascrie pozitia omului cu pozitia bilei

(defrule continue-path
	(declare (salience 9))
	?p <- (path (limit $?L) (id ?W) (dir ?dx ?dy) (ball ?bx ?by) (men $?s ?mx ?my - $?f) (moves $?M))
	(test (= ?mx (+ ?bx ?dx)))
	(test (= ?my (+ ?by ?dy)))
	=>
		(retract ?p)
		(assert (path 
					(dir ?dx ?dy) 
					(id ?W) 
					(limit ?L) 
					(ball (+ ?bx ?dx) (+ ?by ?dy)) 
					(men ?s ?f)
					(moves ?M)
			)
		)
)

;	New-World - Atunci cand nu se afla om pe directia de deplasare a Path
;			  - Se construieste un nou World
;			  - Se suprascrie pozitia libera cu pozitia bilei
;			  - Se adauga pozitia de salt la mutari


(defrule new-world
	(declare (salience 9))
	?p <- (path (limit $?L) (id ?W) (dir ?dx ?dy) (ball ?bx ?by) (men $?Men) (moves $?Moves))
	=>
		(assert (world 
					(id ?W) 
					(limit ?L) 
					(ball (+ ?bx ?dx) (+ ?by ?dy)) 
					(men ?Men)
					(moves ?Moves (+ ?bx ?dx) (+ ?by ?dy) - )
			)
		)
;		(printout t " *** end of path : " ?p crlf)
)	


;	Win-Moves - Cand o bila se afla pe ultima pozitie se considera mutare castigatoare
;			  - Se salveaza mutarea castigatoare
;			  - Se adauga pozitia de plecare a bilei la mutari
							
(defrule win-moves
	(declare (salience 20))
	(world (limit ?lx ?ly) (id ?W) (ball ?bx ?by) (moves $?M))
	(test (= (- ?lx 1) ?bx))
	(Ball ?W $?B)
	=>
		(assert (win (id ?W) (moves ?B - ?M)))
)


;	Initial-ball - Salveaza pozitia initiala a bilei inainte de plecare

(defrule initial-ball
	(declare (salience 30))
	(world (id ?W) (ball $?B) (moves))
	=>
		(assert (Ball ?W ?B))
)

;	Win-Solution - Cand se gaseste o solutie win se sterg restul de facts Path 
				 
(defrule Win-Delete-Path
	(declare (salience 25))
	(win (id ?W1))
	?p <- (path (id ?W1))
	=>
		(retract ?p)
)

;	Regulie de stergere facts, la terminarea rularii
;		- prioritate foarte mica
;		- se executa inainte de terminarea programului


;	Clear all facts Ball - pozitia initiala a mingii pe harta id
(defrule clear-initial-ball
	(declare (salience 1))
	?p <- (Ball $?)
	=>
		(retract ?p)
)


;	Clear all facts Path - daca mai sunt ramase (atunci cand nu exista solutie)
(defrule clear-path
	(declare (salience 1))
	?p <- (path (id ?))
	=>
		(retract ?p)
)

;	Clear all facts World - cele primite si toate celelalte create pe parcurs
(defrule clear-worlds
	(declare (salience 1))
	?p <- (world (id ?))
	=>
		(retract ?p)
)

;	Clear all facts Dir - directiile de deplasare posibile
(defrule clear-dir
	(declare (salience 1))
	?p <- (dir $?)
	=>
		(retract ?p)
)
