#include <Arduino.h>

// 06 - Everything together.
// Each press of the button moves to the next colour and beeps.
// Wiring: keep the RGB LED from 05, the button from 03 and the buzzer from 04.
//
// Input:    the button
// State:    which colour it is on
// Output:   the colour
// Feedback: the beep, so you know it heard you

const int RED_PIN = 13;
const int GREEN_PIN = 15;
const int BLUE_PIN = 33;
const int BUTTON_PIN = 14;
const int BUZZER_PIN = 32;

const bool COMMON_ANODE = false;   // change to true if you changed it in 05

int colour = 0;            // 0 off, 1 red, 2 green, 3 blue
bool wasPressed = false;   // was the button down last time round?

void setColour(bool red, bool green, bool blue) {
  digitalWrite(RED_PIN,   red   != COMMON_ANODE);
  digitalWrite(GREEN_PIN, green != COMMON_ANODE);
  digitalWrite(BLUE_PIN,  blue  != COMMON_ANODE);
}

void show() {
  if (colour == 1) {
    setColour(true, false, false);
  } else if (colour == 2) {
    setColour(false, true, false);
  } else if (colour == 3) {
    setColour(false, false, true);
  } else {
    setColour(false, false, false);
  }
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  show();
}

void loop() {
  bool pressed = digitalRead(BUTTON_PIN) == LOW;

  if (pressed && !wasPressed) {        // only the moment it goes down
    colour = colour + 1;
    if (colour > 3) {
      colour = 0;
    }
    show();
    tone(BUZZER_PIN, 880, 80);         // a short beep
    Serial.print("colour is now ");
    Serial.println(colour);
  }

  wasPressed = pressed;
  delay(20);
}
