#include <Arduino.h>
#include "OLED.h"
#include "MIDI.h"

#define BUTTON1_PIN 2
#define BUTTON2_PIN 3

OLED oled;
int button1State = HIGH;
int button2State = HIGH;
int programNumber = 90;

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
}

void loop() {
    if (digitalRead(BUTTON1_PIN) == LOW) {
        if (button1State == HIGH) {
            button1State = LOW;
            oled.clearDisplay();
            oled.printCurrent(programNumber);
            ++programNumber;
            if (programNumber == 100) {
              programNumber = 1;
            }
            oled.printProgramChange(programNumber);
        }
    } else if (button1State == LOW) {
        button1State = HIGH;
    }
    delay(300);
}
