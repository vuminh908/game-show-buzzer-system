/**
 * File:  gameShowBuzzerTeensyLC.ino
 * Name:  Minh Vu
 * Desc.: Game Show Buzzer System
 * Notes: This sketch is written for Teensy LC.
 *        It may also work for Teensy 3.2 since pinouts are similar.
 * Refs.: https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
**/

// Buttons 0-4 (4 player buttons and 1 reset button)
const byte numBtns = 5;
const byte btnPins[numBtns] = {2, 5, 7, 8, 11};

// State value modified in ISRs
// 0 = listen for button,
// 1 = P1 button pressed, ... , 4 = P4 button pressed
const byte LISTEN = 0;
const byte P1 = 1;
const byte P2 = 2;
const byte P3 = 3;
const byte P4 = 4;
volatile byte currState = LISTEN;

// LED (4 - one for each player) and buzzer pins
const byte numLeds = numBtns - 1;
const byte ledPins[numLeds] = {3, 4, 6, 9};

void isrRST() {
  if (currState != LISTEN)
  {
    digitalWrite(ledPins[currState - 1], LOW);
    currState = LISTEN;
    Serial.println("RST");
  }
}

void isrP1() {
  if (currState == LISTEN)
  {
    currState = P1;
    Serial.println("P1");
    digitalWrite(ledPins[currState - 1], HIGH);
  }
}

void isrP2() {
  if (currState == LISTEN)
  {
    currState = P2;
    Serial.println("P2");
    digitalWrite(ledPins[currState - 1], HIGH);
  }
}

void isrP3() {
  if (currState == LISTEN)
  {
    currState = P3;
    Serial.println("P3");
    digitalWrite(ledPins[currState - 1], HIGH);
  }
}

void isrP4() {
  if (currState == LISTEN)
  {
    currState = P4;
    Serial.println("P4");
    digitalWrite(ledPins[currState - 1], HIGH);
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  for (byte i = 0; i < numBtns; i++)
  {
    pinMode(btnPins[i], INPUT);
  }
  for (byte j = 0; j < numLeds; j++)
  {
    pinMode(ledPins[j], OUTPUT);
  }

  attachInterrupt(btnPins[0], isrP1, RISING);
  attachInterrupt(btnPins[1], isrP2, RISING);
  attachInterrupt(btnPins[2], isrP3, RISING);
  attachInterrupt(btnPins[3], isrP4, RISING);
  attachInterrupt(btnPins[4], isrRST, RISING);
  
  Serial.begin(9600);

}

void loop() {
  // TODO
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println(currState);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
