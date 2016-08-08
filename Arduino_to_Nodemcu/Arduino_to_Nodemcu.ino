#include <SoftwareSerial.h>

 SoftwareSerial mySerial(2,3); // RX=2 |  TX=3 
 int number=0; //for testing serail communication purposses

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println("Serial began...");
  
  mySerial.begin(9600);
  delay(10);
  Serial.println("mySerial began...");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(number);
  mySerial.println(number);
  number++;
  if(number==140){
    number=0;
  }
  delay(600);
}
