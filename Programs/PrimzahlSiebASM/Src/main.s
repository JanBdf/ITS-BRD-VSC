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
			B   do_02        ;

enddo_02
else_01
step_01		ADD r0,r0,#1     ;
			B   until_01     ;

endif_01
enddo_01	NOP

; --- Part 2 -----------------------------------------

            MOV r3,=prime_arr  ; Load Address of prime_arr
            MOV r1,#0          ; num_primes = 0
for_03      MOV r0,#0          ; index = 0

until_03    CMP r0,#998        ; Check index and 998
            BGT enddo_03       ; Jump to enddo_03 if greater

do_03       LRD r4,[r2,r0]     ; Load num_arr[index]
            BEQ step_03        ; Jump to step_03 if zero
            STR r0,[r3,r1]     ; Store index into prime_arr[num_primes]
            ADD r1,r1,#1       ; num_primes++

step_03     ADD r0,r0,#1       ; index++
            B   until_03       ; Jump to until_03

enddo_03    NOP                ; Placeholder code



forever	b	forever		; loop indefinitely
		ENDP
	
		ALIGN
       
		END