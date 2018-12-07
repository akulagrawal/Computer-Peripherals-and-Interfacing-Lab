import paho.mqtt.publish as publish
 
MQTT_SERVER = "10.42.0.1"
MQTT_PATH = "test"

with open ("coords.txt", "r") as f:
	inputstr = f.read()

publish.single(MQTT_PATH, inputstr, hostname=MQTT_SERVER)