
\ pos forth79

: !	(store) ; core
: !CSP	( -- ) SP@ CSP ! ; core
: # (sharp) BASE @ M/MOD ROT 9 OVER < IF 7 + THEN 30 + HOLD ; core
: #> DROP DROP HLD @ PAD OVER - ;
: #S BEGIN # OVER OVER OR 0= UNTIL ;
: ' (tick) -FIND 0= 0 ERROR DROP [COMPILE] LITERAL ; IMMEDIATE
; ( LITERAL 29 WORD ; IMMEDIATE
: ." R@ COUNT DUP 1+ R> + >R TYPE ;
: +LOOP xxxx ;
: (;CODE) iR> LATEST PFA CFA ! ;
: (ABORT) ABORT ;
: (DO) >R >R ;
: (FIND) xxxx ;
: (LINE) xxxx :
: (LOOP) xxxx ;
: (NUMBER) 


