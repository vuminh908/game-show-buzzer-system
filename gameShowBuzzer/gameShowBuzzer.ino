/*
 * File:  gameShowBuzzer.ino
 * Name:  Minh Vu
 * Desc.: Game Show Buzzer System
 * Date:  06/28/18
 * Notes: Current sketch is a test to sample debounced button presses.
 */

// 1 reset and 4 player buttons
const byte numBtns = 5;
const byte btnPins[numBtns] = {2, 4, 7, 8, 12};
byte btnVals[numBtns];

byte numBtnsPressed = 0;

// Minimum delay between using button values
const byte debounceTime = 25;
long prevDebounce = 0; // Timestamp

void setup()
{
  Serial.begin(9600);
  for(int s = 0; s < numBtns; s++)
    pinMode(btnPins[s], INPUT);
}

void loop()
{
  readBtns();
  delay(10);
}

void readBtns()
{
  // Sample button values
  for(int s = 0; s < numBtns; s++)
    btnVals[s] = digitalRead(btnPins[s]);

  // Use button values periodically, which is determined by debounceTime
  if((millis() - prevDebounce) > debounceTime)
  {
    for(int s = 0; s < numBtns; s++)
    {
      if(btnVals[s] == HIGH)
        numBtnsPressed++;
      Serial.print(btnVals[s]);
      Serial.print(' ');
    }
    Serial.println(numBtnsPressed);
    
    numBtnsPressed = 0;
    prevDebounce = millis();
  }
}

