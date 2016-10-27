#include <SPI.h>
#include <WiFi101.h>
#include <String.h>

#define SECOND 1000
#define mynet 192.168.1.1

char ssid[] = "Fit A.I";

String my_ssid = "";
String my_pass = "";
String test_string = "00000000";
bool incoming_ssid = false;
bool incoming_pass = false;
bool AP_Mode = true;
bool Station_Mode = false;

int status = WL_IDLE_STATUS;
WiFiServer fitai_server(80);


void setup() {

  //Communication with PC
  Serial.begin(9600);
  while (!Serial) {
    //wait to connect
  }
  Serial.print("Creating acces point \"");
  Serial.print(ssid);
  Serial.println("\"");

  //Starting the server
  status = WiFi.beginAP(ssid);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating AP Fit A.I failed");
    while (1);
  }

  delay (3 * SECOND);

  fitai_server.begin();

  printWifiStatus();

}

void loop() {
  while (AP_Mode)
  {
    // compare the previous status to the current status
    if (status != WiFi.status()) {
      // it has changed update the variable
      status = WiFi.status();

      if (status == WL_AP_CONNECTED) {
        // a device has connected to the AP
        Serial.println("Device connected to AP");
      } else {
        // a device has disconnected from the AP, and we are back in listening mode
        Serial.println("Device disconnected from AP");
      }
    }

    WiFiClient client = fitai_server.available();

    if (client) {
      //Serial.println("\nnew client\n");
      String currentLine = "";
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          scan(c);
          Serial.write(c);
          if (c == '\n') {
            if (currentLine.length() == 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");

              client.println();
              client.print("Hello, welcome to Fit A.I<br>");
              client.print("<br>");
              client.print("<form>");
              client.print("SSID:<br>");
              client.print("<input type=\"text\" name=\"my_ssid\"><br>");
              client.print("Password:<br>");
              client.print("<input type=\"text\" name=\"my_pass\"><br><br>");
              client.print("<input type=\"submit\" value=\"Submit\">");
              client.print("</form>");
              client.println();
              break;
            }
            else {
              currentLine = "";
            }
          }
          else if (c != '\r') {
            currentLine += c;
          }
        }
      }
      client.stop();
      Serial.print("This is your SSID: ");
      Serial.println(my_ssid);
      Serial.print("This is the length: ");
      Serial.println(my_ssid.length());
      Serial.print("This is your password: ");
      Serial.println(my_pass);
      Serial.print("This is the length: ");
      Serial.println(my_pass.length());
      Serial.println("\n\n");
      if (my_ssid.length() > 0 && my_pass.length() > 0) {
        AP_Mode = false;
        Station_Mode = true;
      }
    }
  }
  while (Station_Mode) {
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present");
      // don't continue:
      while (true);
    }

    WiFi.disconnect();
    status = WL_IDLE_STATUS;

    Serial.print("Connecting to: ");
    Serial.println(my_ssid);
    Serial.print("with the passowrd: ");
    Serial.println(my_pass);
    while (status != WL_CONNECTED) {
      status = WiFi.begin(my_ssid);
      delay(10000);
      Serial.print(". ");
    }
    if (WL_CONNECTED) {
      Serial.print("connected to: ");
      Serial.println(my_ssid);
    }
    else {
      Serial.print("failed to connect to: ");
      Serial.println(my_ssid);
    }
    Station_Mode = false;
  }
  AP_Mode = false;

}


void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP addres: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Signal strength: ");
  Serial.println(rssi);
}

void scan(char c) {
  for (int i = 0; i < 7; i++) {
    test_string[i] = test_string[i + 1];
    //Serial.print(i);
  }
  test_string[7] = c;
  if (c == '&' || c == 'H') {
    incoming_ssid = false;
    incoming_pass = false;
  }
  if (incoming_ssid) {
    if (c == '+') {
      c = ' ';
    }
    my_ssid += c;
  }
  if (incoming_pass) {
    my_pass += c;
  }

  if (test_string == "my_ssid=")
  {
    incoming_ssid = true;
  }
  if (test_string == "my_pass=") {
    incoming_pass = true;
  }
}






