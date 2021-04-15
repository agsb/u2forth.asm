;
; ATMEGA8 interrupt table
;
$000         rjmp   RESET          ; Reset Handler
$001         rjmp   EXT_INT0       ; IRQ0 Handler
$002         rjmp   EXT_INT1       ; IRQ1 Handler
$003         rjmp   TIM2_COMP      ; Timer2 Compare Handler
$004         rjmp   TIM2_OVF       ; Timer2 Overflow Handler
$005         rjmp   TIM1_CAPT      ; Timer1 Capture Handler
$006         rjmp   TIM1_COMPA     ; Timer1 CompareA Handler
$007         rjmp   TIM1_COMPB     ; Timer1 CompareB Handler
$008         rjmp   TIM1_OVF       ; Timer1 Overflow Handler
$009         rjmp   TIM0_OVF       ; Timer0 Overflow Handler
$00a         rjmp   SPI_STC        ; SPI Transfer Complete Handler
$00b         rjmp   USART_RXC      ; USART RX Complete Handler
$00c         rjmp   USART_UDRE     ; UDR Empty Handler
$00d         rjmp   USART_TXC      ; USART TX Complete Handler
$00e         rjmp   ADC            ; ADC Conversion Complete Handler
$00f         rjmp   EE_RDY         ; EEPROM Ready Handler
$010         rjmp   ANA_COMP       ; Analog Comparator Handler
$011         rjmp   TWSI           ; Two-wire Serial Interface Handler
$012         rjmp   SPM_RDY        ; Store Program Memory Ready Handler


RESET: 		; Main program start
            ; Set Stack Pointer to top of RAM
			; must reserve 1 byte for status register
$013         ldi    r16,high(RAMEND - 1) 
$014         out    SPH,r16        
$015         ldi    r16,low(RAMEND - 1)
$016         out    SPL,r16
$017         sei        
