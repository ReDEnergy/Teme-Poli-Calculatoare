alphabet :: a b 0 1 2 3 4 5 6 7 8 9 # $ @ * ;

void    = # ;
sep     = $ ;
cursor  = @ ;
count   = * ;

special := { #, $, @, * } ;
digit   := { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } ;
any     := { a, b, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, #, $, @, * } ;

_goto_n ::= loop@[R(<void>)] [<cursor>] [L(<sep>)] [L(<sep>)] [<void>] [R] (
  {<count>} -> [<sep>] [R(<cursor>)] [<void>] &loop ;
  {<void>}  -> [<sep>] [R(<cursor>)] [<void>] ;
) ;;

_mem_goto_n ::= loop@[R(<void>)] [<cursor>] [L(<sep>)] [L(<sep>)] [<void>] [R] (
  {<count>} -> [<sep>] [R(<cursor>)] [<void>] &loop ;
  {<void>}  -> [<sep>] [R(<cursor>)] [<void>] ;
) ;;

_io_goto_n ::= [R(<sep>)] loop@[R(<void>)] [<cursor>] [L(<sep>)] [L(<sep>)] [L(<sep>)] [<void>] [R] (
  {<count>} -> [<sep>] [R(<sep>)] [R(<sep>)] [R(<cursor>)] [<void>] &loop ;
  {<void>}  -> [<sep>] [R(<sep>)] [R(<sep>)] [R(<cursor>)] [<void>] ;
) ;;

; presupune existenta unui cursor la capatul benzii si pozitia capului poate fi oriunde pe banda, dar la stanga cursorului
_expandr_C_ ::= (
  x@<any> -> [<cursor>] [R(<cursor>)] loop@[L] (
    y@!{<cursor>} -> [R] [&y] [L] &loop ;
    {<cursor>}    -> [R] [&x] [L] [<void>] ;
  ) ;
) ;;

; translateaza partea stanga a benzii cu o pozitie la stanga si face loc pentru un # pe pozitia curenta
_expandl_C_ ::= (
  x@<any> -> [<cursor>] [L(<cursor>)] loop@[R] (
    y@!{<cursor>} -> [L] [&y] [R] &loop ;
    {<cursor>}    -> [L] [&x] [R] [<void>] ;
  ) ;
) ;;

_shrinkr_C_ ::= [<cursor>] [R] (
  x@<any> -> loop@[R] (
    y@!{<cursor>} -> [L] [&y] [R] &loop ;
    {<cursor>}    -> [<void>] [L] [<void>] [L(<cursor>)] [&x] ;
  ) ;
) ;;

_shrinkl_C_ ::= [<cursor>] [L] (
  x@<any> -> loop@[L] (
    y@!{<cursor>} -> [R] [&y] [L] &loop ;
    {<cursor>}    -> [<void>] [R] [<void>] [R(<cursor>)] [&x] ;
  ) ;
) ;;

_mem_expandl_ ::= (
  x@<any> -> [<cursor>] [L(<sep>)] [L(<sep>)] [L] [<cursor>] [R(<cursor>)] [&x] [_expandl_C_] ;
) ;;

_mem_expandr_ ::= (
  x@<any> -> [<cursor>] [R(<sep>)] [R(<sep>)] [R] [<cursor>] [L(<cursor>)] [&x] [_expandr_C_] ;
) ;;
  
_mem_shrinkl_ ::= (
  x@<any> -> [<cursor>] [L(<sep>)] [L(<sep>)] [L] [<cursor>] [R(<cursor>)] [&x] [_shrinkl_C_] ;
) ;;

_mem_shrinkr_ ::= (
  x@<any> -> [<cursor>] [R(<sep>)] [R(<sep>)] [R] [<cursor>] [L(<cursor>)] [&x] [_shrinkr_C_] ;
) ;;

_io_expandl_ ::= (
  x@<any> -> [<cursor>] [L(<sep>)] [L(<sep>)] [L(<sep>)] [L] [<cursor>] [R(<cursor>)] [&x] [_expandl_C_] ;
) ;;

_io_expandr_ ::= (
  x@<any> -> [<cursor>] [R(<sep>)] [R] [<cursor>] [L(<cursor>)] [&x] [_expandr_C_] ;
) ;;

_io_shrinkl_ ::= (
  x@<any> -> [<cursor>] [L(<sep>)] [L(<sep>)] [L(<sep>)] [L] [<cursor>] [R(<cursor>)] [&x] [_shrinkl_C_] ;
) ;;

_io_shrinkr_ ::= (
  x@<any> -> [<cursor>] [R(<sep>)] [R] [<cursor>] [L(<cursor>)] [&x] [_shrinkr_C_] ;
) ;;

_mem_clear_ ::= [R(<void>)] [L] loop@(
  !{<void>} -> [_mem_shrinkl_] &loop ;
) ;;

_io_clear_ ::= [R] loop@(
  !{<void>} -> [_io_shrinkr_] &loop ;
  {<void>}  -> [L] ;
) ;;

io_memread_nn ::= [_mem_goto_n] [<cursor>] [_io_goto_n] [_io_clear_] [R] [<cursor>] [L(<cursor>)] [<void>] loop@[R] (
  x@!{<void>} -> [<cursor>] [R(<cursor>)] [_io_expandr_] [&x] [L(<cursor>)] [&x] &loop ;
  {<void>}    -> [R(<cursor>)] [<void>] [L(<sep>)] [L(<sep>)] ;
) ;;

mem_iosave_nn ::= [_mem_goto_n] [_mem_clear_] [<cursor>] [_io_goto_n] [R(<void>)] loop@[L] (
  x@!{<void>} -> [<cursor>] [L(<cursor>)] [_mem_expandl_] [&x] [R(<cursor>)] [&x] &loop ;
  {<void>}    -> [L(<cursor>)] [<void>] [L(<sep>)] ;
) ;;

mem_alloc_n ::= loop@[L(<sep>)] [<void>] [R] (
  {<count>} -> [<sep>] [R(<sep>)] [R] [_mem_expandl_] [L(<sep>)] &loop ;
  {<void>}  -> [<sep>] [R(<sep>)] ;
) ;;

mem_free_n ::= loop@[L(<sep>)] [<void>] [R] (
  {<count>} -> [<sep>] [R(<sep>)] [R] [_mem_clear_] [_mem_shrinkr_] [L(<sep>)] &loop ;
  {<void>}  -> [<sep>] [R(<sep>)] ;
) ;;

io_alloc_n ::= loop@[L(<sep>)] [<void>] [R] (
  {<count>} -> [<sep>] [R(<sep>)] [R(<sep>)] [R] [_io_expandr_] [L(<sep>)] [L(<sep>)] &loop ;
  {<void>}  -> [<sep>] [R(<sep>)] ;
) ;;

io_free_n ::= loop@[L(<sep>)] [<void>] [R] (
  {<count>} -> [<sep>] [R(<sep>)] [R(<sep>)] [R] [_io_clear_] [_io_shrinkr_] [L(<sep>)] [L(<sep>)] &loop ;
  {<void>}  -> [<sep>] [R(<sep>)] ;
) ;;

_put_ ::= [<count>] [L] ;;

ctl_put_0 ::= [L(<sep>)] [<void>] [L]                                                         [<sep>] [R(<sep>)] ;;
ctl_put_1 ::= [L(<sep>)] [<void>] [L] [_put_]                                                 [<sep>] [R(<sep>)] ;;
ctl_put_2 ::= [L(<sep>)] [<void>] [L] [_put_] [_put_]                                         [<sep>] [R(<sep>)] ;;
ctl_put_3 ::= [L(<sep>)] [<void>] [L] [_put_] [_put_] [_put_]                                 [<sep>] [R(<sep>)] ;;
ctl_put_4 ::= [L(<sep>)] [<void>] [L] [_put_] [_put_] [_put_] [_put_]                         [<sep>] [R(<sep>)] ;;
ctl_put_5 ::= [L(<sep>)] [<void>] [L] [_put_] [_put_] [_put_] [_put_] [_put_]                 [<sep>] [R(<sep>)] ;;
ctl_put_6 ::= [L(<sep>)] [<void>] [L] [_put_] [_put_] [_put_] [_put_] [_put_] [_put_]         [<sep>] [R(<sep>)] ;;
ctl_put_7 ::= [L(<sep>)] [<void>] [L] [_put_] [_put_] [_put_] [_put_] [_put_] [_put_] [_put_] [<sep>] [R(<sep>)] ;;

; ####################################################################################################################

Zero ::= [ctl_put_0] [_io_goto_n] [_io_clear_] [_io_expandl_] [0] [L(<sep>)] [L(<sep>)] ;;

Increment ::= [R(<sep>)] [R(<void>)] [R(<void>)] loop@[L] (
  {0}      -> [1]  [L(<sep>)] [L(<sep>)] ;
  {1}      -> [0] &loop ;
  {<void>} -> [_io_expandl_] [1] [L(<sep>)] [L(<sep>)] ;
) ;;

Increment_10 ::= [R(<sep>)] [R(<void>)] [R(<void>)] loop@[L] (
  {0}      -> [1]  [L(<sep>)] [L(<sep>)] ;
  {1}      -> [2]  [L(<sep>)] [L(<sep>)] ;
  {2}      -> [3]  [L(<sep>)] [L(<sep>)] ;
  {3}      -> [4]  [L(<sep>)] [L(<sep>)] ;
  {4}      -> [5]  [L(<sep>)] [L(<sep>)] ;
  {5}      -> [6]  [L(<sep>)] [L(<sep>)] ;
  {6}      -> [7]  [L(<sep>)] [L(<sep>)] ;
  {7}      -> [8]  [L(<sep>)] [L(<sep>)] ;
  {8}      -> [9]  [L(<sep>)] [L(<sep>)] ;
  {9}      -> [0] &loop ;
  {<void>} -> [_io_expandl_] [1] [L(<sep>)] [L(<sep>)] ;
) ;;

Decrement ::= [R(<sep>)] [R(<void>)] [R(<void>)] loop@[L] (
  {0}      -> [1] &loop ;
  {1}      -> [0] [L] (
    {0, 1}   -> [L(<sep>)] [L(<sep>)] ;
    {<void>} -> [R] [R] (
      {0, 1}   -> [L] [_io_shrinkl_] [L(<sep>)] [L(<sep>)] ;
      {<void>} -> [L(<sep>)] [L(<sep>)] ;
    ) ;
  ) ;
) ;;

Decrement_10 ::= [R(<sep>)] [R(<void>)] [R(<void>)] loop@[L] (
  {9} -> [8] [L(<sep>)] [L(<sep>)] ;
  {8} -> [7] [L(<sep>)] [L(<sep>)] ;
  {7} -> [6] [L(<sep>)] [L(<sep>)] ;
  {6} -> [5] [L(<sep>)] [L(<sep>)] ;
  {5} -> [4] [L(<sep>)] [L(<sep>)] ;
  {4} -> [3] [L(<sep>)] [L(<sep>)] ;
  {3} -> [2] [L(<sep>)] [L(<sep>)] ;
  {2} -> [1] [L(<sep>)] [L(<sep>)] ;
  {0} -> [9] &loop ;
  {1}      -> [0] [L] (
    <digit>   -> [L(<sep>)] [L(<sep>)] ;
    {<void>} -> [R] [R] (
      <digit>   -> [L] [_io_shrinkl_] [L(<sep>)] [L(<sep>)] ;
      {<void>} -> [L(<sep>)] [L(<sep>)] ;
    ) ;
  ) ;
) ;;

Sum ::= [ctl_put_2] [mem_alloc_n]
        [ctl_put_0] [ctl_put_0] [mem_iosave_nn]
        [ctl_put_1] [ctl_put_1] [mem_iosave_nn]
        [ctl_put_1] [io_free_n]
        loop@[ctl_put_0] [_mem_goto_n] [R] (
	  {0} -> [L(<sep>)] [ctl_put_0] [ctl_put_1] [io_memread_nn] [ctl_put_2] [mem_free_n] ;
	  {1} -> [L(<sep>)] [ctl_put_0] [ctl_put_0] [io_memread_nn] [Decrement] [ctl_put_0] [ctl_put_0] [mem_iosave_nn]
	                    [ctl_put_0] [ctl_put_1] [io_memread_nn] [Increment] [ctl_put_0] [ctl_put_1] [mem_iosave_nn] &loop ;
	) ;;

Product ::= [ctl_put_3] [mem_alloc_n]
            [ctl_put_0] [ctl_put_0] [mem_iosave_nn]
            [ctl_put_1] [ctl_put_1] [mem_iosave_nn]
            [ctl_put_1] [io_free_n]
            [Zero] [ctl_put_0] [ctl_put_2] [mem_iosave_nn]
            loop@[ctl_put_0] [_mem_goto_n] [R] (
	      {0} -> [L(<sep>)] [ctl_put_0] [ctl_put_2] [io_memread_nn] 
	                        [ctl_put_3] [mem_free_n] ;
	      {1} -> [L(<sep>)] [ctl_put_0] [ctl_put_0] [io_memread_nn]
	                        [Decrement]
	                        [ctl_put_0] [ctl_put_0] [mem_iosave_nn]
	                        [ctl_put_1] [io_alloc_n]
		                [ctl_put_0] [ctl_put_1] [io_memread_nn]
		                [ctl_put_1] [ctl_put_2] [io_memread_nn]
		                [Sum]
		                [ctl_put_0] [ctl_put_2] [mem_iosave_nn] &loop ;
	    ) ;;

Convert_dec2bin ::= 
  [ctl_put_2] [mem_alloc_n]
  [ctl_put_0] [ctl_put_0] [mem_iosave_nn]
  [Zero] [ctl_put_0] [ctl_put_1] [mem_iosave_nn]
  loop@[ctl_put_0] [_mem_goto_n] [R] (
    {0}  -> [L(<sep>)] [ctl_put_0] [ctl_put_1] [io_memread_nn]
                       [ctl_put_2] [mem_free_n] ;
    !{0} -> [L(<sep>)] [ctl_put_0] [ctl_put_0] [io_memread_nn]
                       [Decrement_10]
                       [ctl_put_0] [ctl_put_0] [mem_iosave_nn]
                       [ctl_put_0] [ctl_put_1] [io_memread_nn]
                       [Increment]
                       [ctl_put_0] [ctl_put_1] [mem_iosave_nn] &loop ;
  ) ;;
	    
Convert_bin2dec ::= 
  [ctl_put_2] [mem_alloc_n]
  [ctl_put_0] [ctl_put_0] [mem_iosave_nn]
  [Zero] [ctl_put_0] [ctl_put_1] [mem_iosave_nn]
  loop@[ctl_put_0] [_mem_goto_n] [R] (
    {0}  -> [L(<sep>)] [ctl_put_0] [ctl_put_1] [io_memread_nn]
                       [ctl_put_2] [mem_free_n] ;
    !{0} -> [L(<sep>)] [ctl_put_0] [ctl_put_0] [io_memread_nn]
                       [Decrement]
                       [ctl_put_0] [ctl_put_0] [mem_iosave_nn]
                       [ctl_put_0] [ctl_put_1] [io_memread_nn]
                       [Increment_10]
                       [ctl_put_0] [ctl_put_1] [mem_iosave_nn] &loop ;
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