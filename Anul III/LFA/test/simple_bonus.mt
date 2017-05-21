alphabet :: a b # ;

void = # ;

letter := { a, b };

; masini Turing simple, descrise folosind limbajul extins, precum si variabile

; copierea unui sir w format din a si b
; #>#w# -> #>#w#w#
Copy ::= loop@[R] (
  x@<letter> -> [#] [R(#)] [R(#)] [&x] [L(#)] [L(#)] [&x] &loop ;
  {<void>} -> [L(<void>)] ;
) ;;

; inversarea unui sir w format din a si b
; #>#w# -> #>#w#w^R#
Reverse ::= [R(#)] loop@[L] (
  x@<letter> -> [#] [R(#)] [R(#)] [&x] [L(#)] [L(#)] [&x] &loop ;
) ;;

