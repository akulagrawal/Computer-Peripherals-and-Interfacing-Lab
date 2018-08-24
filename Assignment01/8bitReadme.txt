------------8 Bit Calculator-------------


# Group No. 4
	
	- Abhishek Suryavanshi - 160101009
	- Ameya Daigavane      - 160101082
	- Nitesh Jindal        - 160101084
	- Akul Agrawal         - 160101085

-----------------------------------------



Memory locations in use
; ----------------------------------------------------
; 8001 - Operation
; 8002 - Input 1
; 8003 -  Input 2
; 8004-05  - Output 
; ----------------------------------------------------

Contents of 8001H memory location is used to multiplex between the operations

	01H ---- Addition
	02H ---- Subtraction
	03H ---- Multiplication
	04H ---- Division

* Addition:

	Adds contents of location 8002H and 8003H and stores the result in 8004H memory location.
	If carry is generated it is stored in 8005H memory location.

	-Working:

	Program first loads contents of 8002H memory location into the accumulator and transfers it to
	B register.
	Next it loads contents of 8003H memory locaton into accumulator.

	Now add Reg B to Accumulator and store result in accumulator.

	Transfer Accumulator contents to memory location 8004H and if carry flag is set place 01H in 8005H.

	Halt

* Subtraction:
	
	Subtracts contents of memory location 8002H from contents of memory location 8003H ans stores the difference
	int 8004H memory location and if the result is negative 8005H stores 1 else it stores 0.

	-Working:
	
	Program first loads contents of 8002H memory location into the accumulator and transfers it to
	B register.
	Next it loads contents of 8003H memory locaton into accumulator.

	Now subtract Reg B from the accumulator and store result in accumulator.
	If borrow is generated it means that difference is negative and we will have to take two's complement of the contents of accumulator.

	Transfer Accumulator contents to memory location 8004H and if borrow flag is set (means difference is negative) place 01H in 8005H.

	Halt

* Multiplication:

	Multiplies contents of location 8002H and 8003H and stores the result in 8004 and 8005H memory location.
	
	-Working:

	Program first loads contents of 8002H memory location into the accumulator and transfers it to B register which will act as multiplicand.

	Next it loads contents of 8003H memory locaton into accumulator and transfers it to E register which will act as multiplier.

	Initialize Reg D to 00H this will hold the most significant 8 bits of product.
	Initialize accumulator to 00H.

	While content of Reg E is not zero
	{
		add Reg E to accumulator
		if carry is generated
			increase Reg D
		Decrement Reg E by one
	}

	Finally, store LSB (ie accumulator) into 8004H and store MSB (ie Reg D) into 8005H

	Halt

* Division:

	Divides contents of location 8003H by 8002H and stores the result in 8004H (Remainder)and 8005H (Quotient) memory location.

	-Working:

	Program first loads contents of 8002H memory location into the accumulator and transfers it to B register which will act as divisor.

	Initialize Reg C to 00H which will store Quotient at the wnd of program.

	Then load dividend (ie 8003H) into accumulator.

	While Reg B < accumulator
	{
		subtract Reg B from accumulator
		Increment Reg C by 1
	} 

	Finally shift contents of accumulator (ie remainder) to 8004H and Reg C ie(quotient) to 8005H.

	Halt




	 


