'''
Kisaan: The Farm-Monitoring System
RaspberryPi MQTT Code

Group 4
'''

import serial
import paho.mqtt.publish as publish
import time
import json
import os

# Serial interface object to get Arduino data over a serial connection.
serial_interface = serial.Serial('/dev/ttyACM1', 9600, timeout=2)
time.sleep(2)

# MQTT parameters
MQTT_SERVER = "10.42.0.1"
MQTT_PATH = "broker_data"

# Last known coordinates of the Arduino
x_last = "0000"
y_last = "0000"
z_last = "0000"

# Get sensor readings from the serial interface as a string
def get_sensor_readings():
    return str(serial_interface.readline())

# Convert string to dict, return None if we can't parse correctly.
def to_dict(s):
    try:
        # Split string according to the comma.
        data_vals = [int(valstring) for valstring in s.split(',')[:-1]]
        
        # Incorrect data read.
        if len(data_vals) != 6:
            print("Parsed received data length incorrect.")
            return None
        
        # Make dict from values obtained.
        data_fields = ['x', 'y', 'z', 'humidity', 'temperature', 'gas']
        data_dict = dict(zip(data_fields, data_vals))
            
        # More data validation.
        if (data_dict['x'] < 0 or data_dict['x'] > 2500) or (data_dict['y'] < 0 or data_dict['y'] > 1200) or (data_dict['z'] < 0 or data_dict['z'] > 500) or (data_dict['temperature'] < 0 or data_dict['temperature'] > 100):
            print("Data in invalid ranges.")
            return None
        
        # Update old coordinates
        x_last = s.split(',')[0].zfill(4)
        y_last = s.split(',')[1].zfill(4)
        z_last = s.split(',')[2].zfill(4)
        
        # Print to 'oldcoords.txt'
        with open("oldcoords.txt", "w+") as f:
            f.write(x_last + "," + y_last + "," + z_last + "\n")
            
        # Add current time.
        data_dict['time'] = time.strftime("%H:%M %d-%m", time.localtime())
        return data_dict
    
    except (ValueError, TypeError):
        print("Invalid data read.")
        return None

# Send instructions to the Arduino
def send_values(inputstr):
    try:
        # Read line from stdin, split entries according to the comma
        line = inputstr.split(',')
        
        # Parse subsequences
        mode = int(line[0])
        x_input = int(line[1])
        y_input = int(line[2])
        z_input = int(line[3])
        
        input_string = line[0] + "," + line[1] + "," + line[2] + "," + line[3] + "\n"
        print input_string
        
        # Send if valid
        if len(input_string) == 17 and (mode in [0, 1]) and (0 <= x_input <= 2500) and (0 <= y_input <= 1200) and (0 <= z_input <= 500):    
            serial_interface.write(input_string)
        else:
            print("Input string is of length", len(input_string))
            raise ValueError
            
    except (ValueError, TypeError):
        print("Invalid string input.")
        return None
    
print "Starting server..."

# Get last known coordinates from "oldcoords.txt"
try:
    with open("oldcoords.txt", "r") as f:
        print "Old coordinates found. Loading now..."
        first_line = f.readline()
        x_last = first_line.split(',')[0]
        y_last = first_line.split(',')[1]
except:
    pass
        
while(True):
    # Check for user input through the file 'coords.txt'
    print "Searching for user input..."
    try:        
        # Read the first line from the file
        with open("coords.txt", "r") as f:
            print "User input found."
            inputstr = f.readline()
            send_values(inputstr)
        
        # Delete input file!
        os.remove("coords.txt")
    except:
        print "User input not found."
        pass
    
    # Get sensor data from the Arduino
    readings = get_sensor_readings()
    print readings
    
    # If requested, return old coordinates
    if readings == "getcoords\r\n":
        print "Sending old coordinates..."
        serial_interface.write("0" + "," + x_last + "," + y_last + "," + z_last + "\n")
    
    # Parse sensor readings 
    else:
        print "Reading sensors..."
        readings_dict = to_dict(readings)
         
        if readings_dict is not None:
            print "valid readings: ", readings_dict 
            readings_JSON = json.dumps(to_dict(readings))
            publish.single(MQTT_PATH, readings_JSON, hostname=MQTT_SERVER)
            
