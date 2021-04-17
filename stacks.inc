;----------------------------------------------------------------------
; ok R@
LEAF 2, "R@"
RAT:
    pspush nds_low, nds_high
    movw nds_low, tos_low
    rspull tos_low, tos_high
    ; sbiw rsp_low, 1    zzz
    rjmp CORE

;----------------------------------------------------------------------
; ok R>
LEAF 2, "R>"
RGT:
    pspush nds_low, nds_high
    movw nds_low, tos_low
    rspull tos_low, tos_high
    rjmp CORE

;----------------------------------------------------------------------
; ok >R
LEAF 2, ">R"
GTR:
    rspush tos_low, tos_high
    rjmp DROP

;----------------------------------------------------------------------
; ok RS@
LEAF 3, "RS@"
RSAT:
    movw wrk_low, rsp_low
    rjmp PUSHS

;----------------------------------------------------------------------
; ok RS!
LEAF 3, "RS!"
RSTO:
    movw rsp_low, tos_low
    rjmp DROP

;----------------------------------------------------------------------
; PS@
LEAF 4, "PS@"
PSAT:
    movw wrk_low, psp_low
    rjmp PUSHS

;----------------------------------------------------------------------
; PS!
LEAF 4, "PS!"
PSTO:
    movw psp_low, tos_low
    rjmp CORE

;----------------------------------------------------------------------
; ok ?DUP (a -- 0 | a a)
LEAF 3, "?DUP"
QMDUP:
    mov _work_, tos_low
    or _work_, tos_high
    breq _QMDUP1
    rjmp DUP
_QMDUP1:
    rjmp CORE

;----------------------------------------------------------------------
; ok DUP (a -- a a)
LEAF 3, "DUP"
DUP:
    pspush nds_low, nds_high
    movw nds_low, tos_low
    rjmp CORE

;----------------------------------------------------------------------
; ok SWAP (a b -- b a)
LEAF 4, "SWAP"
SWAP:
    movw wrk_low, nds_low
    movw nds_low, tos_low
    movw tos_low, wrk_low
    rjmp CORE
 
;----------------------------------------------------------------------
;
; ok DROP (a b -- a)
;
LEAF 4, "DROP"
DROP:
    movw tos_low, nds_low
    pspull nds_low, nds_high
    rjmp CORE

;----------------------------------------------------------------------
;
; ok (a b c -- a b c w)
; push a value or a flag, to tos
;
LEAF 5, "PUSHS"
PUSHS:
    pspush nds_low, nds_high
    movw nds_low, tos_low
    movw tos_low, wrk_low
    rjmp CORE

;----------------------------------------------------------------------
; ok ROT (a b c -- b c a)
;
LEAF 3, "ROT"
ROT:
    pspull wrk_low, wrk_high
    rjmp PUSHS

;----------------------------------------------------------------------
; ok OVER (a b -- a b a)
;
LEAF 4, "OVER"
OVER:
    movw wrk_low, nds_low
    rjmp PUSHS

;----------------------------------------------------------------------
;
; ok push a false 0x0000
;
LEAF 5, "FALSE"
ZERO:
FFALSE:
    clr wrk_low
    clr wrk_high
    rjmp PUSHS

;----------------------------------------------------------------------
;
; ok push a true 0xFFFF
;
LEAF 4, "TRUE"
ONENEG:
FTRUE:
    ser wrk_low
    ser wrk_high
    rjmp PUSHS

;----------------------------------------------------------------------
; ok 2drop
LEAF 5, "2DROP"
TWODROP:
    pspull tos_low, tos_high
    pspull nds_low, nds_high
    rjmp CORE

;----------------------------------------------------------------------
; ok 2dup
LEAF 4, "2DUP"
TWODUP:
    pspush nds_low, nds_high
    pspush tos_low, tos_high
    rjmp CORE

;----------------------------------------------------------------------
; ok 2swap ( a b c d -- c d a b )
LEAF 4, "2SWAP"
TWOSWAP:
; get b
    pspull wrk_low, wrk_high
    rspush wrk_low, wrk_high
; get a
    pspull wrk_low, wrk_high
    rspush wrk_low, wrk_high
; put c
    pspush nds_low, nds_high
; put d 
    pspush tos_low, tos_high
; get a 
    rspull nds_low, nds_high
; get b
    rspull tos_low, tos_high

    rjmp CORE





