# ESP32 Starter

Seven small projects for the Freenove ESP32-WROVER CAM board. They start with blinking the light on the board and end with a button that controls a picture on a web page over WiFi.

Made as teaching material for BDI310 Creative Coding at Seneca Polytechnic.

---

## What you need

**Parts**

- Freenove ESP32-WROVER CAM board
- A breadboard
- An LED
- An RGB LED
- One resistor
- A push button
- A buzzer
- Jumper wires
- A USB-C cable that can carry data, not only charge

**On your laptop**

| What | What it does | Where to get it |
|---|---|---|
| VS Code | Where you open and change the code | [code.visualstudio.com](https://code.visualstudio.com/) |
| PlatformIO IDE | An add-on for VS Code that sends code to the board | In VS Code: click the Extensions icon (four small squares) on the left, search **PlatformIO IDE**, click **Install** |
| Espressif 32 | Teaches PlatformIO how to build code for an ESP32 | In VS Code: click the PlatformIO icon (an alien head) on the left, then **PIO Home**, **Open**, **Platforms**, the **Embedded** tab, search **Espressif 32**, click **Install** |
| CH340 driver | Lets your laptop recognise the board | Windows: [wch-ic.com/downloads/CH341SER_EXE.html](https://www.wch-ic.com/downloads/CH341SER_EXE.html). Mac: most newer Macs already have it. If yours does not, [wch-ic.com/downloads/CH34XSER_MAC_ZIP.html](https://www.wch-ic.com/downloads/CH34XSER_MAC_ZIP.html) |

If VS Code suggests installing **C/C++** by Microsoft, click Install. PlatformIO uses it.

---

## The class handout

`BDI310_Week6_ESP32_Setup_Handout.pdf` in this repo has every step from class, with drawings of each part and each circuit.

## Get the projects

1. On this page, click the green **Code** button, then **Download ZIP**.
2. Unzip it. Inside are seven folders, `01_blink_onboard` to `07_web_art`.

## Put a project on the board

1. In VS Code, click **File**, then **Open Folder**.
2. Choose one numbered folder, such as `01_blink_onboard`. Choose the folder itself, not the folder around it.
3. If VS Code asks whether you trust the authors, click **Yes**.
4. The code is in the `src` folder, in a file called `main.cpp`.
5. Plug the board into your laptop with the USB-C cable.
6. Along the bottom of VS Code is a coloured bar with small icons. Click the **right arrow**. That is Upload.
7. Wait until the text at the bottom says **SUCCESS**. The first upload takes a few minutes. Later ones are quicker.

To see what the board is saying, click the **plug** icon in the same bottom bar. That opens the Serial Monitor.

**Pull the USB cable out every time you move a wire.** The cable is the board's only power, so with it out, nothing on the breadboard is live.

---

## The projects

| Folder | What it does | Wiring |
|---|---|---|
| `01_blink_onboard` | Blinks the tiny light on the board, beside IO2 | Nothing to wire |
| `02_blink_led` | Blinks your own LED | Pin 13 to the LED's long leg. The short leg through the resistor to GND |
| `03_button` | The LED is on while you hold the button | Keep the LED. Button across the middle groove of the breadboard: pin 14 to one of the two legs on the left side, the other left leg to GND |
| `04_buzzer` | Beeps, and tells you which kind of buzzer you have | Pin 32 to the buzzer's longer leg. The other leg to GND |
| `05_rgb` | The RGB LED goes red, green, blue | Take the LED out. Pin 13 to red, pin 15 to green, pin 33 to blue. The longest leg through the resistor to GND |
| `06_all_together` | Each press changes the colour and beeps | The RGB LED, the button and the buzzer, as above |
| `07_web_art` | The button controls a picture on a web page, over WiFi | The same as 06 |

Every project's wiring is also written at the top of its `src/main.cpp`.

**GND line:** run one jumper wire from a GND pin on the board to the line of holes beside the blue stripe on the breadboard. Then every part can reach GND through that line.

**Use only pins 2, 13, 14, 15, 32 and 33.** They stay free even when the board's camera is switched on.

### One step further

`03_button` and `05_rgb` each have a second version in the same file. Scroll to `loop` at the bottom of `main.cpp`. You will see two lines, one starting with `//`. A line starting with `//` is a comment, so the board skips it.

- Put `//` in front of the first line.
- Take the `//` off the second line.
- Save and upload.

Only one of the two lines should be without `//`.

| Folder | First version | Second version |
|---|---|---|
| `03_button` | `holdToLight();` LED on while held | `threeBlinks();` one press, three blinks |
| `05_rgb` | `cycleColours();` red, green, blue | `trafficLight();` green, yellow, red |

### If your RGB LED does not light at all

There are two kinds of RGB LED. If yours stays dark:

1. Pull the cable out.
2. Take the resistor's end out of GND, push it into an empty row, and run a jumper wire from that row to a **3V3** pin.
3. In the code, change `const bool COMMON_ANODE = false;` to `true`. Do this in `05_rgb`, `06_all_together` and `07_web_art`.

---

## 07_web_art: the button and the web page

One press makes a bloom of shapes on the screen. Two presses change all the colours, and the RGB LED changes to match. Three presses sweep the screen clean.

This project has two halves:

- `07_web_art/src` goes on the **board**.
- `07_web_art/web` is the **web page**. It runs on a laptop.

### How it works

1. The board joins a WiFi network and gets an address on it, such as `172.20.10.3`.
2. The board runs a **server**: a program that waits for other programs to connect to it.
3. The web page is the **client**. When you click **Connect**, the page connects to the board and stays connected.
4. When you press the button, the board counts the presses and sends a short message: `press:1`, `press:2` or `press:3`.
5. The page reads the message and draws: a bloom, new colours, or a sweep.
6. After new colours, the page sends a message back, such as `colour:1,0,0`, and the board sets the RGB LED to that colour.

### Set up the WiFi

The board and the laptop **must be on the same WiFi**. Otherwise they cannot find each other.

A phone hotspot works well, because you control it and it is the same in any room. On an **iPhone**:

1. Open **Settings**, then **Personal Hotspot**.
2. Turn on **Allow Others to Join**.
3. Turn on **Maximize Compatibility**. The board can only use the older, slower kind of WiFi (2.4 GHz). Maximize Compatibility makes the hotspot use it. Without it, the board may never connect.

On Android, look for the hotspot setting called **AP band** or **Band** and choose **2.4 GHz**.

### Change the WiFi details

The board joins the WiFi named in `07_web_art/src/secrets.h`:

```cpp
#define WIFI_NAME     "your hotspot name"
#define WIFI_PASSWORD "your hotspot password"
```

Change the two values to your own hotspot, keep the quotes, save, and upload again. The name and password must match exactly, capital letters included.

### Other settings you can change

| Setting | Where | What it does |
|---|---|---|
| `BOARD_NAME = "esp32-art"` | `src/main.cpp` | The name the page uses to find the board: `esp32-art.local`. If two boards run this project on the same WiFi, give each one its own name |
| `PORT = 81` | `src/main.cpp` | The port the board's server waits on. If you change it, also change `:81` in `web/script.js` to match |
| `value="esp32-art.local"` | `web/index.html` | The address already typed into the box on the page. Change it if you changed `BOARD_NAME` |

### Run it

**On the board**

1. Check the wiring matches `06_all_together`.
2. Open the folder `07_web_art` in VS Code and upload it.
3. Open the Serial Monitor (the plug icon). It prints `The board's address is` and a number, every five seconds. Write the number down.
4. Unplug the board from the laptop and plug it into a power bank. The RGB LED blinks blue while the board joins the WiFi. When it stops blinking, the board is ready.

**On the laptop**

1. Join the same WiFi as the board.
2. In VS Code, open the folder `07_web_art/web`.
3. Right-click `index.html` and choose **Open with Live Server**.
4. On the page, click **Connect**. The dot in the top corner turns green.
5. Press the button on the board: once, twice, three times.

You can also press the **1**, **2** and **3** keys on the laptop to try the page without the board.

---

## When it does not work

| What you see | What to check |
|---|---|
| Upload cannot find the board | Try a different USB cable. Then install the CH340 driver |
| The text stops at `Connecting...` | Click the right arrow again and hold the **BOOT** button on the board while it says Connecting |
| No right arrow in the bottom bar | Open the project with **File**, **Open Folder**, choosing the numbered folder itself |
| An LED stays dark | Turn it round. Then check the wire is in the pin the code names |
| Nothing works at all | Check there is a wire from a GND pin to the GND line |
| The LED is always on (03) | The button is not across the groove. Turn it a quarter turn |
| 07: the light keeps blinking blue | The board cannot join the WiFi. Check the name and password in `secrets.h`, and turn on Maximize Compatibility (iPhone) or 2.4 GHz (Android) |
| 07: the dot on the page stays red | Check the laptop is on the same WiFi as the board. Then type the number from the Serial Monitor into the box in place of `esp32-art.local` and click Connect |
| VS Code says Python is not installed (Windows) | Open **Settings**, **Apps**, **Advanced app settings**, **App execution aliases**, and switch off **python.exe** and **python3.exe**. Then restart VS Code |

---

## Reuse

Fork it, change it, use it in your own teaching. No attribution required.
