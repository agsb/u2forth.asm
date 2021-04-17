;----------------------------------------------------------------------
;
;    MUST use gnu cpp 
;    use with .S (upper S) or
;    use with -x assembler-with-cpp
;
;----------------------------------------------------------------------
; generic use
#define    _work_    r0    
; .set _work_ , r0    

; always zero
#define    _zero_    r1    
; .set _zero_ , r1

; Z instruction pointer for access sram and flash high memory
#define    ip_low    r30    
;.set    ipl,    r30

; Z instruction pointer for access sram and flash high memory
#define    ip_high    r31    
;.set    iph,    r31

; Y return stack pointer for access sram
#define    rsp_low    r28    
;.set    rspl,    r28

; Y return stack pointer for access sram
#define    rsp_high    r29    
;.set    rsph,    r29

; X parameter stack pointer for access sram
#define    psp_low    r26    
;.set    pspl,    r26

; X parameter stack pointer for access sram
#define    psp_high    r27    
;.set    psph,    r27

; work registers
#define    wrk_low    r24    
;.set    wl,    r24

; work registers
#define    wrk_high    r25    
;.set    wh,    r25

; second value parameter stack
#define    nds_low    r22    
;.set    nl,    r22

; second value parameter stack
#define    nds_high    r23    
;.set    nh, r23

; fist value parameter stack
#define    tos_low    r20    
;.set    tl,    r20

; fist value parameter stack
#define    tos_high    r21    
;.set    th,    r21

;----------------------------------------------------------------------
;
; version information
;----------------------------------------------------------------------
;
; version information
;
.equ version, 0x0100

;----------------------------------------------------------------------
;
; start reverse linked list
;
.set _LINK , 0x0
.set _THIS , 0x0

;----------------------------------------------------------------------
;
; idents for first opcode in macros
;
.equ isNOOP, 0
.equ isLEAF, 1
.equ isTWIG, 2

;----------------------------------------------------------------------
;
; header of word in dictionary
; opcode for CORE ( execute a opcode )
; must end with CORE :)
;
.macro LEAF size , name
.set _THIS, .
.word _LINK
.set _LINK, _THIS 
.byte \size
.ascii "\name"
;was odd, so we need to pad it
.if ! ( \size & 1 ) 
.byte 0
.endif
.byte isLEAF
.balign 2
.endm

;----------------------------------------------------------------------
;
; header of word in dictionary
; opcode for TWIG ( as colon )
; must end with EXIT :)
;
.macro TWIG size , name
.set _THIS, .
.word _LINK
.set _LINK, _THIS 
.byte \size
.ascii "\name"
;was odd, so we need to pad it
.if ! ( \size & 1 ) 
.byte 0
.endif
.byte isTWIG
.balign 2
.endm

;----------------------------------------------------------------------
.macro rspush low , high
    st -Y , \low
    st -Y , \high
.endm

;----------------------------------------------------------------------
.macro rspull low , high
    ld \high , Y+
    ld \low , Y+
.endm

;----------------------------------------------------------------------
.macro pspush low , high
    st -X , \low
    st -X , \high
.endm

;----------------------------------------------------------------------
.macro pspull low , high
    ld \high , X+
    ld \low , X+
.endm

;----------------------------------------------------------------------
; also io@
.macro peek byte , address
    .if (\address < $40)
    in \byte , \address
    .else
    lds \byte , \address
    .endif
.endm

;----------------------------------------------------------------------
; also io!
.macro poke byte , address
    .if (\address < $40)
    out \byte , \address
    .else
    sts \byte , \address
    .endif
.endm

;----------------------------------------------------------------------
.macro next_wrk
	lpm wrk_low, Z+
	lpm wrk_high, Z+
.endm

;----------------------------------------------------------------------
.macro pull_wrk
 	movw wrk_low, tos_low
 	movw tos_low, nds_low
 	pspull nds_low, nds_high
.endm

;----------------------------------------------------------------------
.macro push_wrk
	pspush nds_low, nds_high
	movw nds_low, tos_low
 	movw tos_low, wrk_low
.endm

