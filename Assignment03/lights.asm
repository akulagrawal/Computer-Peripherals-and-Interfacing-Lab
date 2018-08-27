CPU "8085.TBL"
HOF "INT8"

ORG 9000H

; --------------------------------
; 8000H - current configuration
; 8001-8003H - counter
; 8004H - flag
; --------------------------------

GTHEX: EQU 030EH
HXDSP: EQU 034FH
OUTPUT: EQU 0389H
CLEAR: EQU 02BEH
RDKBD: EQU 03BAH
UPDDT: EQU 044CH
UPDAD: EQU 0440H
ADDR_A: EQU 40H
ADDR_B: EQU 41H
ADDR_C: EQU 42H
ADDR_MODE: EQU 43H

INIT:
    ; set mode of 8255
    MVI A, 8BH
    OUT ADDR_MODE

    ; initialize config
    MVI A, 01H
    STA 8000H

    ; initialize flag
    MVI A, 00H
    STA 8004H

CHECK:
    ; output configuration
    LDA 8000H
    OUT ADDR_A

    ; get inputs
    IN ADDR_B
    ANI 04H     ; for D2
    CPI 00H     ; for D2
    JZ HALT

    IN ADDR_B
    ANI 20H     ; for D5
    CPI 00H     ; for D5
    JZ PAUSE

    ; check flag
    LDA 8004H
    CPI 11H
    CZ DECREMENT_COUNTER
    
    IN ADDR_B
    ANI 40H     ; for D6
    CPI 00H     ; for D6

    JZ CYCLE
    JMP CHECK

DECREMENT_COUNTER:
    
    CALL DELAY
    CALL INCREMENT_CONFIG
    RET


DELAY:
LOOP4:  MVI D, 04H
LOOP1:  MVI E, 0DEH
LOOP2:  DCR E
        JNZ LOOP2
        DCR D
        JNZ LOOP1
        DCR C
        JNZ LOOP4
        RET

INCREMENT_CONFIG:
    LDA 8000H
    RLC
    STA 8000H
    RET

HALT:
    RST 5

PAUSE:
    MVI A, 00H
    STA 8004H
    JMP CHECK

CYCLE:
    MVI A, 11H
    STA 8004H
    JMP CHECK