CPU "8085.TBL"
HOF "INT8"

ORG 9000H

; --------------------------------------------------------------
; 8000H - digitally converted signal value
; 8001H - motor bit pattern
; 8002H - current step count
; 8003H - step value to be reached based on current signal value
; --------------------------------------------------------------

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

; initialize configurations
INIT:
    ; set modes of 8255
    ; mode for motor
    ; all ports - O/P

    MVI A, 80H
    OUT ADDR_MODE_MOTOR

    ; mode for ADC
    ; port A - O/P
    ; port B, C - I/P

    MVI A, 8BH
    OUT ADDR_MODE_ADC

    ; set initial bit pattern of motor
    MVI A, 88H
    STA 8001H
    STA 8000H

    ; initialize step count
    MVI A, 00H
    STA 8002H

; keep checking for data ready
POLL:
    ; shift motor by one
    CALL SHIFT_MOTOR

    ; can't change patterns too fast!
    CALL MOTOR_DELAY

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

; rotate and update current bit pattern, based on direction we have to go to
SHIFT_MOTOR:
    ; reset carry flag
    STC
    CMC

    ; get direction of rotation
    LDA 8003H
    MOV B, A
    LDA 8002H
    CMP B
    JZ RETURN   ; if equal, don't rotate
    JC RIGHT
    JNC LEFT

    LEFT:
        DCR A
        STA 8002H
        LDA 8001H
        RLC
        STA 8001H
        JMP RETURN

    RIGHT:
        INR A
        STA 8002H
        LDA 8001H
        RRC
        STA 8001H
        JMP RETURN

    RETURN:
    LDA 8001H
    OUT ADDR_MOTOR

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

    ; compute the steps to get to the next position
    CALL COMPUTE_STEPS

    ; deassert READ signal
    MVI A, 00H
    OUT ADDR_A

    ; go back to polling
    JMP POLL

; display signal value in data field after a delay
DISPLAY:
    ;   CALL DELAY
    LDA 8000H
    STA 8FF1H
    CALL UPDDT

    RET

; compute the steps required from the initial position
; note, # of steps = (digital value of signal)/2
COMPUTE_STEPS:
    LDA 8000H
    STC
    CMC
    RAR     ; right shift to divide
    STA 8003H
    RET

; delay for the motor
MOTOR_DELAY:
    MVI C, 05H
    LOOP4:  MVI D, 02H
        LOOP5:  MVI E, 01EH
                LOOP6:
                DCR E
                JNZ LOOP6
        DCR D
        JNZ LOOP5
    DCR C
    JNZ LOOP4

    RET
