/*
 * File:  gameShowBuzzer.ino
 * Name:  Minh Vu
 * Desc.: Game Show Buzzer System
 * Date:  07/14/18
 * Notes: Current sketch is a test for the lock-out functionality.
 */

// Buttons 0-4 (1 reset and 4 player buttons)
const byte numBtns = 5;
const byte btnPins[numBtns] = {2, 4, 7, 8, 12};
byte btnVals[numBtns];
byte prevRstBtnVal;

// List of buttons currently pressed (max size = numBtns, current size = numBtnsPressed)
byte btnsPressed[numBtns];
byte numBtnsPressed = 0;

// LEDs (four - one for each player) and buzzer pins
const byte ledPins[numBtns - 1] = {3, 9, 10, 11};
const byte buzzerPin = 5;
const short buzzerFreq = 698; // Frequency for note F5
const unsigned long blinkTime = 750;
unsigned long blinkTimeStamp = 0;
byte currLEDState = LOW;

// State machine values
byte selectedBtn;
enum {LISTEN, P1, P2, P3, P4} state = LISTEN;

// Minimum delay between using button values
const byte debounceTime = 25;
unsigned long prevDebounce = 0; // Timestamp
bool blinkEn = false;
void setup()
{
  Serial.begin(9600);
  for(int s = 0; s < numBtns; s++)
  {
    pinMode(btnPins[s], INPUT);
    
    if(s < numBtns - 1)
      pinMode(ledPins[s], OUTPUT);
  }

  pinMode(buzzerPin, OUTPUT);

  // Keep button 0 (reset) pressed during setup to enable LED blinking
  if(digitalRead(btnPins[0]) == HIGH)
    blinkEn = true;
}

void loop()
{
  // State machine
  switch(state)
  {
    case LISTEN:
      readPlayerBtns();
      break;
      
    default:
      // Blink selected player's LED
      if((millis() - blinkTimeStamp) >= blinkTime && blinkEn)
      {
        if(currLEDState == HIGH)
          currLEDState = LOW;
        else
          currLEDState = HIGH;

        blinkTimeStamp = millis();
      }

      if(selectedBtn >= P1 && selectedBtn <= numBtns - 1)
        digitalWrite(ledPins[selectedBtn - 1], currLEDState);
      
      readResetBtn();
      break;
  }

  //Serial.println(state);
  delay(10);
}

void readPlayerBtns()
{
  // Sample player button values (buttons 1-4)
  for(int s = 1; s < numBtns; s++)
    btnVals[s] = digitalRead(btnPins[s]);

  // Use all button values periodically, which is determined by debounceTime
  if((millis() - prevDebounce) >= debounceTime)
  {
    for(int s = 1; s < numBtns; s++)
    {
      if(btnVals[s] == HIGH)
      {
        btnsPressed[numBtnsPressed] = s;
        numBtnsPressed++;
      }
    }

    if(numBtnsPressed > 0)
    {
      if(numBtnsPressed >= 2)
      {
        // Pick random button out of those currently pressed
        byte tempIndex = random(numBtnsPressed);
        selectedBtn = btnsPressed[tempIndex];
      }
      else // numBtnsPressed == 1
      {
        selectedBtn = btnsPressed[0];
      }

      // Button number will match its respective state value
      state = selectedBtn;

      // Sound buzzer for 1 second
      tone(buzzerPin, buzzerFreq, 1000);

      // Illuminate corresponding LED
      currLEDState = HIGH;
      if(blinkEn)
        blinkTimeStamp = millis();
    }

    numBtnsPressed = 0;
    prevDebounce = millis();
  }
}

void readResetBtn()
{
  // Sample and use button value periodically, which is determined by debounceTime
  if((millis() - prevDebounce) >= debounceTime)
  {
    // Save previous reset button value and sample current value
    prevRstBtnVal = btnVals[0];
    btnVals[0] = digitalRead(btnPins[0]);

    // Reset state if button is pressed, then released
    if(prevRstBtnVal == HIGH && btnVals[0] == LOW)
    {
      state = LISTEN;

      // Turn off LED
      currLEDState = LOW;
      digitalWrite(ledPins[selectedBtn - 1], currLEDState);
    }
    
    prevDebounce = millis();
  }
}

