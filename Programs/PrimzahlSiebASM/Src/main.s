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
	
main	PROC
        BL initITSboard ; needed by ITS-BRD for setup

		MOV r2,=num_arr
; set r0 to 2 (base / loop var)
		MOV r0, #2
; create loop label
; 	compare r0 to 31
loop_0	CMP r0,#31
; 	jump if greater to exit 2
		BGT exit_0
; 	check if num_arr[r0 - 2] == 0
		LDR r3,[r2,r0]
		CMP r3,#0
; 	jump if equal to loop
		ADDEQ r0,r0,#1
		BEQ loop
; 	set r1 to r0 * r0 (inner loop var)
		MUL r1,r0,r0
; 	create loop label 2
; 		compare r1 to 998 
loop_1	CMP r1,#1000
;       if greater jump to exit
		BGT exit_1
; 		set num_arr[prod - 2] to 0;
		STR #0,[r2,r1]
; 		add r1 to r1
		ADD r1,r1,r1
; 		jump to loop 2
		B loop_1
; 	create exit label
; 	add 1 to r0
exit_1	ADD r0,r0,#1
; 	jump to loop
		B loop
; create label exit 2
exit_0 	NOP
;
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