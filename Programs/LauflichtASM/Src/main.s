;******************** (C) COPYRIGHT HAW-Hamburg ********************************
;* File Name          : main.s
;* Author             : Franz Korf
;* Version            : V2.0
;* Date               : 16.05.2022
;* Modified by        : Thomas Lehmann, 2024-07-12 ; Jan Bittendorf 2026-07-01
;* Description        : This is the frame for the last assignment.
;                     : Einfaches Lauflicht.
;
;*******************************************************************************
    EXTERN initITSboard
    EXTERN lcdPrintS            ;Display output
    EXTERN GUI_init
    EXTERN TP_Init
    EXTERN delay
        
; Define address of selected GPIO and Timer registers
PERIPH_BASE         equ 0x40000000                 ;Peripheral base address
AHB1PERIPH_BASE     equ (PERIPH_BASE + 0x00020000)
APB1PERIPH_BASE     equ PERIPH_BASE

GPIOD_BASE          equ (AHB1PERIPH_BASE + 0x0C00)
GPIOE_BASE          equ (AHB1PERIPH_BASE + 0x1000)
GPIOF_BASE          equ (AHB1PERIPH_BASE + 0x1400)
TIM2_BASE           equ (APB1PERIPH_BASE + 0x0000)

GPIO_F_PIN          equ (GPIOF_BASE + 0x10)

GPIO_D_PIN          equ (GPIOD_BASE + 0x10)
GPIO_D_SET          equ (GPIOD_BASE + 0x18)
GPIO_D_CLR          equ (GPIOD_BASE + 0x1A) 
    
GPIO_E_PIN          equ (GPIOE_BASE + 0x10)
GPIO_E_SET          equ (GPIOE_BASE + 0x18)
GPIO_E_CLR          equ (GPIOE_BASE + 0x1A)     



;********************************************
; Data section, aligned on 4-byte boundery
;********************************************   
    AREA MyData, DATA, align = 2
TestPattern DCW     0x8000, 0x7000, 0x5000

;********************************************
; Code section, aligned on 8-byte boundery
;********************************************
    AREA |.text|, CODE, READONLY, ALIGN = 3

;--------------------------------------------
; ror_16b subroutine
;
; Performs a bitwise rotation to the right of the 16 least
; significant bits of register R0.
; The amount of bits to rotate by is specified in R1. (up to 15)
; The remaining bits of R0 are ignored and may be changed.
;--------------------------------------------

ror_16b		PROC
			MOV		R2, R0                  ; Create copy for shifted bits

			LSR		R0, R0,R1               ; Shift lower half of R0 to the right
			MOV 	R3, #0xFFFF             ; - Delete upper half of R0
			LSR		R3, R3,R1
			AND 	R0, R0,R3

			MOV 	R3, #16                 ; - Shift upper half of R2 left by (16 - R1)
			SUB		R1, R3,R1
			LSL		R2, R2,R1

			ORR		R0, R0,R2               ; Combine halfs into R0 as return value
			BX 		LR
			ENDP

;--------------------------------------------
; setLEDs subroutine
;
; Displays the pattern stored in R0 using the LEDs.
;--------------------------------------------

setLEDs		PROC
			MOV 	R2, R0                  ; - Put MSB of R0 into LSB of R2 for left GPIO block
			LSR		R2, R2,#8

			LDR		R1, =GPIO_D_SET         ; - Set active LEDs in both blocks
			LDR		R3, =GPIO_E_SET
			STR		R0, [R1]
			STR		R2, [R3]

			EOR		R0, R0,#0xFFFFFFFF      ; - Clear inactive LEDs in both blocks
			EOR		R2, R2,#0xFFFFFFFF
			LDR		R1, =GPIO_D_CLR
			LDR		R3, =GPIO_E_CLR
			STR 	R0, [R1]
			STR		R2, [R3]

			BX		LR
			ENDP  

;--------------------------------------------
; chaser subroutine
;
; Simple chaser moving a pattern of bits over LEDs D23 to D8.
; The pattern is moving the the right. Frequency is 2 Hz.
; 
; IN R0  Least significant 16 bits store the pattern to be displayed.
; IN R1  Amount of steps the chaser should go through.
;--------------------------------------------

DelayTime   EQU     500

chaser		PROC
			PUSH	{LR}                    ; Save LR to be able to run subroutines
			MOV 	R2,	#0                  ; Running index for while loop
while_01	
			CMP		R2, R1
			BGE		enddo_01                ; If first display is not counted as a step, change to BGT
do_01
			PUSH	{R0-R4}                 ; Save function parameters

			BL		setLEDs                 ; Display current pattern

			LDR		R0, =DelayTime          ; - Wait before displaying next step
			BL		delay

			POP		{R0}                    ; - Rotate pattern
			MOV		R1, #1                  ; Amount to rotate by
			BL		ror_16b

			POP		{R1-R4}                 ; Recover function parameters for next use
step_01		
			ADD 	R2, R2,#1
			BAL		while_01
enddo_01
			MOV		R0, #0                  ; Clear all LEDs after chaser ends
			BL		setLEDs

			POP		{LR}                    ; Could be `POP {PC}` to save an instruction
            BX 		LR
            ENDP

;--------------------------------------------
; main subroutine
; 
; Cycles through a range of test patterns and displays them
; using the lauflicht subroutine.
;--------------------------------------------
    EXPORT main [CODE]
        
InterTestDelay  EQU     4000
    
main    PROC
        BL initITSboard
        LDR     R7, =TestPattern
        MOV     R8, #0                      ; Running index of test pattern array
forever 
        CMP     R8, #3                      ; - R8 MOD 3
        MOVGE   R8, #0
        
        ; Test chaser
        LDRH    R0, [R7,R8,LSL #1]          ; Load current pattern
        MOV     R1, #20                     ; Amount of chaser steps
        BL      chaser
        
        LDR     R0, =InterTestDelay         ; - Wait in between patterns
        BL      delay

        ADD     R8, #1
        BAL     forever  
        ENDP
    
        ALIGN
        END
