CPU "8085.TBL"
HOF "INT8"

ORG 9000H

; ----------------------------------------------------
; 8001 - Operation
; 8002-03  -  Input 1
; 8004-05  -  Input 2
; 8006-09  -  Output 
; 800A-0B  -  Temporary Storage for MUL
; 8012-13  -  Temporary Input 1
; 8014-15  -  Temporary Input 2
; 8016-19  -  Temporary Output
; ----------------------------------------------------

; ------------- Initialize carry to zero -------------
INIT:
MVI C, 00H
MOV A, C
STA 8006H
STA 8007H
STA 8008H
STA 8009H

; ------------- Check the operation -----------------
CALL COPYIN

LDA 8001H
CPI 01H
CZ ADD
LDA 8001H
CPI 01H
CZ COPYOUT

LDA 8001H
CPI 02H
CZ SUB
LDA 8001H
CPI 02H
CZ COPYOUT

LDA 8001H
CPI 03H
CZ MUL

LDA 8001H
CPI 04H
CZ DIV

RST 5

; ------------- Add -----------------
ADD:

MVI A, 00H
STA 8016H
LDA 8013H
MOV B, A
LDA 8015H
ADD B
STA 8019H
LDA 8012H
MOV B, A
LDA 8014H
ADC B
STA 8018H
MVI A, 00H
ACI 00H
STA 8017H

RET

; ------------- Sub -----------------
SUB:

LDA 8015H
CMA 
ADI 01H
MOV B, A
LDA 8014H
CMA
ACI 00H
STA 8014H
MOV A, B
STA 8015H
CALL ADD
MVI A, 00H
STA 8017H

RET

; ------------- Mul -----------------
MUL:

; --- STORE E ---
LDA 8002H
STA 800AH
LDA 8003H
STA 800BH

;--------- Start ------
MVI A, 00H
STA 8006H
STA 8007H
STA 8008H
STA 8009H
STA 8016H
STA 8017H
STA 8018H
STA 8019H

LOOP:
    ;---- COPY B ---  
    LDA 8004H
    STA 8014H
    LDA 8005H
    STA 8015H

    ;---- COPY TEMP SUM ---
    LDA 8008H
    STA 8012H
    LDA 8009H
    STA 8013H

    ; ---- ADD B + TEMP SUM ---
    CALL ADD

    ; --- WRITE BACK TEMP SUM ---
    LDA 8018H
    STA 8008H
    LDA 8019H
    STA 8009H
    
    ; ---- CHECK CARRY ----   
    LDA 8017H
    CPI 00H

    ; ---- JUMP ON NO CARRY ---
    JZ NEXT

    ; ---- IF CARRY, INCREMENT 8006-07 ---
    LDA 8006H
    STA 8012H
    LDA 8007H
    STA 8013H
    MVI A, 00H
    STA 8014H
    MVI A, 01H
    STA 8015H
    CALL ADD 

    ; --- WRITE BACK ----
    LDA 8018H
    STA 8006H
    LDA 8019H
    STA 8007H   

    MVI C, 11H
    
NEXT:
    ; --- DECREMENT E ----
    LDA 800AH
    STA 8012H
    LDA 800BH
    STA 8013H
    MVI A, 00H
    STA 8014H
    MVI A, 01H
    STA 8015H

    CALL SUB

    ; ---- STORE DECREMENTED RESULT ---
    LDA 8018H
    STA 800AH
    LDA 8019H
    STA 800BH

    ; ---- CHECK IF ZERO ---
    LDA 800AH
    MOV B, A
    LDA 800BH
    ORA B
    CPI 00H
    JNZ LOOP

RET

; ------------- Div -----------------
DIV:

LHLD 8002H                      ; Get the dividend
LDA 8005H                       ; Get the divisor
MOV C, A
LXI D, 0000H                    ; Quotient = 0

BACK:   MOV A, H
        SUB C                   ; Subtract divisor
        MOV H, A                ; Save partial result
        JNC SKIP                ; if CY  1 jump
        DCR L                   ; Subtract borrow of previous subtraction

SKIP:   INX D                   ; Increment quotient
        MOV A, L
        CPI 00                  ; Check if dividend < divisor
        JNZ BACK                ; if no repeat

AGAIN:  MOV A, H
        SUB C
        JC END
        INX D
        MOV H, A
        JMP AGAIN

END:    SHLD 8008H              ; Store the remainder
        XCHG
        MOV A, H
        STA  8006H
        MOV A, L
        STA 8007H               ; Store the quotient
                                
RET                             ; Return back

; ------------- Copy Input ------------
COPYIN:

LDA 8002H
STA 8012H
LDA 8003H
STA 8013H
LDA 8004H
STA 8014H
LDA 8005H
STA 8015H
RET

; ------------- Copy Output -----------
COPYOUT:

LDA 8016H
STA 8006H
LDA 8017H
STA 8007H
LDA 8018H
STA 8008H
LDA 8019H
STA 8009H
RET

; ------------- End -----------------
