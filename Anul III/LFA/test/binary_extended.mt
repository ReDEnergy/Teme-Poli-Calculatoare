alphabet :: 0 1 # ;

void = # ;

digit := { 0, 1 } ;

; masini Turing care lucreaza cu numere in baza 2
; masinile sunt descrise folosind limbajul extins

; incrementarea unui numar
; #>#n# -> #>#n+1#
Increment ::= [R(<void>)] loop@[L] (
  !{1} -> [1] [L(<void>)] ;
  {1}  -> [0] &loop ;
) ;;

; decrementarea unui numar
; #>#n# -> #>#n-1#
Decrement ::= [R(<void>)] loop@[L] (
  {0} -> [1] &loop ;
  {1} -> [0] [L] (
    <digit>  -> [L(<void>)] ;
    {<void>} -> [R] [R] (
      <digit>  -> [L] [<void>] ;
      {<void>} -> [L(<void>)] ;
    ) ;
  ) ;
) ;;

; suma a doua numere, folosind incrementare si decrementare
; #>#m#n# -> #>#m+n#
Sum ::= loop@[R(<void>)] [R(!<void>)] (
  {0} -> [<void>] [L(!<void>)] [L(<void>)] ;
  {1} -> [L] [Decrement] [L(!<void>)] [L(<void>)] [Increment] &loop ;
) ;;

