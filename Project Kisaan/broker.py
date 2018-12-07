import paho.mqtt.client as mqtt
import os

MQTT_SERVER = "10.42.0.1"
MQTT_PATH = "broker_data"
MQTT_RECIEVER = "192.168.43.99"
 
# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
 	
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe(MQTT_PATH)
 
# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(str(msg.payload))

    # Write to file 'sensordata.txt'
    with open('sensordata.json', 'w') as f:
    	f.write(str(msg.payload))

    # publish.single(MQTT_PATH, str(msg.payload), hostname = MQTT_RECIEVER)

# Initialize client    
client = mqtt.Client()

# Set callbacks
client.on_connect = on_connect
client.on_message = on_message
 
# Connect to MQTT server
client.connect(MQTT_SERVER, 1883, 60)
 
# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()