; ----------------------------------------------------
# Group No. 4

        - Abhishek Suryavanshi         - 160101009
        - Ameya Daigavane             - 160101082
        - Nitesh Jindal                        - 160101084
        - Akul Agrawal                         - 160101085
; ----------------------------------------------------

-------------------Clock-------------------

Memory locations in use
; ----------------------------------------------------
;         8000H - hours
;         8001H - minute
;         8002H - seconds
;         8010-8012H - temporary locations for loop counters
; ----------------------------------------------------

The idea is to generate a sufficient delay through a cycle of nested ‘for’ loops.
We use three nested ‘for’ loops - counting down from an initial value in each loop. 
Every time the combined ‘for’ loops finish, which happens when all the three counters reach zero, we increment the second. We choose the initial values for the counters to ensure that exactly one second has passed when they finish.


Once the loops finish, if the current second is greater than 59, we increment the minute, and reset the second to 0.
Similarly, if the current minute is greater than 59, we increment the hour, and reset the minute to 0.

For the actual addition, we have the INCREMENT_SECOND, INCREMENT_MIN and INCREMENT_HOUR functions.


To convert from hexadecimal format to decimal format, every time we are at a point of incrementing a 9, to display 10, we add 6 to it in addition to 1. (Since 16 in hexadecimal is 10 in decimal).

These are used because the arithmetic functions addition and subtraction are used as subroutines in the multiplication and division operations,
and the temporary output and input need to be transferred between the actual inputs and outputs.





________________




-------------------Timer-------------------


Memory locations in use:
; ----------------------------------------------------
;         8000H - hours
;         8001H - minute
;         8002H - seconds
;        8010-8012H - temporary locations for loop counters
;        800AH - timer flag (should be set to 1 for timer to work)
;        8FBFH - JMP 9400H
; ----------------------------------------------------

Similar to the working of the 24 hour clock, except that the clock runs backward, we have three nested ‘for’ loops[a][b][c][d][e][f][g][h][i][j][k][l] - when these complete, we decrement the second through DECREMENT_SECOND.
The initial values of the counters in the loops are the same as in the clock.
To handle the cases when the second has reached 0, we call the DECREMENT_MIN function and reset the seconds.
Similarly, when the minutes reach 0, we call the DECREMENT_HOUR function and reset the minutes.
Decrement is done accordingly so as to be consistent with the decimal notation on display.


The key difference with the clock is then, with the interrupts described below:
RST 7.5 interrupt is activated by the KBINT key on the keyboard. The interrupt makes the program to execute from the location 8FBFH in memory. Since sufficient amount of memory is not free here, this location contains a jump instruction to location of ISR that is 9400H.
In the ISR, RDKBD is called that demands input from the keyboard. Countdown remains paused till two key presses are detected - no further execution is done. The countdown resumes once any two keys are pressed on the keyboard.
