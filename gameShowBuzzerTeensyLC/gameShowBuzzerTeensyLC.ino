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
const byte btnPins[numBtns] = {2, 5, 7, 8, 10};

// State value modified in ISRs
// 0 = listen for button,
// 1 = P1 button pressed, ... , 4 = P4 button pressed
const byte LISTEN = 0;
const byte P1 = 1;
const byte P2 = 2;
const byte P3 = 3;
const byte P4 = 4;
volatile byte currState = LISTEN;
volatile unsigned long cooldownTimestamp = 0; // in ms
const unsigned long btnCooldown = 500; // in ms, cooldown after button press before being able to reset

// LED (4 - one for each player) and buzzer pins
const byte numLeds = numBtns - 1;
const byte ledPins[numLeds] = {3, 4, 6, 9};
const byte ledPWMVal = 128;
const byte buzzerPin = 16;
const short buzzerFreqs[numBtns - 1] = {784, 989, 1175, 1397}; // Frequencies for piezo, dominant 7th arpegio starting on G5

void isrRST() {
  cli();
  unsigned long currTime = millis();
  if (currState != LISTEN && currTime - cooldownTimestamp >= btnCooldown)
  {
    analogWrite(ledPins[currState - 1], 0);
    currState = LISTEN;
    Serial.println("RST");
  }
  sei();
}

void isrP1() {
  cli();
  if (currState == LISTEN)
  {
    currState = P1;
    Serial.println("P1");
    analogWrite(ledPins[currState - 1], ledPWMVal);
    cooldownTimestamp = millis();
    tone(buzzerPin, buzzerFreqs[currState - 1], btnCooldown);
  }
  sei();
}

void isrP2() {
  cli();
  if (currState == LISTEN)
  {
    currState = P2;
    Serial.println("P2");
    analogWrite(ledPins[currState - 1], ledPWMVal);
    cooldownTimestamp = millis();
    tone(buzzerPin, buzzerFreqs[currState - 1], btnCooldown);
  }
  sei();
}

void isrP3() {
  cli();
  if (currState == LISTEN)
  {
    currState = P3;
    Serial.println("P3");
    analogWrite(ledPins[currState - 1], ledPWMVal);
    cooldownTimestamp = millis();
    tone(buzzerPin, buzzerFreqs[currState - 1], btnCooldown);
  }
  sei();
}

void isrP4() {
  cli();
  if (currState == LISTEN)
  {
    currState = P4;
    Serial.println("P4");
    analogWrite(ledPins[currState - 1], ledPWMVal);
    cooldownTimestamp = millis();
    tone(buzzerPin, buzzerFreqs[currState - 1], btnCooldown);
  }
  sei();
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
  pinMode(buzzerPin, OUTPUT);

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
  cli();
  Serial.println(currState);
  sei();
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
