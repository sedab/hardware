#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

//#include <dummy.h>

#include <ESP8266WiFi.h>
#include <IPaddress.h> 

const char* ssid = "nahnahnahnah"; //input your ssid
const char* password = "cheese11"; //input your password for that ssid

int ledPin = 13;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay (500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
//  Serial.println(WiFi.WiFiClient.remoteIP());
 
}


void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  String request= client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
    if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("Led pin is now:");

  if (value == HIGH) {
    client.println("On");
  }
  else {
    client.println("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button> Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button> Turn Off </button></a><br/>");
  client.println("</html>");

  delay(1);
  Serial.println("Client Disconnected");
  Serial.println("");

}
