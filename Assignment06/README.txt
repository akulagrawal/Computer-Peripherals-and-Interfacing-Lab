-----------------------------Raspberry Pi Basics------------------------------


# Group No. 4


        - Abhishek Suryavanshi - 160101009
        - Ameya Daigavane      - 160101082
        - Nitesh Jindal        - 160101084
        - Akul Agrawal         - 160101085


-------------------------------------------------------------------------------
PART A


Input -                
                C : gpio26
                D : gpio13
                E : gpio05


Output -
                A : gpio24
                B : gpio25


While C is set to false, the LEDs light alternatively, initially with a time period of 0.5 sec.
A variableTime_working stores the time period of lighting of each LED, which is halved if D is set to true, and restored back if D is set to false.
If E is set to false, the program halts after lighting both the LEDs for 5 seconds.


-------------------------------------------------------------------------------
PART B


Output -
                    led : gpio12


We use the .ChangeDutyCycle(duty) function to change the duty cycle of the wave.
The pi_pwm object allows us to interface with the physical pin.
Below is a sample of the code.






pi_pwm = GPIO.PWM(ledpin,2000)                        # create PWM instance with frequency
pi_pwm.start(0)                                               # start PWM of required Duty Cycle


low = 10
high = 90


while True:
    for duty in range(low, high + 1, 1):
        pi_pwm.ChangeDutyCycle(duty)                 # increase duty cycle from low-high
        sleep(0.01)
    sleep(0.1)
    
    for duty in range(high, low - 1, -1):
        pi_pwm.ChangeDutyCycle(duty)                 # decrease duty cycle from high-low
        sleep(0.01)
    sleep(0.1)
