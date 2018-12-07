import serial
import time

serial_interface = serial.Serial('/dev/ttyACM0', 9600, timeout=None)
time.sleep(1)

x = 4005
y = 1130
z = 1322

for count in range(1000, 1100):
    serial_interface.write(str(count)+ ',' + str(x) + ',' + str(y) + ',' + str(z) + '\n')
    print("received: " + serial_interface.readline())
