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
        	BL initITSboard     ; needed by ITS-BRD for setup

			LDR  r2,=num_arr    ; Load address of num_arr
for_01		MOV  r0, #2         ; base = 2

until_01	CMP  r0,#31         ; Compare base to 31
			BGT  enddo_01       ; If its greater, jump to enddo_01


do_01		LDRB r3,[r2,r0]     ; Load num_arr[base]

if_01		CMP  r3,#0          ; Compare loaded value to 0
			BEQ  else_01        ; If 0, jump to else_01

then_01
for_02		MUL  r1,r0,r0       ; prod = base ** 2

until_02	CMP  r1,#1000       ; 
			BGT  enddo_02       ;

do_02		MOV  r3,#0          ;
			STRB r3,[r2,r1]     ;

step_02		ADD  r1,r1,r0       ;
			B    until_02       ;

enddo_02
else_01
step_01		ADD  r0,r0,#1       ;
			B    until_01       ;

endif_01
enddo_01	NOP

; --- Part 2 -----------------------------------------

            LDR  r3,=prime_arr  ; Load address of prime_arr
            MOV  r1,#0          ; prime_index = 0
for_03      MOV  r0,#2          ; num_index = 2

until_03    CMP  r0,#1000       ; Check num_index and 1000
            BGT  enddo_03       ; Jump to enddo_03 if greater

do_03
if_02       LDRB r4,[r2,r0]     ; Load num_arr[index]
			CMP  r4,#0
            BEQ  endif_02       ; Jump to endif_02 if zero

then_02     STRH r0,[r3,r1]     ; Store num_index into prime_arr[prime_index]
            ADD  r1,r1,#2       ; prime_index += 2

endif_02
step_03     ADD  r0,r0,#1       ; num_index++
            B    until_03       ; Jump to until_03

enddo_03    NOP                 ; Placeholder code

; --- Program end ------------------------------------

forever	b	forever	          	; loop indefinitely
		ENDP
	
		ALIGN
       
		END