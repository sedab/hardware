#include <SPI.h>
#include <WiFi101.h>
#include <PubSubClient.h>
#include <String.h>

//Boris' home WiFi
const char ssid[] = "nahnahnahnah";
const char password[] = "cheese11";

//Kyle's computer address (mqtt server)
const uint8_t mqtt_server[] = {72, 227, 147, 224};

//for debuging purposes
int counter = 0;
const char* msg;

WiFiClient mkrClient;
PubSubClient client(mkrClient);

//connection to the network
void connect_to_wifi() {

  delay(10);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("You are connected to ");
  Serial.println(ssid);
  Serial.print("Local IP:");
  Serial.println(WiFi.localIP());
}

//callback
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

//reconnecting
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("MKRClient")) {
      Serial.println("connected");
      client.publish("fitai", "Welcome to FitAI");
      client.subscribe("fitai");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  //Starting Serial communication with the computer
  Serial.begin(9600);
  while(!Serial){
    ;//waiting to serail to connect
  }
  Serial.println("Serial communication with the PC started...");

  connect_to_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}



void loop() {
  if (!client.connected()) {
    reconnect();
  }
  msg=(const char*)counter;
  Serial.println(msg);
  client.publish("fitai", msg);
  counter++;
  delay(2000);
}
