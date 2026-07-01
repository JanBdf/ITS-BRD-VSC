# README for timer project in GT

> Jan Bittendorf | 10.06.2026
- - -

## Allgemeine Vorgaben

- Der Timer soll drei Zustände haben: INIT, RUNNING, HOLD.
- INIT: Es wird "00:00.00" angezeigt.
- RUNNING: Es wird die Zeit passend hochgezählt und angezeigt.
- HOLD: Es wird die Zeit nicht zurückgesetzt aber auch nicht mehr auf dem Bildschirm aktualisiert.
- Zeitformat: "mm:ss.cc"
- LED 8: Zeitmessung aktiv (RUNNING | HOLD)
- LED 9: Zeitanzeige angehalten (HOLD)

## Finite State Machine

```text
INIT <--+
 | ^    |
S7 |    |
 | S5   |
 v |    |
RUNNING |
 | ^    |
S6 |    S5
 | S7   |
 v |    |
HOLD ---+
```

## Ansteuerung der LEDs

```ARM
MOV R0,0x3             ; Bitmaske für LEDs D8 und D9
LDR R1,=GPIO_D_SET     ; Register zum LEDs anschalten laden
STR R0,[R1]            ; Entsprechende LED anschalten
EOR R0,R0,#0xFFFFFFFF  ; Bits umdrehen
LDR R1,=GPIO_D_CLR     ; Register zum LEDs ausschalten laden
STR R0,[R1]            ; Alle anderen LEDs ausschalten
```

## Auslesen der Tastatur

```ARM
MOV R1,=GPIO_F_PIN
LDR R0,[R1]
```
