#include <WiFi101.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <String.h>

//******clock and globals******
#define BUFFER_SIZE 200
#define CPU_HZ 48000000
#define TIMER_PRESCALER 1024
#define FREQUENCY_HZ 20
#define COMF_PIN 6

//******internet connection and mqtt******
const char* ssid = "nahnahnahnah";
const char* password = "cheese11";
IPAddress ServeR = {52, 204, 229, 101}; // Amazon
//IPAddress ServeR = {72, 227, 147, 224}; //Kyle

WiFiClient WIFIclient;
PubSubClient client(WIFIclient);

//******accelerometer******
const int VCC = A1;
const int Xaxis = A2;
const int Yaxis = A3;
const int Zaxis = A4;
const int GND = A5;

//******fitai_variables******
char msg[1032];
int xaxis, yaxis, zaxis;
int root_mean_square;
int dataBuffer[BUFFER_SIZE] = {0};
int buffer_position = 0;
String packetTotal, packetHeader, packetData, packetTail;


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;//waiting for srial communication to start
  }
  Serial.println("Communication with the computer started");
  Serial.println();
  Serial.print("Trying to connect to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (!WL_CONNECTED) {
    delay(10);
    Serial.print(".");
  }
  Serial.println("Connected");
  connect_to_mqtt();

  //ACCELEROMETER SETUP
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

  //Debug LED
  pinMode(COMF_PIN, OUTPUT);

  packetHeader = "{\"header\": {\"athlete_id\": 123,\"lift_id\": 0,\"lift_sampling_rate\": 50,\"lift_start\": \"2012-04-23T18:25:43.511Z\",\"lift_type\" : \"deadlift\", \"lift_weight\": 100,\"lift_weight_units\": \"lbs\",\"lift_num_reps\": 10},\"content\":{\"a_x\": [";
  packetTail = "]}}";

  startTimer(FREQUENCY_HZ);

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
}

void connect_to_mqtt() {
  client.setServer(ServeR, 1883);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  //there are not gioing to be any messages received
  //for right now, so we can skip this
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

void setTimerFrequency(int frequencyHz) {
  int compare_value = (CPU_HZ / (TIMER_PRESCALER * frequencyHz)) - 1;
  TcCount16* TC = (TcCount16*) TC3;

  TC->CC[0].reg = compare_value;
  while (TC->STATUS.bit.SYNCBUSY == 1);
}

//This is where reading the IMU happens
void TC3_Handler() {
  TcCount16* TC = (TcCount16*) TC3;
  //If the time hits are value, this code runs
  if (TC->INTFLAG.bit.MC0 == 1) {
    TC->INTFLAG.bit.MC0 = 1;
    recordData();
    //Serial.println(buffer_position);
  }
}

void recordData() {

  //read all the axis
  xaxis = analogRead(Xaxis);
  yaxis = analogRead(Yaxis);
  zaxis = analogRead(Zaxis);

  //calculate the RMS
  root_mean_square = sqrt(xaxis * xaxis + yaxis * yaxis + zaxis * zaxis);

  //record it into the buffer
  dataBuffer[buffer_position] = root_mean_square;

  //if buffer is full, send the data
  if (buffer_position == BUFFER_SIZE - 1) {
    if (!client.connected()) {
      reconnect();
    }
    packetData = dataBuffer[0];

    //copy all the numbers into string
    for (int i = 1; i < BUFFER_SIZE; i++) {
      packetData = packetData + ',' + dataBuffer[i];
    }
    buffer_position = 0;

    //put all th eparts of the packet together
    packetTotal = packetHeader + packetData + packetTail;
    client.loop();
    packetTotal.toCharArray(msg, 1032);
    Serial.println(msg);
    client.publish("fitai", msg );
    packetData = "";
  }
  else {
    buffer_position++;
  }
}
