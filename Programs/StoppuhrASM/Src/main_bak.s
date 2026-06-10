;******************** (C) COPYRIGHT HAW-Hamburg ********************************
;* File Name          : main.s
;* Author             : Jan Bittendorf
;* Version            : V1.0
;* Date               : 03.06.2026
;* Description        : Timer display program using lcd and buttons
;
;*******************************************************************************

; Define address of selected GPIO and Timer registers
PERIPH_BASE     	equ	0x40000000                 ;Peripheral base address
AHB1PERIPH_BASE 	equ	(PERIPH_BASE + 0x00020000)
APB1PERIPH_BASE     equ PERIPH_BASE

GPIOD_BASE			equ	(AHB1PERIPH_BASE + 0x0C00)
GPIOF_BASE			equ	(AHB1PERIPH_BASE + 0x1400)
TIM2_BASE           equ (APB1PERIPH_BASE + 0x0000)
	
GPIO_F_PIN        	equ	(GPIOF_BASE + 0x10)

GPIO_D_PIN			equ	(GPIOD_BASE + 0x10)
GPIO_D_SET			equ (GPIOD_BASE + 0x18)
GPIO_D_CLR			equ	(GPIOD_BASE + 0x1A)
	
TIMER				equ (TIM2_BASE + 0x24)   ; CNT : current time stamp (32 bit),  resolution
TIM2_PSC			equ (TIM2_BASE + 0x28)   ; Prescaler  resolution
TIM2_ERG			equ (TIM2_BASE + 0x14)   ; 16 Bit register, Bit 0 : 1 Restart Timer


    EXTERN initITSboard
    EXTERN GUI_init
	EXTERN TP_Init
	EXTERN initTimer
	EXTERN lcdSetFont
	EXTERN lcdGotoXY      		; TFT goto x y function
	EXTERN lcdPrintS			; TFT output function	
    EXTERN lcdPrintC            ; TFT output one character		
	EXTERN Delay				; Delay (ms) function

CONST_10_MS  EQU     1000
CONST_100_MS EQU    10000
CONST_1_S    EQU   100000
CONST_10_S   EQU  1000000
CONST_1_M    EQU  6000000
CONST_10_M   EQU 60000000


;********************************************
; Data section, aligned on 4-byte boundery
;********************************************
	AREA MyData, DATA, align = 2

DEFAULT_BRIGHTNESS	DCW     800

TEXT_INIT			DCB		"INIT", 0
TEXT_RUNNING		DCB		"RUNNING", 0
TEXT_HOLD			DCB 	"HOLD", 0

TEXT_TIME			DCB 	"00:00.00", 0
TEXT_TIME_OLD		DCB 	"--------", 0

STATE_FUNCS         DCD     init_entry, init, running_entry, running, hold_entry, hold
STATE_IDX			DCB		0

TIME_FACTORS		DCD		CONST_10_M, CONST_1_M, CONST_10_S, CONST_1_S, CONST_100_MS, CONST_10_MS

;********************************************
; Code section, aligned on 8-byte boundery
;********************************************
	AREA |.text|, CODE, READONLY, ALIGN = 3


;--------------------------------------------
; main subroutine
;--------------------------------------------
	EXPORT main [CODE]
	
main			PROC

			; Initialisierung der HW
			BL		initITSboard
			LDR   	r1, =DEFAULT_BRIGHTNESS
			LDRH 	r0, [r1]
			BL   	GUI_init
			BL  	initTimer
			LDR 	R1,=TIM2_PSC   			; Set pre scaler such that 1 timer tick represents 10 us
			MOV 	R0,#(90*10-1) 
			STRH	R0,[R1]
			BL      reset_timer
			MOV 	R0, #24
			BL  	lcdSetFont

superloop
			LDR		r0,=STATE_IDX
			LDR 	r1,=STATE_FUNCS
			LDRB 	r2,[r0]
			LDR		r3,[r1,r2, LSL #2]
			BLX     r3
			BAL		superloop				; End of superloop

		 		ENDP

; ---------------------------------------------------

reset_timer 	PROC

			LDR 	R1,=TIM2_ERG
			MOV		R0,#0x01
			STRH	R0,[R1]					; Set UG Bit -> Restart timer
			BX		LR

				ENDP

; ---------------------------------------------------

read_btn    	PROC

			LDR 	r1,=GPIO_F_PIN
			LDRH	r2,[r1]
			LSR		r2,r2,r0
			ANDS    r0,r2,#1
			BX 		LR

				ENDP

; ---------------------------------------------------

set_leds    	PROC

		 	LDR		R1,=GPIO_D_SET
		 	STR		R0,[R1]
			EOR		R0,R0,#0xFFFFFFFF
			LDR		R1,=GPIO_D_CLR
			STR		R0,[R1]
			BX 		LR

				ENDP

; ---------------------------------------------------

print_time		PROC

			PUSH	{R4-R8,LR}
			MOV		r4,#0
			LDR		r5,=TEXT_TIME
			LDR		r6,=TEXT_TIME_OLD
while_01	CMP		r4,#7
			BGT		enddo_01
do_01		LDRB	r7,[r5,r4]
			LDRB	r8,[r6,r4]
if_09		CMP		r7,r8
			BEQ		endif_09
then_09		ADD		r0,r4,#10
			MOV		r1,#6
			BL		lcdGotoXY
			LDRB	r0,[r5,r4]
			STRB	r0,[r6,r4]
			BL		lcdPrintC
endif_09
step_01		ADD		r4,r4,#1
			BAL		while_01
enddo_01	POP		{R4-R8,LR}
			BX		LR

				ENDP

; ---------------------------------------------------

set_time  		PROC

			PUSH	{R4,LR}
			LDR		R1,=TIME_FACTORS
			LDR		R2,[R1,#8]
			LDR		R1,=TEXT_TIME
			LDR		R2,=CONST_10_M
			UDIV	R3,R0,R2

if_10		CMP		R3,#10                   ; Reset timer if greater than 99:59.99
			BLT		endif_10
then_10		PUSH	{r1}
			BL 		reset_timer
			POP		{r1}
			MOV		R0,#0
			MOV		R3,#0

endif_10	MUL		R4,R3,R2
			SUB		R0,R0,R4
			ADD		R3,R3,#0x30
			STRB	R3,[R1,#0]

			LDR		R2,=CONST_1_M
			UDIV	R3,R0,R2
			MUL		R4,R3,R2
			SUB		R0,R0,R4
			ADD		R3,R3,#0x30
			STRB	R3,[R1,#1]

			LDR		R2,=CONST_10_S
			UDIV	R3,R0,R2
			MUL		R4,R3,R2
			SUB		R0,R0,R4
			ADD		R3,R3,#0x30
			STRB	R3,[R1,#3]

			LDR		R2,=CONST_1_S
			UDIV	R3,R0,R2
			MUL		R4,R3,R2
			SUB		R0,R0,R4
			ADD		R3,R3,#0x30
			STRB	R3,[R1,#4]

			LDR		R2,=CONST_100_MS
			UDIV	R3,R0,R2
			MUL		R4,R3,R2
			SUB		R0,R0,R4
			ADD		R3,R3,#0x30
			STRB	R3,[R1,#6]

			LDR		R2,=CONST_10_MS
			UDIV	R3,R0,R2
			MUL		R4,R3,R2
			SUB		R0,R0,R4
			ADD		R3,R3,#0x30
			STRB	R3,[R1,#7]
			POP		{R4,LR}
			BX		LR

				ENDP

; ---------------------------------------------------

init_entry 		PROC

			PUSH	{LR}
			MOV		r0,#0
			BL		set_time
			BL		print_time
			MOV		r0,#0
			;MOV		r1,#0
			;BL		lcdGotoXY
			BL		set_leds
			;LDR 	R0,=TEXT_INIT
			;BL  	lcdPrintS

			LDR 	r0,=STATE_IDX
			MOV		r1,#1                  ; INIT
			STRB	r1,[r0]
			POP		{LR}
			BX 		LR

				ENDP

; ---------------------------------------------------

init        	PROC

			PUSH 	{R6,LR}
if_04		MOV		r0,#7
			BL		read_btn
			BNE		endif_04
then_04		MOV   	r6,#2                   ; RUNNING_ENTRY
			LDR		r1,=STATE_IDX
			STRB  	r6,[r1]
			BL		reset_timer
endif_04	POP		{R6,LR}
			BX 		LR

				ENDP

; ---------------------------------------------------

running_entry	PROC
			
			PUSH	{LR}
			;MOV		r0,#0
			;MOV		r1,#0
			;BL		lcdGotoXY
			MOV		r0,#0x1
			BL		set_leds
			;LDR		r0,=TEXT_RUNNING
			;BL		lcdPrintS

			LDR		r0,=STATE_IDX
			MOV		r1,#3					; RUNNING
			STRB	r1,[r0]
			POP		{LR}
			BX 		LR

				ENDP

; ---------------------------------------------------

running			PROC

			PUSH	{LR}
			LDR 	r0,=TIMER
			LDR		r0,[r0]
			BL		set_time
			BL		print_time

if_05		MOV		r0,#6
			BL		read_btn
			BNE		else_05
then_05		LDR		r0,=STATE_IDX
			MOV 	r1,#4                   ; HOLD_ENTRY
			STRB	r1,[r0]
			BAL		endif_05
else_05		
if_06		MOV		r0,#5
			BL		read_btn
			BNE		endif_06
then_06		LDR		r0,=STATE_IDX
			MOV		r1,#0                   ; INIT_ENTRY
			STRB	r1,[r0]
endif_06	
endif_05	POP		{LR}
			BX 		LR

				ENDP

; ---------------------------------------------------

hold_entry		PROC

			PUSH	{LR}
			;MOV		r0,#0
			;MOV		r1,#0
			;BL		lcdGotoXY
			MOV		r0,#3
			BL		set_leds
			;LDR		r0,=TEXT_HOLD
			;BL		lcdPrintS

			LDR		r0,=STATE_IDX
			MOV 	r1,#5                   ; HOLD
			STRB	r1,[r0]

			POP		{LR}
			BX		LR

				ENDP

; ---------------------------------------------------

hold			PROC

			PUSH	{LR}
if_07		MOV		r0,#7
			BL		read_btn
			BNE		else_05
then_07		LDR		r0,=STATE_IDX
			MOV 	r1,#2                   ; RUNNING_ENTRY
			STRB	r1,[r0]
			BAL		endif_05
else_07		
if_08		MOV		r0,#5
			BL		read_btn
			BNE		endif_06
then_08		LDR		r0,=STATE_IDX
			MOV		r1,#0                   ; INIT_ENTRY
			STRB	r1,[r0]
endif_08	
endif_07	POP		{LR}
			BX 		LR

				ENDP

; ---------------------------------------------------

		 	ALIGN
		 	END
