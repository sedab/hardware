//Boris Nimcevic - FitAI
// June 24th 2016
//Sending analogRead from NodeMCU to ThingSpeak
#include <ESP8266WiFi.h>

const char* server = "api.thingspeak.com";
String apiKey ="83OGBHPSFOB2WCQO";    //This is from ThingSpeak
const char* MY_SSID = "nahnahnahnah"; //Your SSID
const char* MY_PWD = "cheese11";      //Your Password
const int analog_input = A0;            //The only Analog Input on the NodeMCU
float readings;                 //This is where anlogRead is stored

void setup() {
  Serial.begin(115200);
  pinMode(analog_input,INPUT);
  connectWifi();
}

void loop() {
  readings=analogRead(analog_input);
  sendReadings(readings);
  Serial.print("Readings: ");
  Serial.print(readings);
  Serial.println(" sent!");
  delay(2000);
}

void connectWifi()
{
  Serial.print("Connecting to "+*MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");  
}//end connect

void sendReadings(float temp)
{  
   WiFiClient client;
  
   if (client.connect(server, 80)) { // use ip 184.106.153.149 or api.thingspeak.com
   Serial.println("WiFi Client connected ");
   
   String postStr = apiKey;
   postStr += "&field1=";
   postStr += String(temp);
   postStr += "\r\n\r\n";
   
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(postStr.length());
   client.print("\n\n");
   client.print(postStr);
   delay(1000);
   
   }//end if
  // sent++;
 client.stop();
}//end send
