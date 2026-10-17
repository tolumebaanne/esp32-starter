#include <Arduino.h>

// 03 - The button and the LED.
// Wiring: keep the LED from 02 where it is.
// The button sits across the groove. Use the two legs on the left of the groove.
// A wire from pin 14 to the row of one of them, and a wire from the row of the
// other one to the GND line.
//
// loop, at the bottom, runs one of two jobs.
// To try the other job: put // in front of holdToLight(); and take the //
// off threeBlinks();. Then save and upload.

const int LED_PIN = 13;
const int BUTTON_PIN = 14;

bool wasPressed = false;   // was the button down last time round?

// The LED is on only while you hold the button down.
void holdToLight() {
  bool pressed = digitalRead(BUTTON_PIN) == LOW;   // pressed reads LOW

  if (pressed) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  Serial.println(pressed ? "pressed" : "not pressed");
}

// One step further: one press makes the LED blink three times.
void threeBlinks() {
  bool pressed = digitalRead(BUTTON_PIN) == LOW;

  if (pressed && !wasPressed) {          // only the moment the button goes down
    Serial.println("pressed: three blinks");
    for (int i = 0; i < 3; i = i + 1) {  // do this three times
      digitalWrite(LED_PIN, HIGH);
      delay(200);
      digitalWrite(LED_PIN, LOW);
      delay(200);
    }
  }

  wasPressed = pressed;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);   // turns on the board's own resistor for the button
  Serial.begin(115200);
}

void loop() {
  holdToLight();
  // threeBlinks();
  delay(50);
}
