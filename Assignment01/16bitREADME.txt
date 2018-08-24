------------16 Bit Calculator-------------


# Group No. 4

	- Abhishek Suryavanshi - 160101009
	- Ameya Daigavane      - 160101082
	- Nitesh Jindal        - 160101084
	- Akul Agrawal         - 160101085

-----------------------------------------



Memory locations in use
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

Contents of 8001H memory location is used to multiplex between the operations:

	01H ---- Addition
	02H ---- Subtraction
	03H ---- Multiplication
	04H ---- Division

There are 2 additional routines-
* Copy Input
* Copy Output

These are used because the arithmetic functions addition and subtraction are used as subroutines in the multiplication and division operations,
and the temporary output and input need to be transferred between the actual inputs and outputs.

* Addition:

	Adds contents of locations 8012H-8013H and 8014H-8015H and stores the result in 8018H-8019H memory locations.
	The carry is stored as the lowest order bit in memory location 8017H.

	-Working:

	Adds 8 bits at a time - first the lower order bits and then the higher order bits.
    Note that the addition of two 8-bit numbers can result in at maximum a 9-bit number.
    Carry is checked for, and an increment is performed at the next stage if required.

	Halt

* Subtraction:

    Takes the ones complement of 8014H-8015H 8 bits at a time.
    Increments this value to get the twos complement.
    The carry is thrown away.
    Then adds to the values stored in 8012H-8013H using the addition subroutine defined above.

	Halt

* Multiplication:

	Multiplies contents of location 8012H-8013H and 8014H-8015H and stores the result in 8006H-8009H memory locations.

	-Working:

    The algorithm is exactly the same as the 8-bit multiplicator, with the difference being in implementation of the basic arithmetic operations.
    We cannot use the ADD and SUB instructions directly as they operate on 8-bit operands.

    Instead, for adding and subtracting, we use the addition and subtraction subroutines defined above.
    For checking carry, we check the contents of the 8017H register after performing an addition using the CPI instruction.
    Starting with zero, we keep adding the first input value, using the other value as a counter.
    When the other value reaches zero, we know that we have finished.


* Division:

	Divides contents of location 8002H-8003H by 8004H-8005H and stores the result in 8006H-8007H (Quotient) and 8008H-8009H (Remainder) memory location.

	-Working:

	The Algorithms is same as that of 8 bit division(Division by repeated subtraction)
	Program first loads contents of 8002H memory location in the HL register pair.
	L stores Higher order 8 bits while H stores lower order 8 bits
	load quotient(DE register pair) as zero
	while(L is not zero)
	{
		load H in A
		while(H is non negative)
		{
			subtract divisor from A
			store in H
			increment D
		}
	}
	while(H is positive)
	{
		subtract divisor from A
		if(A is negative)
			break
		else
		{
			increment D
			store A in H
		}
	}
	
	H-L has the remainder
	D-E has the quotient
	store the result in appropriate location
	
	Halt
