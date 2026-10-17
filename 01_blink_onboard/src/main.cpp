#include <Arduino.h>

// 01 - Blink the light that is already on the board.
// Nothing to wire. The tiny light beside IO2 on the board is on pin 2.

const int LIGHT_PIN = 2;   // the light on the board

void setup() {
  pinMode(LIGHT_PIN, OUTPUT);   // pin 2 will send power out
}

void loop() {
  digitalWrite(LIGHT_PIN, HIGH);   // light on
  delay(500);                      // wait half a second
  digitalWrite(LIGHT_PIN, LOW);    // light off
  delay(500);                      // wait half a second
}
