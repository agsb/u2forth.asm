

http://forth.sourceforge.net/standard/fst83/fst83-12.htm


          12.1 The Required Word Set Layers

          The words of the Required Word Set are grouped to show like
          characteristics.  No implementation requirements should be
          inferred from this grouping.


          Nucleus layer

              	!  *  */  */MOD  +  +!  -  /  /MOD  0<  0=  0>  1+  1-  2+
				2-  2/  <  =  >  >R  ?DUP  @  ABS  AND  C!  C@  CMOVE
				CMOVE>  COUNT  D+  D<  DEPTH  DNEGATE  DROP  DUP  EXECUTE
				EXIT  FILL  I  J  MAX  MIN  MOD  NEGATE  NOT  OR  OVER  PICK
				R>  R@  ROLL  ROT  SWAP  U<  UM*  UM/MOD  XOR

          Device layer

              	BLOCK  BUFFER  CR  EMIT  EXPECT  FLUSH  KEY  SAVE-BUFFERS
				SPACE  SPACES  TYPE  UPDATE

          Interpreter layer

              	#  #>  #S  #TIB  '  (  -TRAILING  .  .(  <#  >BODY  >IN
				ABORT  BASE  BLK  CONVERT  DECIMAL  DEFINITIONS  FIND
				FORGET  FORTH  FORTH-83  HERE  HOLD  LOAD  PAD  QUIT  SIGN
				SPAN  TIB  U.  WORD

          Compiler layer

              	+LOOP  ,  ."  :  ;  ABORT"  ALLOT  BEGIN  COMPILE  CONSTANT
				CREATE  DO  DOES>  ELSE  IF  IMMEDIATE  LEAVE  LITERAL  LOOP
				REPEAT  STATE  THEN  UNTIL  VARIABLE  VOCABULARY  WHILE
				[']  [COMPILE]  ]
