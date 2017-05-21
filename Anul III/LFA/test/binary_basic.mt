alphabet :: 0 1 # ;

; masini Turing care lucreaza cu numere in baza 2
; masinile sunt descrise folosind limbajul de baza

; incrementarea unui numar
; #>#n# -> #>#n+1#
Increment ::= [R(#)] loop@[L] (
  {0, #} -> [1] [L(#)] ;
  {1}    -> [0] &loop ;
) ;;

; decrementarea unui numar
; #>#n# -> #>#n-1#
Decrement ::= [R(#)] loop@[L] (
  {0} -> [1] &loop ;
  {1} -> [0] [L] (
    {0, 1} -> [L(#)] ;
    {#}    -> [R] [R] (
      {0, 1} -> [L] [#] ;
      {#}    -> [L(#)] ;
    ) ;
  ) ;
) ;;

; suma a doua numere, folosind incrementare si decrementare
; #>#m#n# -> #>#m+n#
Sum ::= loop@[R(#)] [R(!#)] (
  {0} -> [#] [L(!#)] [L(#)] ;
  {1} -> [L] [Decrement] [L(!#)] [L(#)] [Increment] &loop ;
) ;;

; suma a doua numere, calculata direct
Sum2 ::= [R(#)] loop@[L] (
  {#} -> [#] [R(#)] [R(#)] [L] (
    {#} -> [L(#)] ;
    {0} -> [#] [L(#)] [L(#)] [0] &loop ;
    {1} -> [#] [L(#)] [L(#)] [1] &loop ;
  ) ;
  {0} -> [#] [R(#)] [R(#)] [L] (
    {#} -> [L(#)] [0] [L(#)] ;
    {0} -> [#] [L(#)] [L(#)] [0] &loop ;
    {1} -> [#] [L(#)] [L(#)] [1] &loop ;
  ) ;
  {1} -> [#] [R(#)] [R(#)] [L] (
    {#} -> [L(#)] [1] [L(#)] ;
    {0} -> [#] [L(#)] [L(#)] [1] &loop ;
    {1} -> [#] [L(#)] [L(#)] [0] carry@[L] (
      {#} -> [R(#)] [R(#)] [L] (
	{#} -> [L(#)] [1] [L(#)] ;
	{0} -> [#] [L(#)] [L(#)] [1] &loop ;
	{1} -> [#] [L(#)] [L(#)] [0] &carry ;
      ) ;
      {0} -> [#] [R(#)] [R(#)] [L] (
	{#} -> [L(#)] [1] [L(#)] ;
	{0} -> [#] [L(#)] [L(#)] [1] &loop ;
	{1} -> [#] [L(#)] [L(#)] [0] &carry ;
      ) ;
      {1} -> [#] [R(#)] [R(#)] [L] (
	{#} -> [L(#)] [0] &carry ;
	{0} -> [#] [L(#)] [L(#)] [0] &carry ;
	{1} -> [#] [L(#)] [L(#)] [1] &carry ;
      ) ;
    ) ;
  ) ;
) ;;

