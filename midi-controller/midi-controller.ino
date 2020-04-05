#include <Arduino.h>
#include "OLED.h"
#include "MIDI.h"

#define BUTTON1_PIN 2
#define BUTTON2_PIN 3

#define NO_BUTTON     0
#define BUTTON_UP     1
#define BUTTON_DOWN   2
#define BUTTON_MODE   3


#define MIDI_CHANNEL 1


OLED oled;
uint8_t button_1_state = HIGH;
uint8_t button_2_state = HIGH;
uint8_t current_program = 1;
uint8_t next_program = 1;

uint8_t read_button_fs3x() {
    if (digitalRead(BUTTON1_PIN) == LOW) {
        if (digitalRead(BUTTON2_PIN) == LOW) {
            return BUTTON_UP;
        }
        return BUTTON_DOWN;
    } else if (digitalRead(BUTTON2_PIN) == LOW) {
        return BUTTON_MODE;
    }
    return NO_BUTTON;
}


void setup() {
    oled.init();
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);
    Serial.begin(9600);
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
            delay(300);
            break;
        }
        case BUTTON_MODE: {
            oled.clearDisplay();
            current_program = next_program;
            oled.printProgramChange(current_program);
            sendMIDI(&Serial, PROGRAM_CHANGE, MIDI_CHANNEL, current_program);
            delay(300);
            break;
        }
        default:
            delay(100);
            break;
    }
    if (digitalRead(BUTTON1_PIN) == LOW) {
        if (button_1_state == HIGH) {
            button_1_state = LOW;
            
        }
    } else if (button_1_state == LOW) {
        button_1_state = HIGH;
    }
}
