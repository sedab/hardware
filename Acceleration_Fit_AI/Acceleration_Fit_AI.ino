#include <math.h>
//#include <Average.h>

const int GND = A5;
const int Zaxis = A2;
const int Yaxis = A3;
const int Xaxis = A4;
const int VCC = A1;

int totalAcceleration;
int xAcc, yAcc, zAcc;
int rest;

void setup() {
  Serial.begin(9600);
  Serial.println("Hi!");

  pinMode(GND,OUTPUT);
  pinMode(VCC,OUTPUT);
  digitalWrite(GND,LOW);
  digitalWrite(VCC,HIGH);

  
  pinMode(Xaxis, INPUT);
  pinMode(Yaxis, INPUT);
  pinMode(Zaxis, INPUT);


//  rest = sqrt(square(xAcc) + square(yAcc) + square(zAcc));
  /*
    for (int i = 0; i < 99; i++) {
    xAcc = analogRead(Xaxis);
    yAcc = analogRead(Yaxis);
    zAcc = analogRead(Zaxis);
    totalAcceleration = sqrt(square(xAcc) + square(yAcc) + square(zAcc));
    rest[i] = totalAcceleration;
    delay(10);
    }
    for(int k=0;k<99;k++){
    total=total+rest[k];
    Serial.println(total);
    }
    total=(total/100);
    Serial.println("This is the average value");
    Serial.println(total);
  */
}

void loop() {
  //Serial.println(rest);

  xAcc = analogRead(Xaxis);
  yAcc = analogRead(Yaxis);
  zAcc = analogRead(Zaxis);

  delay(10);
  Serial.print(xAcc);
  Serial.print("\t");
  Serial.print(yAcc);
  Serial.print("\t");
  Serial.print(zAcc);
  Serial.print("\t");
  Serial.println(" ");


  delay(700);


}
