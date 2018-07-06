/*
 * File:  gameShowBuzzer.ino
 * Name:  Minh Vu
 * Desc.: Game Show Buzzer System
 * Date:  07/05/18
 * Notes: Current sketch is a test to sample debounced button presses.
 *        The number of buttons pressed and corresponding button numbers
 *        are printed to the serial port.
 */

// Buttons 0-4 (1 reset and 4 player buttons)
const byte numBtns = 5;
const byte btnPins[numBtns] = {2, 4, 7, 8, 12};
byte btnVals[numBtns];

// List of buttons currently pressed (max size = numBtns, current size = numBtnsPressed)
byte btnsPressed[numBtns];
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
      {
        btnsPressed[numBtnsPressed] = s;
        numBtnsPressed++;
      }
    }

    Serial.print(numBtnsPressed);
    Serial.print('\t');
    for(int i = 0; i < numBtnsPressed; i++)
    {
      Serial.print(btnsPressed[i]);
      Serial.print(' ');
    }
    Serial.println();
    
    numBtnsPressed = 0;
    prevDebounce = millis();
  }
}

