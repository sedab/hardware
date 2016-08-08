#!/usr/bin/env python3

'''
Simple MQTT loopback terminal client example
by Patrick Lloyd

This simple MQTT client requires no input from the user and is used to test
compatibility with the Mosquitto broker. It subscribes to the /loopback/hello
topic and once every two seconds publishes a message with a counter
'''

# Library to connect with the broker
# See http://www.eclipse.org/paho/ for more info
import paho.mqtt.client as mqtt
import time  # Can never get enough...

MQTT_HOST = ""    # Change for your setup
MQTT_PORT = 1883  # Change for your setup


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, rc):
    print("Connected with result code "+str(rc))
    #Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("loopback/hello")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+": "+str(msg.payload))


client = mqtt.Client(client_id="python-loopback")  # Create a client instance

# Callback declarations
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_HOST, MQTT_PORT, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_start()
index = 0
while True:
    index = index + 1
    time.sleep(2)
    client.publish("loopback/hello", payload="Hiss... buzz... #"+str(index),
                   qos=0, retain=False)
