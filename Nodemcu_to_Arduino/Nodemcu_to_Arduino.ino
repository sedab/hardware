#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <String.h>

#define SIZE_OF_LIFT_ARRAY 200
#define PACKET_LENGTH 8
SoftwareSerial mySerial(12, 13); // RX(12)=D6 |  TX(13)=D7 (D6 & D7 are the
// the labels on the NodeMCU board correspoinding to the RX and TX

const char* ssid = "nahnahnahnah"; // your network SSID
const char* password = "cheese11"; // your network password
//const uint8_t mqtt_server[] = {52, 204, 229, 101}; //AWS for fitai

//const char* mqtt_server="ec2-52-204-229-101.compute-1.amazonaws.com";
//const uint8_t mqtt_server[] = {72, 227, 147, 224}; //Kyle's computer from home
const uint8_t mqtt_server[] = {127, 0, 0, 1}; //Boris' computer (local)



WiFiClient espClient;
//PubSubClient client(espClient);
PubSubClient client(espClient);
long lastMsg = 0;
char msg[200];
int value = 0;
int liftArray[SIZE_OF_LIFT_ARRAY] = {};
int counter = 0;
String packet1 = "";
String packet2 = "'header': 'athlete_id': 0, 'lift_id': 0, 'lift_sampling_rate': 50, 'lift_start': 'CURRENT_TIME ";
//const char* packet3[300]={};
String packet;




void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("fitai", "Welcome to FitAI"); //outTopic
      // ... and resubscribe
      client.subscribe("fitai"); //inTopic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(9600);
  delay(10);
  Serial.println("Serial began...");

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  mySerial.begin(9600);
  delay(10);
  Serial.println("mySerial began...");
}

void loop() {

  Serial.println("Beginngin of the loop");
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  Serial.println(mySerial.available());
  /*
    if (counter != 200 /* && mySerial.available()) {
    Serial.println("Kurac");
    packet1 += mySerial.readString();
    packet1 += ",";
    counter++;
    Serial.println(counter);
    }
    else if (counter == 200)
    {
  */
  //JSON
  packet = packet2 + /*packet1*/ + "}}";
  Serial.println(packet);
  packet.toCharArray(msg, 200);
  Serial.println(msg);
  client.publish("fitai", msg);
  packet1 = "";
  counter = 0;
  delay(2000);
}


