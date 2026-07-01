;******************** (C) COPYRIGHT HAW-Hamburg ********************************
;* File Name          : main.s
;* Author             : Franz Korf  
;* Version            : V1.0
;* Date               : 16.05.2022
;* Modified by        : Thomas Lehmann, 2024-07-12
;* Description        : This is the frame for the last assignment.
;                     : Einfaches Lauflicht.
;
;*******************************************************************************
    EXTERN initITSboard
    EXTERN lcdPrintS            ;Display ausgabe
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
; main subroutine
;--------------------------------------------

        
; Unterprogramm Lauftlicht
;
; Einfaches Lauflicht, das ein Bitmuster zyklisch ueber die 
; LEDs D23 bis D8 schiebt. Das LED Muster wird nach rechts 
; geschoben. Die Frequenz betraegt 2 Hz.
;
; IN R0  Die unteren 16 Bits von R0 speichern das Muster, mit
;        dem die LEDs initialisiert werden.
; IN R1  Anzahl Schritte, die das Lauflicht laufen soll.
;--------------------------------------------       
;

DelayTime   EQU     500

Lauflicht   PROC
            BX LR
            ENDP

;--------------------------------------------
; main subroutine
;--------------------------------------------
    EXPORT main [CODE]
        
InterTestDelay  EQU     4000
    
main    PROC
        BL initITSboard
        LDR     R7, =TestPattern
        MOV     R8, #0                  ; Laufindex Testpattern
forever 
        CMP     R8, #3
        MOVGE   R8, #0
        
        ; Test Lauflicht
        LDRH    R0, [R7,R8,LSL #1]
        MOV     R1, #20
        BL      Lauflicht
        
        LDR     R0, =InterTestDelay
        BL      delay

        ADD     R8, #1
        BAL     forever     ; nowhere to retun if main ends     
        ENDP
    
        ALIGN
        END
