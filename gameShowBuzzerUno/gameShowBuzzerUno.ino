/**
 * File:  gameShowBuzzerUno.ino
 * Name:  Minh Vu
 * Desc.: Game Show Buzzer System
 * Date:  07/06/19
 * Notes: This sketch is written for Arduino Uno.
 *        Sketch code last updated 07/14/18
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
const byte numLeds = numBtns - 1;
const byte ledPins[numLeds] = {3, 9, 10, 11};
const byte buzzerPin = 5;
const short buzzerFreqs[numBtns - 1] = {349, 440, 523, 698}; // Frequencies for piezo
bool buzzerIsOn = false;
const unsigned long blinkTime = 750;
unsigned long blinkTimeStamp = 0;
const unsigned long toggleTime = 3000;
unsigned long toggleTimeStamp = 0;
byte currLEDState = LOW;

// State machine values
byte selectedBtn;
enum {LISTEN, P1, P2, P3, P4, TOGGLE} state = LISTEN;

// Minimum delay between using button values
const unsigned long debounceTime = 25;
unsigned long prevDebounce = 0; // Timestamp
bool blinkEn = true;
bool buzzerIsPiezo = false; // True if buzzer is piezo type (settable-tone), false if single-tone

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

  // Keep button 0 (reset) pressed during setup to indicate piezo buzzer instead of single-tone
  if(digitalRead(btnPins[0]) == HIGH)
    buzzerIsPiezo = true;
}

void loop()
{
  // State machine
  switch(state)
  {
    case LISTEN:
      readPlayerBtns();
      break;

    case TOGGLE:
      readToggleBtn();
      break;

    default:
      // Blink selected player's LED
      // Also disable buzzer if it is signle-tone and is on
      if((millis() - blinkTimeStamp) >= blinkTime && blinkEn)
      {
        currLEDState = !currLEDState;

        if(buzzerIsOn)
          handleBuzzer(false);

        blinkTimeStamp = millis();
      }

      digitalWrite(ledPins[selectedBtn - 1], currLEDState);
      
      readResetBtn();
      break;
  }

  // Debugging
  //Serial.println(state);
}

void readPlayerBtns()
{
  // Sample reset button for toggling LED blink
  // Save previous reset button value and sample current value
  prevRstBtnVal = btnVals[0];
  btnVals[0] = digitalRead(btnPins[0]);

  // Sample player button values (buttons 1-4)
  for(int s = 1; s < numBtns; s++)
    btnVals[s] = digitalRead(btnPins[s]);

  // Use all button values periodically, which is determined by debounceTime
  if((millis() - prevDebounce) >= debounceTime)
  {
    // Player button handling overridden if reset button is being pressed
    if(btnVals[0] == HIGH)
    {
      toggleTimeStamp = millis();
      state = TOGGLE;
      return;
    }

    // Handle player buttons
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

      // Sound buzzer
      handleBuzzer(true);

      // Illuminate corresponding LED
      currLEDState = HIGH;
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

      // Turn off buzzer
      handleBuzzer(false);
    }

    prevDebounce = millis();
  }
}

void readToggleBtn()
{
  // Sample and use button value periodically, which is determined by debounceTime
  if((millis() - prevDebounce) >= debounceTime)
  {
    // Save previous reset button value and sample current value
    prevRstBtnVal = btnVals[0];
    btnVals[0] = digitalRead(btnPins[0]);

    if(btnVals[0] == LOW)
    {
      state = LISTEN;
      return;
    }

    // Toggle LED blinking if reset button is held for at least 3 seconds
    if((millis() - toggleTimeStamp) >= toggleTime)
    {
      blinkEn = !blinkEn;

      // User feedback - flash all LEDs (250 ms if blinking enabled, 1 s if not)
      // No need to do this non-blocking since nothing else should happen during this
      short flashTime = (blinkEn) ? 250 : 1000;
      for(int l = 0; l < numLeds; l++)
        digitalWrite(ledPins[l], HIGH);
      delay(flashTime);
      for(int l = 0; l < numLeds; l++)
        digitalWrite(ledPins[l], LOW);

      state = LISTEN;
    }
  }
}

void handleBuzzer(bool buzzerOn)
{
  if(buzzerOn)
  {
    // Sound buzzer for 750 milliseconds, same duration as an LED blink
    if(buzzerIsPiezo)
      tone(buzzerPin, buzzerFreqs[selectedBtn - 1], blinkTime);
    else
      digitalWrite(buzzerPin, HIGH);
    buzzerIsOn = true;
  }
  else
  {
    if(buzzerIsPiezo)
      noTone(buzzerPin);
    else
      digitalWrite(buzzerPin, LOW);
    buzzerIsOn = false;
  }
}
