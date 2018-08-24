CPU "8085.TBL"
HOF "INT8"

ORG 9000H

; ----------------------------------------------------
; 8001 - Operation
; 8002 - Input 1
; 8003 -  Input 2
; 8004-05  - Output 
; ----------------------------------------------------

; ------------- Initialize carry to zero -------------
INIT:
MVI C, 00H
MOV A, C
STA 8005H

; ------------- Load B from memory location -----------------
LDA 8002H
MOV B, A

; ------------- Check the operation -----------------
LDA 8001H

CPI 01H
JZ ADD

CPI 02H
JZ SUB

CPI 03H
JZ MUL

CPI 04H
JZ DIV

; -------- If invalid operation, default to addition ---------
JNZ ADD

; ------------- Add -----------------
ADD:

LDA 8003H
ADD B
JNC BRANCH_ADD
INR C

BRANCH_ADD:
	STA 8004H
	MOV A, C
	STA 8005H
RST 5

; ------------- Sub -----------------
SUB:

MVI C, 00H
MOV A, C
STA 8005H
LDA 8003H
SUB B
JNC BRANCH_SUB
CMA
INR A
INR C

BRANCH_SUB:
	STA 8004H
	MOV A, C
	STA 8005H
RST 5

; ------------- Mul -----------------
MUL:

MVI D,00
MVI A,00
LDA 8003H
MOV E, A
MVI A,00

LOOP: 
      ADD B
	  JNC NEXT
      INR D
NEXT: 
      DCR E
      JNZ LOOP
      STA 8005H
	  MOV A, D
	  STA 8004H
RST 5

; ------------- Div -----------------
DIV:

MVI  C,00
LDA 8003H
NEXT_2:   
        CMP B
        JC  LOOP_2
        SUB  B
        INR C
        JMP NEXT_2
LOOP_2: 
        STA 8004H
        MOV A,C
        STA 8005H
RST 5

; ------------- End -----------------
