#include <Arduino.h>

// 02 - Blink your own LED.
// Wiring: a wire from pin 13 to the row with the LED's long leg.
// The resistor from the row with the short leg to the GND line.
// A wire from a GND pin to the GND line.

const int LED_PIN = 13;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);   // LED on
  delay(500);
  digitalWrite(LED_PIN, LOW);    // LED off
  delay(500);
}
