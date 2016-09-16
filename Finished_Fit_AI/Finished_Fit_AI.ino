#include <WiFi101.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <String.h>

#define BUFFER_SIZE 200

const char* ssid = "nahnahnahnah"; //your SSID
const char* password = "cheese11"; //your Password

const int VCC = A1;
const int Xaxis = A2;
const int Yaxis = A3;
const int Zaxis = A4;
const int GND = A5;

char msg[1200];
int xaxis, yaxis, zaxis;
int root_mean_square;
int dataBuffer[BUFFER_SIZE];

String packetTotal, packetHeader, packetData, packetTail;

IPAddress ServeR = {52, 204, 229, 101}; // Amazon

WiFiClient WIFIclient;
PubSubClient client(WIFIclient);


void setup() {
  Serial.begin(9600);  //MKR1000 USB Serial communication
  while (!Serial) {
    ;//waiting for srial communication to start
  }
  Serial1.begin(9600); //MKR1000 RX & TX Serial communication

  //OUTPUTS
  pinMode(VCC, OUTPUT);
  pinMode(GND, OUTPUT);

  //INPUTS
  pinMode(Xaxis, INPUT);
  pinMode(Yaxis, INPUT);
  pinMode(Zaxis, INPUT);

  //POWER SUPPLY FOR THE ACCELEROMETER
  digitalWrite(VCC, HIGH);
  digitalWrite(GND, LOW);

  Connect_to_wifi();
  Connect_to_mqtt();

  for (int i = 0; i < BUFFER_SIZE; i++) {
    dataBuffer[i] = 0;
  }
}

void loop() {
  delay(1000);
  
  scanTag(); //If bracelet is close, scan it.

  if (!client.connected()) {
    reconnect();
  }
  recordData();
  packetTotal = packetHeader + packetData + packetTail;
  client.loop();
  packetTotal.toCharArray(msg, 1200);
  Serial.println(packetTotal);
  Serial.println(msg);
  packetData = "";
}

void Connect_to_wifi() {
  Serial.println();
  Serial.print("Trying to connect to: ");
  Serial.println(ssid);
  while (!WL_CONNECTED) {
    WiFi.begin(ssid, password);
    delay(10);
    Serial.print(".");
  }
  Serial.println("Connected");

}

void Connect_to_mqtt() {
  client.setServer(ServeR, 1883);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  //there are not gioing to be any messages received
  //for right now, so we can skip this
}

void recordData() {
  xaxis = analogRead(Xaxis);
  yaxis = analogRead(Yaxis);
  zaxis = analogRead(Zaxis);
  root_mean_square = sqrt(xaxis * xaxis + yaxis * yaxis + zaxis * zaxis);
  for (int i = BUFFER_SIZE - 1; i > 0; i--) {
    dataBuffer[i] = dataBuffer[i - 1];
  }
  dataBuffer[0] = root_mean_square;
  if (abs(dataBuffer[0] - dataBuffer[1]) > 10) {
    for (int k = 0; k < BUFFER_SIZE; k++) {
      packetData += dataBuffer[k];
      packetData += ",";
    }
  }

}

void reconnect() {
  while (!client.connected()) {
    Serial.println("connecting to MQTT server");

    if (client.connect("MKRclient")) {
      Serial.println("Connected to the MQTT server!");
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


void printAxis() {
  Serial.print("X axis: ");
  Serial.print(analogRead(Xaxis));
  Serial.print(" Y axis: ");
  Serial.print(analogRead(Yaxis));
  Serial.print(" Z axis: ");
  Serial.println(analogRead(Zaxis));
}


void scanTag() {
  char tagString[13]; //There are 12 chars and a new line
  int index = 0;
  boolean reading = false;

  while (Serial1.available()) {

    int readByte = Serial1.read(); //read next available byte

    if (readByte == 2) reading = true; //begining of tag
    if (readByte == 3) reading = false; //end of tag

    if (reading && readByte != 2 && readByte != 10 && readByte != 13) {
      //store the tag
      tagString[index] = readByte;
      index ++;
    }
  }

  checkTag(tagString); //Check if it is a match
  clearTag(tagString); //Clear the char of all value
  delay(200); //This has to be here, otherwise it is not working
}


//Print out the tag ID - it should be compared with the
//actuall name on the Server
void checkTag(char tag[]) {
  if (strlen(tag) < 12) return; //empty or less than real ID, no need to contunue
  Serial.print("User ID: ");
  for (int i = 0; i < 12; i++) {
    Serial.print(tag[i]);
  }
  Serial.println("");
}

void clearTag(char one[]) {
  ///////////////////////////////////
  //clear the char array by filling with null - ASCII 0
  //Will think same tag has been read otherwise
  ///////////////////////////////////
  for (int i = 0; i < strlen(one); i++) {
    one[i] = 0;
  }
}



