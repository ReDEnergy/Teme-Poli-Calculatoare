; descrierea lumii
(deftemplate world
	(multislot limit) ; max size (linii, coloane)
	(multislot ball) ; minge
	(multislot men) ; pozitiile una dupa alta, x y -
	(slot id) ; identificator pt lume
	(multislot moves) ; lista mutarilor, initial vida
	)

; descrierea unui win-record
(deftemplate win
	(slot id) ; identificatorul lumii din care am castigat
	(multislot moves) ; lista mutarilor ce au dus la succes
	)

