b32b6114-2abc-11eb-8261-1be5f55f2874 Thu, 19 Nov 2020 20:12:31 -0300
# https://github.com/agsb/u2forth.git

## u2forth.asm

  Another forth virtual machine, simple and minimal.
  
  But this one is for use into AVR microcontroler ATmega8, 
  with RISC 32 8bit registers [1], Harvard model, 8 MHz internal clock, 
  8k bytes of FLASH, 1k bytes of SRAM and 512 bytes for EEPROM.
  
  This is a experience in progress, using avr-gcc -mmcu=atmega8 and to make a startup code for avr-as

  Thanks to Jonasforth, as great essay of how to do a forth.
  
  *this is a tribute to G. H. Ting, and his books, papers and perseverance.*
  
  SURE, this readme needs updates !
  
  __Not operational yet 04/2021__
  
  # Goals
 
  All focus in be simple, be smaller and be smart for ATmega8
  
  To self learn about (AVR) RISC assembler, using avr-gcc with "trampolines" and diferent levels of optimzations, to learn howto.
  
  *"Programmers can use trampolined functions to implement tail-recursive function calls in stack-oriented programming languages.[1]"*
  
  u2forth is based in eForth dictionary from Dr. Ting, except for
 
         1. using Indirect Thread Code (?), to minimize size of all 
         2. using interrupts, to have a live system (**)
         3. have a timer in miliseconds, vide 2 (**)
         4. do not use real PC and SP, vide 2
         5. forth core words and constants in flash, as imutable
         6. forth user words and constants in sram, as wipes to nothing
         7. all variables in sram, vide 6
         8. use of poolling USART, vide 2
         9. use of poolling I2C, vide 2 (**)
        10. NO checks for ANY words
 
        ** not yet;
 
 # Virtual CPU and uC
 
A really virtual implementation of a forth CPU implies in NO specific opcodes of any real CPU into dictionary.
 
No assembler instructions, nor calls, nor jumps, nor returns, not at all.
 
The forth virtual cpu does opcodes defined by one byte, and executes the code from a indexed table of rotines defined in real assembler (ISA) for a family of real microprocessors. 

This may be a bit slow than specific optmized code, but is really portable as just the primitives must be coded in real assembler and all is done, because dicionary is immutable. 

# Dictionary
 
**the dictionary words can be leafs or twigs, only.**
         
          the leafs only contain bytecodes,
         
          the twigs only contains address to words.
 
 The leafs are sequences (threads) of real cpu code without any calls or any jumps inside, else at end. 
 
 The twigs are sequences of reference address for calls executed in order. 
 
 In RiscV cpu opcodes, leafs are equivalent to *link and leave*  and twigs to *call and return* ;
 
 WARNING: Goal is create a small object code for make a handcraft assembled forth for atmega8
  
 WARNING: Much code was borrowed from internet, with GPL licence, please help me to get all references. 

# 

  Still now, the dictionary will be, in no especific order,
    
   _primitives_
    
    NOOP=0, 
    
    THIS, ( literal LIT )
    
    CODE, ( leaf interpreter, the uC )
    
    NEXT, ENTER, EXIT, ( twig interpreter )
    
    JUMP, JUMPNZ, ( branch, ?branch, )
    
    RTP, R2P, P2R, ( R@, R>, >R, )
    
    DUPNZ, DUP, DROP, SWAP, OVER, ROT, ( as is )
    
    EQZ, LTZ, GTZ, ( =0, <0, >0, )
    
    AND, OR, XOR, INV, CPL, SHL, SHR, ( and, or, xor, inverse, negate, 2*, 2/, )
    
    PLUS, MINUS, MULTIPLY, DIVIDE, ( +, -, *, /, )
    
    UPLUS, UMULTIPLY, ( unsigned )
    
    /MOD, UM/MOD, ( reminder of unsigned division, quontient and reminder unsigned division )
    
    DROP2, DUP2, ( 2DROP, 2DUP, as is )
    
    CSTORE, CFETCH, STORE, FETCH, ( C!, C@, !, @, )
    
    CMOVE, CCOMP, MOVE, COMP, ( CMOVE, CCOMPARE, MOVE, COMPARE, moves and compares )
    
   _constants_
    
    RSP0, PSP0, TIB0, PAD0, PAD1, ( forth address constants )
    
    CELL, FALSE, TRUE, PADSZ, TIBSZ ( forth values constants )

    ZERO, ONE, TWO, ( usefull values // 0, 1, 2 in decimal )
    
    BELL, BS, TAB, LF, FF, CR, ESC, BL ( ascii usefull constants // 7, 8, 9, 10, 12, 13, 27, 32, in decimal )

  _variables_
  
    UP, DP, UPR, DPR,  ( forth address variables, user and dicionary for flash, user and dicitonary for sram )
    
    STATE, BASE,   ( forth variables )
   
  _system_
  
    RXQU, RXCU, TXCU, IOSU, ( ?rx, rx@, tx@, io_, // pooled USART ) 
    
    SETP, GETP, (basic I/O pins)
  
  _forth_
  
    TICK, COMMA, ( ' and , )
    
    COLON, SEMICOLON, HERE, LAST, 
    
    RESET, BYE, COLD, QUIT, QUERY, EXPECT, PARSE, EVAL, NAME?, NUMBER?, LITERAL, COMPILE, EXECUTE, ABORT,
  
    etc
    
    (more words ...)
    
# Words

The dictionary of words follow the classic structure, but lives part in FLASH, core imutable, and in SRAM, user temporary. 

  *link*, adress of previous word, as linked list, zeros at last one;
  
  *size*, number of characters of this word, limited to 31 chars (0x1F), and flags for IMMEDIATE (0x80), EXCLUSIVE (0x40), HIDDEN (0x20);
  
  *word*, the characters, with pad byte 0 when size is odd;
  
  *opcode*, a single byte with opcode for virtual uC, 0 Nop, 1 Leaf, 2 Twig, 3 - 7 reserved, 8 - 254 words coded in assembler
  
  *parameters*, a set of opcodes (if leaf) or a set of address of words (if twig)
  
As ATmega8 have only 8k bytes of FLASH then: 
  still no support for compile new words into FLASH;
  still no support for use eeprom;
  sure all goes to SRAM, all goes to void.

(Sure, there are lots of documents showing how to do those, including Dr. Ting books, AVR manuals, but reviews will be done.)

# Constants and Variables

Those could be defined in assembler as .EQU for constants and as .DW for variables, 
but must be pushed at top of parameter stack

The forth internal constants are in flash

  address: PSP0, RSP0, TIB0, PAD0, as start of parameter (data) stack, return stack, terminal input buffer, scratch-pad buffer.
  
  values: CELL, FALSE, TRUE, ZERO, ONE, TWO, as 2 bytes, 0 zero, -1, 0, 1, 2
    
  ascii:  space, \b, \n, \r, \e, as ascii codes for whitespace, backspace, new line, carriage return, escape
  
  ascii:  \, (, ), for comments
  
  ascii: ", for strings 

The forth internal variables are in sram

  STATE, BASE, LAST, as compilation or execution state, number system base, where is last word.
  
  NP, UP, CP, SPAN, HLOD, CONTEXT, CURRENT,   

# Details

  *In Risc-V, x0 is always zero.*

  Most of Forths are created in 1980 decade, for CPUs with few registers and diferent cycles per instructions, and uses a classic virtual model as (under) but uses internal CPU registers extras to scratch and speed.
  
  __IP__    index pointer
  
  __RSP__   return stack pointer
  
  __PSP__   parameter stack pointer
  
  __TOS__   top (first cell) of parameter stack
  
  __W__     work cell scratch
  
  Nowadays, most RISC cpus have at least 32 registers plus Program Counter, Stack Pointer and Status Register, and same cycles per instructions, almost, then to optimize the overhead of memory access, was adopted a virtual model with upto 255 bytes opcodes and registers defined as (under) with ATmega8 registers.
 
  *_work_*    temporary opcode instruction byte, r00   
  
  *_zero_*  reserved and always 0, r01, ( also modern RISC V uses X00 hardwired as ZERO )
  
  *ip*      instruction index pointer, r30r31 also know as Z register
  
  *psp*     parameter stack pointer index (rsp), r28r29, also know as Y register
  
  *rsp*     return stack pointer index (psp), r26r27, also know as X register
  
  *wrk*     work cell for exchange values, r24r25 
  
  *nds*     second cell of parameter stack, r22r23 
  
  *tos*     first cell of parameter stack, r20r21 
  
  SP        Not used, reserved for real cpu and interrupts :)
  
  PC        Not used, reserved for real cpu work :)
  
  (r4 to r19 are free and can direct accessed with @ and !, it works for Port IOs too. )
   
  (r0, r1, r2, r3, are used in internal mul* and fmul* instructions, and r1 must explicity set to 0)
  
  and obey avr-gcc header stats:
   
   __SREG__      Status register is at address 0x3F
   
   __SP_H__      Stack pointer high byte is at address 0x3E
   
   __SP_L__      Stack pointer low byte is at address 0x3D
   
   __tmp_reg__   Register r0, used for temporary storage
   
   __zero_reg__  Register r1, always zero
   
   __PC__        Program counter
   
   __SP__        Stack pointer

# Memory Model

@ need reviews:

SRAM:
  
  0x060 a 0x100, forth variables,
  
  584 bytes free
  
  0x346 top of PAD, reserved 80 bytes
  0x396 top of TIB, reserved 80 bytes
  0x3E6 end of TIB, end of DSP
  0x40E top of forth DSP stack, reserved 40 bytes.
  0x436 top of forth RSP stack, reserved 40 bytes.
  0x45E top of real SP stack, reserved 40 bytes.
  0x45F reserved for segment resister
   
reminder:
As the Atmega8 have eeprom, flash and sram. All eeprom goes from $000 to $200, and is read to and write from, sram. All flash goes from $000 to $FFF, with boot sector, NRWW sector and RWW sector. RWW could be rewrited on-the-fly. All sram goes from $000 to $45F, 1120 bytes, first 96 are cpu registers $00 to $20, and I/O registers $00 to $3F, then 1024 free ram. Data Adress Space was to $000 to $05F reserved and $060 to $45F free. The boot sector could be 512, 1024 and 2048 bytes. The Stack Pointer (SP) must be set to point above 0x60, pushs decrements, pops increments. The Program Counter (PC) is 12 bits wide, thus addressing the 4K words of program memory locations in flash.

#  why do not use SP as forth register ?
 
  In Forth there is no generic SP and all functionality and cycles of call, return, push and pop, could be made with Z, Y, X at same cpu cycles. Many extern standart library functions uses SP for internal pass-thru registers, then better leave SP for it.
  
##  DISCLAIMER:
/*
 *
 *  Copyright © 2020, Alvaro Barcellos,
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
*/

##  References

        https://code.google.com/archive/p/subtle-stack/downloads
        
        https://www.forth.com/starting-forth/
        
        http://thinking-forth.sourceforge.net/
        
        https://muforth.nimblemachines.com/threaded-code/

        All C. H. Ting books.
        
