----------------Motor---------------------

# Group No. 4

	- Abhishek Suryavanshi - 160101009
	- Ameya Daigavane      - 160101082
	- Nitesh Jindal        - 160101084
	- Akul Agrawal         - 160101085

-----------------------------------------


Memory Locations Used
; --------------------------------------------------------------
; 8000H - digitally converted signal value
; 8001H - motor bit pattern
; 8002H - current step count
; 8003H - step value to be reached based on current signal value
; --------------------------------------------------------------

We use a ADC to convert the analog signal into a digital value, approximated.
The ADC asserts the EOC (End of Conversion) signal at the D0 bit at a certain address.
We keep polling for this signal to go high (with POLL and WAIT_EDGE in the code), and can then read the output from the ADC, storing it at the memory location 8000H, in the subroutine DATA_READY.

To rotate the motor, we set the configuration of the motor's 4 magnets using the 4 least significant bits of Port A, in SHIFT_MOTOR.
Initially, for the magnets, we have a configuration of 88 in hexadecimal, which corresponds to 10001000 in binary. We can see that only one of the magnets will be turned on.
Further, on rotating this configuration, in any direction, only one of the magnets will be turned on at any point of time, and since this will be adjacent to the magnet previously turned on, the effect will be to rotate the motor by one step.
We cannot change the magnet configurations too fast because it takes time for the shaft to rotate to align itself according to the configuration.
For this reason we call MOTOR_DELAY. Now that we have the digital value of the signal, we use a delay that decreases with increase in the value of the signal.
This has the effect of slowing down the motor when the applied analog signal is small, and speeding it up when the amplitude increases.
