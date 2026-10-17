#include <Arduino.h>

// 05 - The RGB LED.
// Take the plain LED and the resistor out. The resistor goes on the RGB LED now.
// Wiring: the resistor from the longest leg's row to the GND line.
//         Wires from pin 13 to the red leg, pin 15 to green, pin 33 to blue.
// If nothing lights, your RGB LED is the other kind. Take the resistor's end
// out of the GND line, put it in an empty row, and wire that row to a 3V3 pin.
// Then change false to true on the COMMON_ANODE line below.
//
// loop, at the bottom, runs one of two jobs.
// To try the other job: put // in front of cycleColours(); and take the //
// off trafficLight();. Then save and upload.

const int RED_PIN = 13;
const int GREEN_PIN = 15;
const int BLUE_PIN = 33;

const bool COMMON_ANODE = false;

void setColour(bool red, bool green, bool blue) {
  // the other kind of RGB LED lights up when the pin is off, so this swaps them
  digitalWrite(RED_PIN,   red   != COMMON_ANODE);
  digitalWrite(GREEN_PIN, green != COMMON_ANODE);
  digitalWrite(BLUE_PIN,  blue  != COMMON_ANODE);
}

// Red, then green, then blue, then off.
void cycleColours() {
  setColour(true, false, false);   // red
  delay(1000);
  setColour(false, true, false);   // green
  delay(1000);
  setColour(false, false, true);   // blue
  delay(1000);
  setColour(false, false, false);  // off
  delay(1000);
}

// One step further: a traffic light. Green, then yellow, then red.
// Yellow is red and green switched on at the same time.
void trafficLight() {
  setColour(false, true, false);   // green
  delay(3000);
  setColour(true, true, false);    // yellow: red and green together
  delay(1000);
  setColour(true, false, false);   // red
  delay(3000);
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  cycleColours();
  // trafficLight();
}
