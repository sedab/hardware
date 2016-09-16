#include <WiFi101.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <String.h>

//#define MQTT_MAX_PACKET_SIZE 1500

const char* ssid = "nahnahnahnah";
const char* password = "cheese11";

int counter = 0;
int flag = 1;
byte bssid[6];
byte yourMAC[6];
IPAddress ip, subIPmask, gateway;
//IPAddress ServeR = {72, 227, 147, 224}; //Kyle
//IPAddress ServeR = {192, 168, 1, 204}; //Boris Local WIFI
//IPAddress ServeR = {192, 168, 1, 78};//Boris Local Ethernet
//IPAddress ServeR = {67,247,49,181}; //Boris Public
IPAddress ServeR = {52,204,229,101}; // Amazon

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  //there are not gioing to be any messages received
  //for right now, so we can skip this
}

WiFiClient WIFIclient;
PubSubClient client(WIFIclient);

long lastMsg = 0;
long randomNumber;
char msg[1200];
int value = 0;
int debugLED = 6;
String packetTotal, packetHeader, packetData, packetTail;

void recordData() {
  for (int i = 0; i < 199; i++) {
    randomNumber = random(100, 999);
    packetData += randomNumber;
    packetData += ",";
    delay(20);
  }
  randomNumber = random(100, 999);
  packetData += randomNumber;
}


void WiFiInfo() {
  Serial.println();
  Serial.println("WiFi info:");
  Serial.print("WiFi network name: ");
  Serial.println(WiFi.SSID());

  Serial.print("Local IP: ");
  ip = WiFi.localIP();
  Serial.println(ip);

  Serial.print("MAC address of the router: ");
  WiFi.BSSID(bssid);
  Serial.print(bssid[0]); Serial.print(":");
  Serial.print(bssid[1]); Serial.print(":");
  Serial.print(bssid[2]); Serial.print(":");
  Serial.print(bssid[3]); Serial.print(":");
  Serial.print(bssid[4]); Serial.print(":");
  Serial.println(bssid[5]);

  Serial.print("Signal strength: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dB");

  Serial.print("Ecnryption type: ");
  byte encryption = WiFi.encryptionType();
  if (encryption == 2) {
    Serial.println("TKIP (WPA)");
  }
  else if (encryption == 8) {
    Serial.println("AUTO");
  }
  else if (encryption == 4) {
    Serial.println("CCMP (WPA)");
  }
  else if (encryption == 5) {
    Serial.println("WEP");
  }
  else if (encryption == 7) {
    Serial.println("NONE");
  }

  Serial.print("Your MAC address:  ");
  WiFi.macAddress(yourMAC);
  Serial.print(yourMAC[0]); Serial.print(":");
  Serial.print(yourMAC[1]); Serial.print(":");
  Serial.print(yourMAC[2]); Serial.print(":");
  Serial.print(yourMAC[3]); Serial.print(":");
  Serial.print(yourMAC[4]); Serial.print(":");
  Serial.println(yourMAC[5]);

  Serial.print("Subnet mask: ");
  subIPmask = WiFi.subnetMask();
  Serial.println(subIPmask);

  Serial.print("Gateway: ");
  gateway = WiFi.gatewayIP();
  Serial.println(gateway);

  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("connecting to MQTT server");

    if (client.connect("MKRclient")) {
      Serial.println("Connected to the MQTT server!");
      digitalWrite(debugLED, HIGH);
      delay(200);
      digitalWrite(debugLED, LOW);
      delay(200);
      digitalWrite(debugLED, HIGH);
      delay(200);
      digitalWrite(debugLED, LOW);
      delay(200);
      digitalWrite(debugLED, HIGH);
      delay(200);
      digitalWrite(debugLED, LOW);
      delay(200);
      client.publish("fitai", "Hi, I am a sensor");
      client.subscribe("fitai");
    }
    else {
      Serial.print("Error connecting to the server: ");
      Serial.println(client.state());
      Serial.println("Will try again in 4 sec...");
      delay(4000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;//waiting for srial communication to start
  }
  Serial.println("Communication with the computer started");
  Serial.println();

  pinMode(debugLED, OUTPUT);
  digitalWrite(debugLED, HIGH);
  delay(2000);
  digitalWrite(debugLED, LOW);

  //awailable networks
  Serial.print("Number of networks awilable: ");
  byte numSsid = WiFi.scanNetworks();
  Serial.println(numSsid);

  //connecting to wifi
  Serial.println();
  Serial.print("Trying to connect to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (!WL_CONNECTED) {
    delay(10);
    Serial.print(".");
  }
  Serial.println("Connected");

  WiFiInfo();

  //connecting to MQTT server
  client.setServer(ServeR, 1883);
  client.setCallback(callback);

  packetHeader = "{\"header\": {\"athlete_id\": Tim_Nugmanov,\"lift_id\": 0,\"lift_sampling_rate\": 50,\"lift_start\": \"2012-04-23T18:25:43.511Z\",\"lift_type\" : \"deadlift\", \"lift_weight\": 100,\"lift_weight_units\": \"lbs\",\"lift_num_reps\": 10},\"content\":{\"a_x\": [";
  packetTail = "]}}";
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  recordData();
  packetTotal = packetHeader + packetData + packetTail;
  client.loop();
  packetTotal.toCharArray(msg, 1200);
  //packet1.toCharArray(msg, 1000);
  //packet2.toCharArray(msg, 1000);
  Serial.println(msg);
  Serial.println(packetTotal);
  //packet1="what'up\t what's up";
  //packet1.toCharArray(msg, 19);
  //delay(2000);

  //client.publish("fitai", msg );
  Serial.println(msg);
  delay(9000);
  packetData = "";



}
