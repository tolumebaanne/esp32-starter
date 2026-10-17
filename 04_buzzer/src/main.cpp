#include <Arduino.h>

// 04 - Make the buzzer sound, and find out which kind you have.
// Wiring: a wire from pin 32 to the row with the buzzer's longer leg.
// A wire from the row with the other leg to the GND line.
//
// Part 1 switches pin 32 on for one second.
//   A long beep:  you have an ACTIVE buzzer.
//   Only a click: you have a PASSIVE buzzer.
// Part 2 plays three notes. Both kinds make a sound.

const int BUZZER_PIN = 32;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  Serial.println("Part 1: pin on for one second");
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
  delay(1000);

  Serial.println("Part 2: three notes");
  tone(BUZZER_PIN, 523, 200);   // a note, for 200 milliseconds
  delay(300);
  tone(BUZZER_PIN, 659, 200);
  delay(300);
  tone(BUZZER_PIN, 784, 200);
  delay(300);
  noTone(BUZZER_PIN);

  delay(3000);   // wait three seconds, then do it all again
}
