
**Some thoughts and notes for u2forth.asm in a atmega8, a list in no particular order, needs revisions, still a work in progress, things can change, use as you please and at own risk, with no warranty any.**

0. "The most popular architectures used to implement Forth have had byte-addressed memory, 16-bit operations, and two's-complement number representation." https://www.taygeta.com/forth/dpanse.htm as ANSI X3.215-1994

1. For a ATMEGA8, a forth CELL is 2 bytes, or 16 bits;

2. The instructions adiw and sbiw, only works in r25:r24, r27:r26, r29:r28, r31:r30;

3. Used as  8 bits registers: 

        r0 work register, 
        r1 zero register, 
        r2-r4 used by instructions
        r5-r15 free;

4. Used as 16 bits registers: 

        r16-r19 free (but used in 16 bit multiplications routines from AVR)
        r21:r20 tos first cell of psp, as T 
        r23:r22 nds second cell of psp, as N 
        r25:r24 wrk working generic, as W 
        r27:r26 rsp return stack, as X
        r29:r28 psp parameter stack, as Y
        r31:r30 ipp instruction pointer, as Z, ever points to an address.

5. why using explict TOS, NDS, WRK is more convenient ? 
    
        macro push uses 4 cycles, (2 for each 8bit register)
        macro pull uses 4 cycles, (2 for each 8bit register)
        instr sbiw uses 2 cycles
        instr adiw uses 2 cycles
        instr movw uses 1 cycle
        instr oper uses 1 cycle
    
    with pre loaded T, N, oper results in T 
     
        push into psp (6): push N, movw N T, movw T W,
        pull from psp (5): movw T N, pull N
        one argument (1): oper (T)
        two argument (5): oper (T N ), pull N
        three argument (9): pull W, oper (T N W), pull N
         
    without and oper results in T      
        
        push into psp (4): push W
        pull from psp (4): pull W
        one argument (9): pull T, oper (T), push T
        two argument (13): pull T, pull N, oper (T N), push T
        three argument (17): pull T, pull N, pull W, oper (T N W), push T
            
     yes, and more simple and more eficient.
     
6. Also uses 12 bit pc program counter, 16 bit sp stack pointer, 8 bit alu aritmetic logic unit, 8 bit status register.
  
7. all primitives are coded in assembler of a specif family of CPU or uC, and  must end with ????zzz

8. when forth concept is to be a real VM then no assembler is allowed inside dictionary, except for primitives.
  
9. for standart forth-2012 all public WORDS are in uppercase, so all user WORDS will be in upppercase too;
  
10. only words inside forth engine could mix uppercase and lowercase and can those words could not be redefined;
  
11. In march 1992, http://www.forth.org/fd/FD-V13N6.pdf shows an article about comparing Forth systems, using NEXT, NEST, UNNEST, DOLIT, DOCON, DOVAR, @ and ! as representative of current implementations
  
12. the dictionary interleaves NAME and CODE but some forths uses separate a Name dictionary and a Code dictionary. 
  
13. hidden inside engine, forth has following words, or functions, or routines. 
  
    Core, reads a opcode at ips pointed address, increase ips by ONE, and execute opcode.
   
    Enter, reads into wrk an address pointed by ips address, increase ips by CELL, push ips into rsp, copy wrk to ips, go to Next
    Exit,  pull ips from rsp, go to Exec
    Exec, reads wrk from psp, push ips into rsp, copy wrk to ips, go to Next ??? 
    
    Jump, is a jump to next address in dictionary
    Jumpz, is a jump to next address in dictionary only when tos is zero, pull from psp.
   
    Dolit,
    Docon,
    Dovar,
    Dovoc,
    
14. traditional eForth $NEXT is Next, $NEST or $DOCOL or $ENTER is Enter, $UNNEST is Exit, BRANCH is Jump, ?BRANCH is Jumpz, those words are never used by user, only by forth engine.
  
15. do not confuse with standart, or not, EXEC, NEXT, EXIT

16. include DEFER and IS.

        NODEF creates a dictionary word tha emits "Word not defined: WORD"
        DEFER creates a dictionary entry with a fake CFA
        IS pushes a CFA of a defined word into a CFA of a deffered word

17. Try use r19:r18 as return register for emulate riscV jal and jalr, single instruction to jump and save return address, jump and link in leaf ?
                
18. In Cortex M4, esp32, Dr. C.H.Ting uses a optimal aprouach for forth engine, but with those specific ISA inside dictionary.

        _next:    BX LR                 ; branch to ptr in LR, link register
        _nest:    STMFD R2!, {LR}       ; push LR into return stack
        _unnest:  LDMFD R2!, {PC}       ; pull PC from return stack
        BL ptr                          ; branch and link, ( mov LR ptr, inc LR, inc LR, jmp ptr)
        
        and dictionary parameters fields made as:
        
        leaf ==> instr, instr, instr, instr, _next
        twig ==> _nest, BL ptr, BL ptr, BL ptr _unest
        
19. In ATMEGA8, those can be made with
        ; (instruction) pointer Z is r31:r30, 
        ; (parameter stack) pointer Y is r29:r28, 
        ; (return stack) pointer X is r27:r26,
         ; (reference) pointer using r19:r18 
        
        _enter: ; same as BL  
                ld r24, Z+       ; load indirect
                ld r25, Z+       ; load indirect
                movw r18, r30    ; preserve return
                movw r30, r24    ; prepare reference to jump
                ijmp             ; jump immediate
               
        _exit: ; same as BX LR
                movw r30, r18    ; restore return
                ijmp             ; jump immediate
               
        _nest: ; same as STLPM Rn!, {LR}
                ld r24, Z+       ; load indirect
                ld r25, Z+       ; load indirect
                st -X, r18       ; push return
                st -X, r19       ; push return
                movw r30, r24    ; prepare reference to jump
                ijmp             ; jump immediate
               
        _unnest: sme as LDLPM Rn!, {PC}
                ld r31, X+       ; pull return
                ld r30, X+       ; pull return
                ijmp             ; jump immediate
               
        but how decide what to do with any word ?
                
                primitive: instr, instr, ... instr, macro_exit
                
                leaf:   ptr_enter, primitive_ptr, ????
                
                twig:   ptr_nest, ptr, ptr, ptr, ... ptr, ptr_unnest
                
         need decide names.       
                
