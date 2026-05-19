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

; initialize byte field for num_arr with length 999
; initialize word field for primes with length 500

; set r0 to 2 (base / loop var)
; create loop label
; 	compare r0 to 31
; 	jump if greater to exit 2
; 	check if num_arr[r0 - 2] == 0
; 	jump if equal to loop
; 	set r1 to r0 * r0 (inner loop var)
; 	create loop label 2
; 		compare r1 to 998 
;       if greater jump to exit
; 		set num_arr[prod - 2] to 0;
; 		add r1 to r1
; 		jump to loop 2
; 	create exit label
; 	add 1 to r0
; 	jump to loop
; create label exit 2
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