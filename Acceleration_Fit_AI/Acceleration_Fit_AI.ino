#include <math.h>
//#include <Average.h>

const int Xaxis = A3;
const int Yaxis = A4;
const int Zaxis = A5;
int totalAcceleration;
int xAcc, yAcc, zAcc;
int rest;

void setup() {
  Serial.begin(9600);
  Serial.println("Hi!");

  pinMode(Xaxis, INPUT);
  pinMode(Yaxis, INPUT);
  pinMode(Zaxis, INPUT);
  delay(2000);
  xAcc = analogRead(Xaxis);
  yAcc = analogRead(Yaxis);
  zAcc = analogRead(Zaxis);

  rest = sqrt(square(xAcc) + square(yAcc) + square(zAcc));
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
  totalAcceleration = sqrt(square(xAcc) + square(yAcc) + square(zAcc));
  //Serial.println(3 * (abs(rest - totalAcceleration)));
  delay(10);
  Serial.print(xAcc);
  Serial.print("\t");
  Serial.print(yAcc);
  Serial.print("\t");
  Serial.print(zAcc);
  Serial.print("\t");
  Serial.print(totalAcceleration);
  Serial.println();

  delay(40);


}
