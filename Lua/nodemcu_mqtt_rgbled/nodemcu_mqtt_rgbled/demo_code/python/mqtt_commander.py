#!/usr/bin/env python3

'''
Simple MQTT PUB/SUB terminal client example
by Patrick Lloyd

This simple MQTT client allows the user to send messages to an ESP8266
running NodeMCU in order to interact with an RGB LED. The client ubscribes
to the "/mcu/#" topic and prints all messages from the "/mcu/rgbled_status/"
topic.

Valid inputs are only the numbers 0-3 but this can be expanded to accomodate
more complex projects.
'''

# Library to connect with the broker
# See http://www.eclipse.org/paho/ for more info
import paho.mqtt.client as mqtt

# ----- CHANGE THESE FOR YOUR SETUP -----
MQTT_HOST = "192.168.2.5"
MQTT_PORT = 1883
# ---------------------------------------


# The callback function for when the client connects to broker
def on_connect(client, userdata, rc):
    print("\nConnected with result code " + str(rc) + "\n")

    #Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("/mcu/#")  # Connect to everything in /mcu topic
    print("Subscibed to /mcu/#")


# The callback function for when a message on /mcu/rgbled_status/ is published
def on_message_rgbled(client, userdata, msg):
    print("\n\t* LED UPDATED ("+msg.topic+"): " + str(msg.payload))


# Call this if input is invalid
def command_error():
    print("Error: Unknown command")


# Create an MQTT client instance
client = mqtt.Client(client_id="python-commander")

# Callback declarations (functions run based on certain messages)
client.on_connect = on_connect
client.message_callback_add("/mcu/rgbled_status/", on_message_rgbled)

# This is where the MQTT service connects and starts listening for messages
client.connect(MQTT_HOST, MQTT_PORT, 60)
client.loop_start()  # Background thread to call loop() automatically

# Main program loop
while True:

    # Get basic user input and process it
    animate_msg = input(
        "\n(0 = OFF, 1 = R-G-B, 2 = Random Breathe, 3 = Disco): ")

    # Check the input and sent it to the broker if it's valid
    if animate_msg == "0":
        client.publish("/mcu/cmd/animate", payload="0", qos=0, retain=False)
    elif animate_msg == "1":
        client.publish("/mcu/cmd/animate", payload="1", qos=0, retain=False)
    elif animate_msg == "2":
        client.publish("/mcu/cmd/animate", payload="2", qos=0, retain=False)
    elif animate_msg == "3":
        client.publish("/mcu/cmd/animate", payload="3", qos=0, retain=False)
    else:
        command_error()
