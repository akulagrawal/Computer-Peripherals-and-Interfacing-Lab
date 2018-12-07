'''
Kisaan: The Farm-Monitoring System
RaspberryPi Input Code

Group 4
'''

while(True):
    # Get input string from the command line
    input_str = raw_input("Enter mode, x, y and z: ")
    
    # Write to file 'coords.txt'
    with open("coords.txt", "w+") as f:
        f.write(input_str)
    
