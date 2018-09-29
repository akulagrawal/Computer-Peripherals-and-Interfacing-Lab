import RPi.GPIO as GPIO
from time import sleep

ledpin = 12  			# PWM pin connected to LED
GPIO.setwarnings(False)			#disable warnings
GPIO.setmode(GPIO.BCM)		#set pin numbering system
GPIO.setup(ledpin,GPIO.OUT)
pi_pwm = GPIO.PWM(ledpin,2000)		#create PWM instance with frequency
pi_pwm.start(0)       #start PWM of required Duty Cycle

low = 10
high = 90

while True:
    for duty in range(low, high + 1, 1):
        pi_pwm.ChangeDutyCycle(duty) #provide duty cycle in the range 0-100
        sleep(0.01)
    sleep(0.1)
    
    for duty in range(high, low - 1, -1):
        pi_pwm.ChangeDutyCycle(duty)
        sleep(0.01)
    sleep(0.1)