#include <WiFi101.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <String.h>

#define BUFFER_SIZE 200

//Clock parameters
#define CPU_HZ 48000000
#define TIMER_PRESCALER 1024

//Sample rate for reading the accelerometer
#define FREQUENCY_HZ 2

const char* ssid = "nahnahnahnah"; //your SSID
const char* password = "cheese11"; //your Password

//Not the best way to use A1 and A5, but for right now
//it is convenient to jsut plug in sensor on the breadboard
const int VCC = A1;
const int Xaxis = A2;
const int Yaxis = A3;
const int Zaxis = A4;
const int GND = A5;

char msg[1200];
int xaxis, yaxis, zaxis;
int root_mean_square;
int dataBuffer[BUFFER_SIZE] = {0};
int buffer_position=0;

String packetTotal, packetHeader, packetData, packetTail;

IPAddress ServeR = {52, 204, 229, 101}; // AWS

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
  startTimer(FREQUENCY_HZ);
}

void loop() {
  delay(1000);

  scanTag(); //If bracelet is close, scan it.

  if (!client.connected()) {
    reconnect();
  }

  packetTotal = packetHeader + packetData + packetTail;
  client.loop();
  packetTotal.toCharArray(msg, 1200);
  Serial.println(packetTotal);
  Serial.println(msg);
  packetData = "";
}

void setTimerFrequency(int frequencyHz) {
  int compare_value = (CPU_HZ / (TIMER_PRESCALER * frequencyHz)) - 1;
  TcCount16* TC = (TcCount16*) TC3;

  TC->CC[0].reg = compare_value;
  while (TC->STATUS.bit.SYNCBUSY == 1);
}

void startTimer(int frequencyHz) {
  REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID (GCM_TCC2_TC3)) ;
  while ( GCLK->STATUS.bit.SYNCBUSY == 1 );

  TcCount16* TC = (TcCount16*) TC3;

  TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;

  TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
  while (TC->STATUS.bit.SYNCBUSY == 1);

  TC->CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
  while (TC->STATUS.bit.SYNCBUSY == 1);

  TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024;
  while (TC->STATUS.bit.SYNCBUSY == 1);

  setTimerFrequency(frequencyHz);

  TC->INTENSET.reg = 0;
  TC->INTENSET.bit.MC0 = 1;

  NVIC_EnableIRQ(TC3_IRQn);

  TC->CTRLA.reg |= TC_CTRLA_ENABLE;
  while (TC->STATUS.bit.SYNCBUSY == 1);
}

//This is where reading the IMU happens
void TC3_Handler() {
  TcCount16* TC = (TcCount16*) TC3;
  //If the time hits are value, this code runs
  if (TC->INTFLAG.bit.MC0 == 1) {
    TC->INTFLAG.bit.MC0 = 1;
    recordData();
  }
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
  
  //read all the axis
  xaxis = analogRead(Xaxis);
  yaxis = analogRead(Yaxis);
  zaxis = analogRead(Zaxis);

  //calculate the RMS
  root_mean_square = sqrt(xaxis * xaxis + yaxis * yaxis + zaxis * zaxis);

  //record it into the buffer
  dataBuffer[buffer_position]=root_mean_square

  //if buffer is full, send the data
  if(buffer_position=BUFFER_SIZE-1){
    
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



