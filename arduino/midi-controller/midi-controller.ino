#include <Arduino.h>
#include "OLED.h"
#include "MIDIController.h"

// FS Configuration
#define FS_TIP_PIN  2
#define FS_RING_PIN 3

#define NO_BUTTON     0
#define BUTTON_UP     1
#define BUTTON_DOWN   2
#define BUTTON_MODE   3

#define BLINK_SCREEN  0
#define BLINK_START   9
#define BLINK_END    10

// Global Variables
OLED oled;
MIDIController midi;
uint8_t button_1_state = HIGH;
uint8_t button_2_state = HIGH;
uint8_t current_program = 1;
uint8_t next_program = 1;
bool blink_screen = false;
unsigned int blink_count = 0;

uint8_t read_button_fs3x() {
    if (digitalRead(FS_TIP_PIN) == LOW) {
        if (digitalRead(FS_RING_PIN) == LOW) {
            return BUTTON_UP;
        }
        return BUTTON_MODE;
    } else if (digitalRead(FS_RING_PIN) == LOW) {
        return BUTTON_DOWN;
    }
    return NO_BUTTON;
}

void setup() {
    oled.init();
    Serial.begin(MIDI_BAUD_RATE);
    midi.init(&Serial);
    pinMode(FS_TIP_PIN, INPUT_PULLUP);
    pinMode(FS_RING_PIN, INPUT_PULLUP);
    oled.clearDisplay();
    oled.printProgramChange(current_program);
}

void loop() {
    uint8_t button_pressed = read_button_fs3x();
    switch(button_pressed) {
        case BUTTON_UP: {
            oled.clearDisplay();
            oled.printCurrent(current_program);
            ++next_program;
            if (next_program == 100) {
              next_program = 1;
            }
            oled.printProgramChange(next_program);
            blink_screen = true;
            delay(300);
            break;
        }
        case BUTTON_DOWN: {
            oled.clearDisplay();
            oled.printCurrent(current_program);
            --next_program;
            if (next_program == 0) {
              next_program = 99;
            }
            oled.printProgramChange(next_program);
            blink_screen = true;
            delay(300);
            break;
        }
        case BUTTON_MODE: {
            oled.clearDisplay();
            current_program = next_program;
            oled.printProgramChange(current_program);
            midi.sendProgramChange(current_program);
            blink_screen = false;
            delay(300);
            break;
        }
        default:
            delay(100);
            break;
    }
    if (BLINK_SCREEN && blink_screen) {
        ++blink_count;
        if (blink_count > BLINK_END) {
            blink_count = 0;
            oled.clearDisplay();
            oled.printCurrent(current_program);
            oled.printProgramChange(next_program);
        } else if (blink_count == BLINK_START) {
            oled.clearDisplay();
            oled.printCurrent(next_program);
            oled.printProgramChange(current_program);
        }
    }
}
