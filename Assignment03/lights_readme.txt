------------Timed Lights-------------


# Group No. 4
        
        - Abhishek Suryavanshi         - 160101009
        - Ameya Daigavane              - 160101082
        - Nitesh Jindal                        - 160101084
        - Akul Agrawal                          - 160101085


-----------------------------------------


We send 8B as control word which configures Port A as output port and Port B and C as input ports.


The contents of the dip-switches are available as and when they are changed as inputs are not latched in mode 0 of 8255.


We start with a variable initialized with 01H value and after each second (counted by a delay function implemented as three for loops) we rotate it by one using the RLC instruction and output this value at Port A.


We check if D5 is made low - if so, we do not rotate and instead output the unchanged value to the LEDs (via port A).
Here, a flag is set which denies further rotation. This flag is unset only when D6 is made low.


Similarly if D2 is made low then a function is called which simply halts the program, by an RST 5 instruction.