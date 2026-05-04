;************************************************
;* Beginn der globalen Daten *
;************************************************
                   AREA MyData, DATA, align = 2
Base
VariableA          DCW 0x1234
VariableB          DCW 0x4711

VariableC          DCD  0

MeinHalbwortFeld   DCW 0x22 , 0x3e , -52, 78 , 0x27 , 0x45

MeinWortFeld       DCD 0x12345678 , 0x9dca5986
                   DCD -872415232 , 1308622848
                   DCD 0x27000000
                   DCD 0x45000000

MeinTextFeld       DCB "ABab0123",0

                   EXPORT VariableA
                   EXPORT VariableB
                   EXPORT VariableC
                   EXPORT MeinHalbwortFeld
                   EXPORT MeinWortFeld
                   EXPORT MeinTextFeld

;***********************************************
;* Beginn des Programms *
;************************************************
    AREA |.text|, CODE, READONLY, ALIGN = 3
; ----- S t a r t des Hauptprogramms -----
                EXPORT main
                EXTERN initITSboard
main            PROC
                bl    initITSboard                 ; HW Initialisieren

; Laden von Konstanten in Register
; r0 = 0x12
                mov   r0,#0x12                      ; Anw-01
; r1 = ~(128) + 1 = -128
                mov   r1,#-128                      ; Anw-02
; r2 = [pc + ?] = 0x12345678
                ldr   r2,=0x12345678                ; Anw-03

; Zugriff auf Variable
; r0 = &VariableA = 0x2000000c
                ldr   r0,=VariableA                 ; Anw-04
; r1 = VariableA = 0x1234
                ldrh  r1,[r0]                       ; Anw-05
; r2 = (VariableB << 8) & VariableA = 0x47111234
                ldr   r2,[r0]                       ; Anw-06
; VariableC = r2 = 0x47111234
                str   r2,[r0,#VariableC-VariableA]  ; Anw-07

; Zugriff auf Felder (Speicherzellen)
; r0 = &MeinHalbwortFeld = 0x20000014
                ldr   r0,=MeinHalbwortFeld          ; Anw-08
; r1 = MeinHalbwortFeld[0] = 0x22
                ldrh  r1,[r0]                       ; Anw-09
; r2 = MeinHalbwortFeld[1] = 0x3e
                ldrh  r2,[r0,#2]                    ; Anw-10
; r3 = 10
                mov   r3,#10                        ; Anw-11
; r4 = MeinHalbwortFeld[r3 / 2] = MeinHalbwortFeld[5] = 0x45
                ldrh  r4,[r0,r3]                    ; Anw-12

; r5 = MeinHalbwortFeld[1] = 0x3e; r0 += 2
                ldrh  r5,[r0,#2]!                   ; Anw-13
; r6 = MeinHalbwortFeld[2] = 0xffcc; r0 += 2
                ldrh  r6,[r0,#2]!                   ; Anw-14
; MeinHalbwortFeld[3] = r6; r0 += 2
                strh  r6,[r0,#2]!                   ; Anw-15

; Addition und Subtraktion von unsigned / signed Integer-Werten
; r0 = &MeinWortFeld = 0x20000020
				ldr  r0,=MeinWortFeld               ; Anw-16
; r1 = MeinWortFeld[0] = 0x12345678
                ldr  r1,[r0]                        ; Anw-17
; r2 = MeinWortFeld[1] = 0x9dca5986
                ldr  r2,[r0,#4]                     ; Anw-18
; r3 = r1 + r2 = 0xaffeaffe
                adds r3,r1,r2                       ; Anw-19

; r4 = MeinWortFeld[2] = -872415232
                ldr  r4,[r0,#8]                     ; Anw-20
; r5 = MeinWortFeld[3] = 1308622848
                ldr  r5,[r0,#12]                    ; Anw-21
; r6 = r4 - r5 = 2113929216
                subs r6,r4,r5                       ; Anw-22

; r7 = MeinWortFeld[4] = 0x27000000
                ldr  r7,[r0,#16]                    ; Anw-23
; r8 = MeinWortFeld[5] = 0x45000000
                ldr  r8,[r0,#20]                    ; Anw-24
; r9 = r7 - r8 = -503316480
                subs r9,r7,r8                       ; Anw-25

; while (true) { /* nop */ }
forever         b   forever                         ; Anw-26
                ENDP
                END