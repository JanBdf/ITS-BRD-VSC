;******************** (C) COPYRIGHT HAW-Hamburg ********************************
;* File Name          : main.s
;* Author             : Jan Bittendorf
;* Version            : V1.0
;* Date               : 01.06.2021
;* Description        : Prime number algorithm by Erathosthenes in ASM
;*******************************************************************************
    EXTERN initITSboard
    EXTERN lcdPrintS            ; display output
    EXTERN GUI_init
;	EXTERN TP_Init

;********************************************
; Data section, aligned on 4-byte boundery
;********************************************
	
	AREA MyData, DATA, align = 2

; initialize byte field for num_arr with length 999
num_arr FILL 1001, 0x1, 1
; initialize half word field for primes with length 500
prime_arr SPACE 1000

;********************************************
; Code section, aligned on 8-byte boundery
;********************************************

	AREA |.text|, CODE, READONLY, ALIGN = 3

;--------------------------------------------
; main subroutine
;--------------------------------------------
	EXPORT main [CODE]
	
main		PROC
        	BL initITSboard ; needed by ITS-BRD for setup

			LDR r2,=num_arr  ; 
for_01		MOV r0, #2       ; var base 

until_01	CMP r0,#31       ; 
			BGT enddo_01     ;


do_01		LDRB r3,[r2,r0]  ; 

if_01		CMP r3,#0        ; 
			BEQ else_01      ;

then_01
for_02		MUL r1,r0,r0     ; var prod

until_02	CMP r1,#1000     ;
			BGT enddo_02     ;

do_02		MOV r3,#0        ;
			STRB r3,[r2,r1]  ;

step_02		ADD r1,r1,r0     ;
			B do_02          ;

enddo_02
else_01
step_01		ADD r0,r0,#1     ;
			B until_01       ;

endif_01
enddo_01	NOP

; set r1 to 0 (num_primes)
; set r0 to 0 (loop var)
; create loop label 3
; 	compare r0 to 998
; 	if greater jump to exit 3
; 	test num_arr[r0]
; 	if not zero set primes[r1] to r0 + 2
; 	add 1 to r0
; 	jump to loop 3
; create label exit 3

forever	b	forever		; loop indefinitely
		ENDP
	
		ALIGN
       
		END