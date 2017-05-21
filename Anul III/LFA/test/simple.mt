alphabet :: a b # ;

; masini Turing simple, descrise folosind limbajul de baza

; copierea unui sir w format din a si b
; #>#w# -> #>#w#w#
Copy ::= loop@[R] (
  {a} -> [#] [R(#)] [R(#)] [a] [L(#)] [L(#)] [a] &loop ;
  {b} -> [#] [R(#)] [R(#)] [b] [L(#)] [L(#)] [b] &loop ;
  {#} -> [L(#)] ;
) ;;

; inversarea unui sir w format din a si b
; #>#w# -> #>#w#w^R#
Reverse ::= [R(#)] loop@[L] (
  {a} -> [#] [R(#)] [R(#)] [a] [L(#)] [L(#)] [a] &loop ;
  {b} -> [#] [R(#)] [R(#)] [b] [L(#)] [L(#)] [b] &loop ;
) ;;

; adunarea a doua numere reprezentate ca numar de a-uri
; #>#m#n# -> #>#m#n#m+n#
Add ::= start@[R] (
  {a} -> [#] [R(#)] [R(#)] [R(#)] [a] [L(#)] [L(#)] [L(#)] [a] &start ;
  {#} -> [Copy] [L(#)];
) ;;

; adunarea a doua numere reprezentate ca numar de a-uri, pastrand doar rezultatul pe banda
; #>#m#n# -> #>#m+n#
Add2 ::= [R(#)] [R] (
  {a} -> [L] [a] [R(#)] [L] [#] [L(#)] ;
  {#} -> [L(#)] [L(#)] ;
) ;;

; modulul diferentei a doua numerere reprezentate ca ca numar de a-uri, pastrand doar rezultatul pe banda
; #>#m#n# -> #>#m-n#
Subtract ::= loop@[R] (
  {a} -> [#] [R(#)] [R(#)] [L] (
    {a} -> [#] [L(#)] [L(#)] &loop ;
    {#} -> [L(#)] [a] [L] ;
  ) ;
) ;;

; produsul a doua numerere reprezentate ca numar de a-uri
; #>#m#n# -> #>#m#n#m*n#
Multiply ::= loop@[R] (
  {a} -> [#] [R(#)] [Copy] [L(#)] [a] &loop ;
  {#} -> [L(#)] ;
) ;;

