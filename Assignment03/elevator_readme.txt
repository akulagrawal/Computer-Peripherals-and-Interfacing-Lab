------------Elevator-------------


# Group No. 4
        
        - Abhishek Suryavanshi - 160101009
        - Ameya Daigavane      - 160101082
        - Nitesh Jindal        - 160101084
        - Akul Agrawal         - 160101085


-----------------------------------------




Memory Locations Used:
; --------------------------------
8000H - floor
8001H - direction
8002H - bitmap
8003H - count
8004H - flag capacity
8005H - capacity
800BH - boss
800FH - delay flag
8006H - ground flag
8010-8012H - temporary locations
; --------------------------------


Algorithm:
Initially, the elevator is at the ground floor. The LEDs on the left indicate the current floor of the elevator at each instant of time. 
To signal a request to go down, there are 8 switches - one for each floor.
When going up the elevator goes to the highest floor and then descends downward to the ground floor, picking up passengers until the max capacity is reached.
The maximum capacity can be set in memory location 8005H.


The elevator never changes direction when new requests are coming in, but can service those on the way if there is space. The remaining passengers will be taken on the next trip after the elevator goes to the ground floor.


We assume that on each floor only one person boards the lift for calculation purposes.
For the LED display purposes, we use the loops from the first part.


For the boss floor, we handle the boss first, and then service the remaining passengers until maximum capacity is reached. This gives the boss faster trips as compared to an ordinary passenger. The boss floor can be set in memory location 800BH.