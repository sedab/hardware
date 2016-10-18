/*
 * This code is made for testing accelerometer
 */

#include <math.h>
//#include <Average.h>

//*** Accelerometer Pins***
const int GND = A5;
const int Zaxis = A2;
const int Yaxis = A3;
const int Xaxis = A4;
const int VCC = A1;


int xAcc, yAcc, zAcc;


void setup() {
  //start serial
  Serial.begin(9600);
  while(!Serial){
    //wait for the serial connection
  }
  Serial.println("Hi!");

  //Configure the accelerometer pins
  pinMode(GND,OUTPUT);
  pinMode(VCC,OUTPUT);
  digitalWrite(GND,LOW);
  digitalWrite(VCC,HIGH);
  pinMode(Xaxis, INPUT);
  pinMode(Yaxis, INPUT);
  pinMode(Zaxis, INPUT);
}

void loop() {

  //read the values from the accelerometer
  xAcc = analogRead(Xaxis);
  yAcc = analogRead(Yaxis);
  zAcc = analogRead(Zaxis);

  //display all the values
  delay(10);
  Serial.print("X: ");
  Serial.print(xAcc);
  Serial.print("\t");
  Serial.print("Y: ");
  Serial.print(yAcc);
  Serial.print("\t");
  Serial.print("Z: ");
  Serial.print(zAcc);
  Serial.print("\t");
  Serial.println(" ");

  delay(500);
}
