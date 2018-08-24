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
LDA 8001H
CALL COPYIN

CPI 01H
CZ ADD
CALL COPYOUT

LDA 8001H
CPI 02H
CZ SUB
CALL COPYOUT

LDA 8001H
CPI 03H
CZ MUL
CALL COPYOUT

LDA 8001H
CPI 04H
CZ DIV

RST 5

; ---- If invalid operation, default to addition ----
JMP ADD


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

LDA 8013H
MOV B, A
MVI D, 00H
MVI A, 00H
LDA 8015H
MOV E, A
MVI A, 00H
CALL MUL8B

; -------- Store last 16 bits (Y1Y2) -------
LDA 800BH
STA 8019H
LDA 800AH
STA 8018H

; ------- 
LDA 8012H
MOV B, A
MVI D, 00H
MVI A, 00H
LDA 8014H
MOV E, A
MVI A, 00H
CALL MUL8B

; ----- Store first 16 bits (X1X2) -------
LDA 800BH
STA 8017H
LDA 800AH
STA 8016H

; ------
LDA 8012H
MOV B, A
MVI D, 00H
MVI A, 00H
LDA 8015H
MOV E, A
MVI A, 00H
CALL MUL8B


; ----- 8 bit multiplication subroutine ----
; ----- stores values in 800AH and 800BH ---
MUL8B:
LOOP: 
      ADD B
	    JNC NEXT
      INR D
NEXT: 
      DCR E
      JNZ LOOP
      STA 800BH
	    MOV A, D
	    STA 800AH

RET

; ------------- Div -----------------
DIV:

 LHLD 8002H                ; Get the dividend
       LDA 8005H                ; Get the divisor
       MOV C, A
       LXI D, 0000H                ; Quotient = 0
BACK: MOV A, H
       SUB C                        ; Subtract divisor
       MOV H, A                ; Save partial result
       JNC SKIP                ; if CY  1 jump
       DCR L                ; Subtract borrow of previous subtraction
SKIP: INX D                        ; Increment quotient
       MOV A, L
       CPI 00                ; Check if dividend < divisor
       JNZ BACK                ; if no repeat
AGAIN: MOV A, H
       SUB C
       JC END
       INX D
       MOV H, A
       JMP AGAIN
END:   SHLD 8008H                ; Store the remainder
       XCHG
       MOV A, H
      STA  8006H
      MOV A, L
      STA 8007H
                     ; Store the quotient
RET                        ; Terminate program execution

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