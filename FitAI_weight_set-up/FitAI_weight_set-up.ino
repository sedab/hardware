//Fit AI - program for setting up the weight and displaying it on the LCD

#include <LiquidCrystal.h>
#include <math.h>
#include <SoftwareSerial.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //setting up the pins to control the LCD
SoftwareSerial mySerial(6,7); // RX=6 |  TX=7 
const int UP = 8;//up and down inputs
const int DOWN = 9; 

const int Xaxis= A3;
const int Yaxis= A4;
const int Zaxis= A5;
int totalAcceleration;
int xAcc,yAcc,zAcc;

int UpState = LOW;// need to remember last state in order to debounce
int DownState = LOW; 
long lastDebounceTimeUp = 0; //parameters for debouncing 
long lastDebounceTimeDown = 0;
long debounceDelay = 50; 

int weight = 100; //starting weight

void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);

  mySerial.begin(9600);
  delay(10);

  lcd.begin(16, 2);
  lcd.print("FitAI weight:"); //display in the first column
  lcd.setCursor(12, 1);
  lcd.print("lb");

}

void loop() {

  //screen display
  lcd.setCursor(7, 1);
  int UpRead = digitalRead(UP);
  int DownRead = digitalRead(DOWN);

  if (UpRead != UpState) {//debouncing UP button, and increasing weight by 5 of the UP button is pressed

    if (((millis() - lastDebounceTimeDown) > debounceDelay) && (UpRead == HIGH)) {
      //Serial.println("Up by 5");
      weight = weight + 5;
    }
    UpState = UpRead;
    lastDebounceTimeUp = millis();
  }
  if (DownRead != DownState) {//debouncing UP button, and decreasing weight by 5 of the DOWN button is pressed

    if (((millis() - lastDebounceTimeDown) > debounceDelay) && (DownRead == HIGH)) {
      //Serial.println("Up by 5");
      weight = weight - 5;
      if (weight < 100) { //clearing the 3rd digit if its only supposed to display 2 digits
        lcd.setCursor(9, 1);
        lcd.print(" ");
        lcd.setCursor(7, 1);
      }
    }
    DownState = DownRead; //setting new state
    lastDebounceTimeDown = millis(); //refreshing the debaunce reference
  }
  lcd.print(weight);

  //accelerometer
  xAcc=analogRead(Xaxis);
  yAcc=analogRead(Yaxis);
  zAcc=analogRead(Zaxis);
  totalAcceleration=sqrt(square(xAcc)+square(yAcc)+square(zAcc));
  mySerial.println(totalAcceleration);
  delay(100);
  
}
