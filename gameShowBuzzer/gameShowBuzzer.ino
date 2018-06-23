/*
 * File:  gameShowBuzzer.ino
 * Name:  Minh Vu
 * Desc.: Game Show Buzzer System
 * Date:  05/26/18
 * Notes: Current sketch is a test to detect button presses and display reading of all buttons.
 */

const byte numBtns = 5;
const byte btnPins[numBtns] = {2, 4, 7, 8, 12};
byte btnVals[numBtns];

void setup() {
  Serial.begin(9600);
  for(int s = 0; s < numBtns; s++)
    pinMode(btnPins[s], INPUT);
}

void loop() {
  for(int s = 0; s < numBtns; s++)
  {
    btnVals[s] = digitalRead(btnPins[s]);
    Serial.print(btnVals[s]);
    Serial.print(' ');
  }
  Serial.println();
  
  delay(10);
}
