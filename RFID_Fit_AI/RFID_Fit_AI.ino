//#define RFID_RESET_PIN 7  // if we are using repetitive scanning, othervise
                          // pin 2 on the ID-12LA RFID goes to VCC

//Registered ID's
char Tim[13] = "010039944EE2";
char Boris[13] = "010039D21DF7";
char Seda[13] = "7D001CF336A4";

void setup() {
  Serial.begin(9600); //MKR1000 USB Serial communication
  Serial1.begin(9600); //MKR1000 RX & TX Serial communication

//  pinMode(RFID_RESET_PIN, OUTPUT);
//  digitalWrite(RFID_RESET_PIN, HIGH);

}

void loop() {
  scanTag();
}



void scanTag(){
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
  delay(150);
//  resetReader(); //reset the RFID reader
}

void checkTag(char tag[]) {
  ///////////////////////////////////
  //Check the read tag against known tags
  ///////////////////////////////////

  if (strlen(tag) == 0) return; //empty, no need to contunue

  if (compareTag(tag, Tim)) { // if matched tag1, do this
    Serial.println("Hi Tim, please put in your weight.");
    Serial.println();

  }
  else if (compareTag(tag, Boris)) { //if matched tag2, do this
    Serial.println("Hi Boris, we are here to make your workout experience better!");
    Serial.println();
  }
  else if (compareTag(tag, Seda)) { //if matched tag2, do this
    Serial.println("Hi Seda.");
    Serial.println();
  }
  else {
    Serial.print(tag); //read out any unknown tag
    Serial.println (" - This is a new user, please register");
    Serial.println();
  }

}

/*
void resetReader() {
  ///////////////////////////////////
  //Reset the RFID reader to read again.
  ///////////////////////////////////
//  digitalWrite(RFID_RESET_PIN, LOW);
//  digitalWrite(RFID_RESET_PIN, HIGH);
  delay(150);
}
*/

void clearTag(char one[]) {
  ///////////////////////////////////
  //clear the char array by filling with null - ASCII 0
  //Will think same tag has been read otherwise
  ///////////////////////////////////
  for (int i = 0; i < strlen(one); i++) {
    one[i] = 0;
  }
}

boolean compareTag(char one[], char two[]) {
  ///////////////////////////////////
  //compare two value to see if same,
  //strcmp not working 100% so we do this
  ///////////////////////////////////

  if (strlen(one) == 0) return false; //empty

  for (int i = 0; i < 12; i++) {
    if (one[i] != two[i]) return false;
  }

  return true; //no mismatches
}

