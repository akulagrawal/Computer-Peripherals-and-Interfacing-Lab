import RPi.GPIO as gpio
import time

gpio.setmode(gpio.BCM)

A_Pin = 24
B_Pin = 25
C_Pin = 26
D_Pin = 13
E_Pin = 5

## Output Pins
gpio.setup(A_Pin, gpio.OUT)
gpio.setup(B_Pin, gpio.OUT)

## Input Pins
gpio.setup(C_Pin, gpio.IN)
gpio.setup(D_Pin, gpio.IN)
gpio.setup(E_Pin, gpio.IN)

gpio.output(A_Pin, False)
gpio.output(B_Pin, False)

# setup the raspi board
time.sleep(1)

Sel_C = gpio.input(C_Pin)
Sel_D = gpio.input(D_Pin)
Sel_E = gpio.input(E_Pin)

A_val = True
B_val = False

gpio.output(A_Pin, A_val)
gpio.output(B_Pin, B_val)

TimePeriod_orig = 0.5
Time_working = TimePeriod_orig

while(Sel_C != True):

    Sel_E = gpio.input(E_Pin)
    if(Sel_E == False):
        A_val = True
        B_val = True
        gpio.output(A_Pin, A_val)
        gpio.output(B_Pin, B_val)
        time.sleep(5)
        break
    
    Sel_D = gpio.input(D_Pin)
    if(Sel_D == True):
        Time_working = TimePeriod_orig/2
    else:
        Time_working = TimePeriod_orig
        
    gpio.output(A_Pin, A_val)
    gpio.output(B_Pin, B_val)
    time.sleep(Time_working)

    A_val = not A_val
    B_val = not B_val
    gpio.output(A_Pin, A_val)
    gpio.output(B_Pin, B_val)
    time.sleep(Time_working)

    Sel_C = gpio.input(C_Pin)

#while ends   

A_val = False
B_val = False
gpio.output(A_Pin, A_val)
gpio.output(B_Pin, B_val)

gpio.cleanup()





















