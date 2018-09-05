CPU "8085.TBL"
HOF "INT8"

ORG 9000H

; ----------------------------------------
; 8000H - digitally converted signal value
; 8001H - motor bit pattern
; ----------------------------------------


; 8255 addresses
; J1 connector for the motor
ADDR_MOTOR: EQU 00H
ADDR_MODE_MOTOR: EQU 03H

; J2 connector for the ADC
ADDR_A: EQU 40H
ADDR_DATA: EQU 41H
ADDR_READY: EQU 42H
ADDR_MODE_ADC: EQU 43H

; display routine
UPDDT: EQU 044CH

INIT:
    ; set modes of 8255
    MVI A, 80H
    OUT ADDR_MODE_MOTOR

    ; set mode for ADC
    ; port A - O/P
    ; port B, C - I/P

    MVI A, 8BH
    OUT ADDR_MODE_ADC

    ; set initial bit pattern of motor
    MVI A, 88H
    STA 8001H
    STA 8000H

; keep checking for data ready
POLL:
    ; shift motor by one
    CALL SHIFT_MOTOR

    ; select ADC channel
    MVI A, 00H
    OUT ADDR_A

    ; start!
    MVI A, 20H
    OUT ADDR_A

    ; start pulse over!
    MVI A, 00H
    OUT ADDR_A

    ; get D0 bit
    IN ADDR_READY
    ANI 01H
    JZ WAIT_EDGE

    ; check again
    JMP POLL

; rotate and update current bit pattern
SHIFT_MOTOR:
    LDA 8001H
    RRC
    STA 8001H
    OUT ADDR_MOTOR

    ; can't change patterns too fast!
    CALL MOTOR_DELAY

    RET

; wait for EOC signal
WAIT_EDGE:
    ; get D0 bit
    IN ADDR_READY
    ANI 01H
    JNZ DATA_READY
    JMP WAIT_EDGE

; come here after WAIT_EDGE when data's ready
DATA_READY:
    ; assert READ signal
    MVI A, 40H
    OUT ADDR_A

    ; get the digital value
    IN ADDR_DATA

    ; store the data and output
    STA 8000H
    CALL DISPLAY

    ; deassert READ signal
    MVI A, 00H
    OUT ADDR_A

    ; send value to motor

    JMP POLL

; display signal value in data field after a delay
DISPLAY:
    ;   CALL DELAY
    LDA 8000H
    STA 8FF1H
    CALL UPDDT

    RET

; delay for the display
DELAY:
    MVI C, 01H
    LOOP1:  MVI D, 08H
        LOOP2:  MVI E, 0AEH
                LOOP3:
                DCR E
                JNZ LOOP3
        DCR D
        JNZ LOOP2
    DCR C
    JNZ LOOP1

    RET

; delay for the motor
MOTOR_DELAY:
    ; load the digital value and subtract
    LDA 8000H
    MOV B, A
    MVI A, 0FFH
    SUB B
    MOV C, A
    LOOP4:  MVI D, 10H
        LOOP5:  MVI E, 03EH
                LOOP6:
                DCR E
                JNZ LOOP6
        DCR D
        JNZ LOOP5
    DCR C
    JNZ LOOP4

    RET
