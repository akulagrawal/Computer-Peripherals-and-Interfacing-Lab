CPU "8085.TBL"
HOF "INT8"

ORG 9000H

; --------------------------------
; 8000H - hours
; 8001H - minute
; 8002H - seconds
; 8010-8012H - temporary locations
; --------------------------------

GTHEX: EQU 030EH
HXDSP: EQU 034FH
OUTPUT: EQU 0389H
CLEAR: EQU 02BEH
RDKBD: EQU 03BAH		;keyboard interrupt for timer
UPDDT: EQU 044CH		;for display in data fields (2 displays)
UPDAD: EQU 0440H		;for display in address fields (4 displays)

CALL INIT

START:
  LOOP1:
    MVI A, 18H
    STA 8011H
    LDA 8012H
    CPI 00H
    JZ INCREMENT_SECOND
    LDA 8012H
    DCR A
    STA 8012H
    JMP LOOP2

  LOOP2:
    MVI A, 4CH
    STA 8010H
    LDA 8011H
    CPI 00H
    JZ LOOP1
    LDA 8011H
    DCR A
    STA 8011H
    JMP LOOP3

  LOOP3:
    LDA 8010H
    CPI 00H
    JZ LOOP2
    LDA 8010H
    DCR A
    STA 8010H
    JMP LOOP3

  INCREMENT_SECOND:
    LDA 8002H
   	ANI 0FH
   	XRI 09H
   	LDA 8002H
   	JZ INR_6_SECOND

   	BACK_SECOND:
    INR A
    STA 8002H
    CPI 60H
    JZ INCREMENT_MIN
    CALL INIT
    JMP LOOP1

  INCREMENT_MIN:
    CALL RESET_SECOND
    LDA 8001H
    ANI 0FH
   	XRI 09H
   	LDA 8001H
   	JZ INR_6_MIN

   	BACK_MIN:
    INR A
    STA 8001H
    CPI 60H
    JZ INCREMENT_HOUR
    CALL INIT
    JMP LOOP1

  INCREMENT_HOUR:
    CALL RESET_MIN
    LDA 8000H
    ANI 0FH
   	XRI 09H
   	LDA 8000H
   	JZ INR_6_MIN

    BACK_HOUR:
    INR A
    STA 8000H
    CPI 24H
    JZ RESET_HOUR
    CALL INIT
    JMP LOOP1

  INR_6_SECOND:
  	INR A
  	INR A
  	INR A
  	INR A
  	INR A
  	INR A

    JMP BACK_SECOND

  INR_6_MIN:
  	INR A
  	INR A
  	INR A
  	INR A
  	INR A
  	INR A

    JMP BACK_MIN

  INR_6_HOUR:
  	INR A
  	INR A
  	INR A
  	INR A
  	INR A
  	INR A

    JMP BACK_HOUR


INIT:
  MVI A, 4CH
  STA 8010H
  MVI A, 18H
  STA 8011H
  STA 8012H
  CALL DISPLAY_SECOND
  CALL DISPLAY_MIN
  CALL DISPLAY_HOUR
  RET

RESET:
  MVI A, 00H
  STA 8002H
  STA 8001H
  STA 8000H
  MVI A, 18H
  STA 8010H
  STA 8011H
  STA 8012H
  RET

RESET_SECOND:
  MVI A, 00H
  STA 8002H
  RET

RESET_MIN:
  MVI A, 00H
  STA 8001H
  RET

RESET_HOUR:
  MVI A, 00H
  STA 8000H
  JMP START


DISPLAY_SECOND:
  LDA 8002H
  STA 8FF1H
  CALL UPDDT 
  RET

DISPLAY_MIN:
  LDA 8001H
  STA 8FEFH
  CALL UPDAD 
  RET

DISPLAY_HOUR:
  LDA 8000H
  STA 8FF0H
  CALL UPDAD 
  RET

