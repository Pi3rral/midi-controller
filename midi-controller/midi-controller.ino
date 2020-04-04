#include <Arduino.h>
#include "OLED.h"
#include "MIDI.h"

#define BUTTON1_PIN 2
#define BUTTON2_PIN 3

OLED oled;
int button1State;
int button2State;

void oled_init() {
    oled.init();
    oled.clearDisplay();
    oled.print("Ready");
}

void setup() {
    oled_init();
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);
    Serial.begin(9600);
    button1State = HIGH;
    button2State = HIGH;
}

void loop() {
    if (digitalRead(BUTTON1_PIN) == LOW) {
        if (button1State == HIGH) {
            oled.clearDisplay();
            oled.print("Button1 Pressed");
            button1State = LOW;
        }
    } else {
        if (button1State == LOW) {
            oled.clearDisplay();
            oled.print("Button1 NOT Pressed");
            button1State = HIGH;
        }
    }
//    delay(100);
}
