alphabet :: a b # ;

void = # ;
cursor  = @ ;
letter := { a, b };
any := { a, b, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, #, $, @, * } ;


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

Product_10 ::= [ctl_put_2] [mem_alloc_n]
               [ctl_put_0] [ctl_put_0] [mem_iosave_nn]
               [ctl_put_1] [io_free_n]
               [Convert_dec2bin]
               [ctl_put_0] [ctl_put_1] [mem_iosave_nn]
               [ctl_put_0] [ctl_put_0] [io_memread_nn]
               [Convert_dec2bin]
               [ctl_put_1] [io_alloc_n]
               [ctl_put_0] [ctl_put_1] [io_memread_nn]
               [Product]
               [Convert_bin2dec]
               [ctl_put_2] [mem_free_n]
               ;;