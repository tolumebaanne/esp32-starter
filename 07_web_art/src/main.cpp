#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebSocketsServer.h>
#include "secrets.h"

// 07 - Web art. The button plays a picture on a web page, over WiFi.
// Wiring: the same as 06. RGB LED on 13, 15 and 33, button on 14, buzzer on 32.
//
// What happens:
//   1. The board joins the WiFi named in secrets.h. While it tries, the light blinks blue.
//   2. It opens a live connection that a web page can join, at esp32-art.local, port 81.
//   3. It counts presses of the button: one, two or three, close together.
//   4. It sends the count to the web page, and beeps and blinks once per press.
//   5. The web page sends a colour back, and the RGB LED changes to match the screen.

const int RED_PIN = 13;
const int GREEN_PIN = 15;
const int BLUE_PIN = 33;
const int BUTTON_PIN = 14;
const int BUZZER_PIN = 32;

const bool COMMON_ANODE = false;   // change to true if you changed it in 05

const char* BOARD_NAME = "esp32-art";   // the page finds the board at esp32-art.local
const int PORT = 81;

const unsigned long BOUNCE_MS = 30;    // ignore button wobble shorter than this
const unsigned long GAP_MS = 400;      // presses closer together than this count as one group

WebSocketsServer pageLink(PORT);

bool red = false, green = true, blue = false;   // the colour the page asked for

bool wasPressed = false;
unsigned long lastChange = 0;
unsigned long lastPressAt = 0;
int pressCount = 0;
unsigned long lastAddressAt = 0;

void setColour(bool r, bool g, bool b) {
  // the other kind of RGB LED lights up when the pin is off, so this swaps them
  digitalWrite(RED_PIN,   r != COMMON_ANODE);
  digitalWrite(GREEN_PIN, g != COMMON_ANODE);
  digitalWrite(BLUE_PIN,  b != COMMON_ANODE);
}

void showPageColour() {
  setColour(red, green, blue);
}

// Feedback: one beep and one blink of the light per press.
void answerPresses(int count) {
  for (int i = 0; i < count; i = i + 1) {
    setColour(false, false, false);
    tone(BUZZER_PIN, 880, 70);
    delay(100);
    showPageColour();
    delay(100);
  }
}

// Messages from the web page look like "colour:1,0,1" for red and blue.
void onPageMessage(uint8_t who, WStype_t type, uint8_t* data, size_t length) {
  if (type == WStype_CONNECTED) {
    Serial.println("A web page joined.");
    pageLink.sendTXT(who, "hello");
  } else if (type == WStype_DISCONNECTED) {
    Serial.println("A web page left.");
  } else if (type == WStype_TEXT) {
    String message = "";
    for (size_t i = 0; i < length; i = i + 1) {
      message += (char)data[i];
    }
    if (message.startsWith("colour:") && message.length() >= 12) {
      red = message.charAt(7) == '1';
      green = message.charAt(9) == '1';
      blue = message.charAt(11) == '1';
      showPageColour();
    }
  }
}

void joinWifi() {
  Serial.print("Joining WiFi ");
  Serial.println(WIFI_NAME);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

  bool lightOn = false;
  while (WiFi.status() != WL_CONNECTED) {   // blink blue until it joins
    lightOn = !lightOn;
    setColour(false, false, lightOn);
    delay(250);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Joined. The board's address is ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin(BOARD_NAME)) {
    Serial.print("Or use the name ");
    Serial.print(BOARD_NAME);
    Serial.println(".local");
  }
}

// Counts presses. When the button has been quiet for GAP_MS, the group is done.
void countPresses() {
  bool pressed = digitalRead(BUTTON_PIN) == LOW;
  unsigned long now = millis();

  if (pressed != wasPressed && now - lastChange > BOUNCE_MS) {
    lastChange = now;
    wasPressed = pressed;
    if (pressed) {
      pressCount = pressCount + 1;
      lastPressAt = now;
    }
  }

  if (pressCount > 0 && !pressed && now - lastPressAt > GAP_MS) {
    int count = pressCount;
    if (count > 3) {
      count = 3;
    }
    String message = "press:" + String(count);
    pageLink.broadcastTXT(message);
    Serial.println(message);
    answerPresses(count);
    pressCount = 0;
  }
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);

  joinWifi();
  pageLink.begin();
  pageLink.onEvent(onPageMessage);
  showPageColour();
}

// Says the board's address again every five seconds, so it is easy to catch in the Serial Monitor.
void repeatAddress() {
  if (millis() - lastAddressAt > 5000) {
    lastAddressAt = millis();
    Serial.print("The board's address is ");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  pageLink.loop();
  countPresses();
  repeatAddress();
}
