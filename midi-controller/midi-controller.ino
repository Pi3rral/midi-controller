#include <Arduino.h>
#include "OLED.h"
#include "MIDI.h"
#include "ButtonReaderFS3X.h"
#include "Settings.h"

// FS3X Configuration
#define FS_TIP_PIN  2
#define FS_RING_PIN 3

#define BLINK_START   9
#define BLINK_END    10

// 1.3" screen use sh1106
// 0.96" screen use ssd1306
#define OLED_SH1106   1

// Global Variables
OLED oled;
MIDI midi;
Settings settings;
ButtonReaderFS3X fs3x(FS_TIP_PIN, FS_RING_PIN);
uint8_t current_program = 1;
uint8_t next_program = 1;
bool blink_screen = false;
unsigned int blink_count = 0;
bool simple_mode = true;

void setup() {
    Serial.begin(MIDI_BAUD_RATE);
    midi.begin(&Serial);
    oled.begin(OLED_SH1106);
    fs3x.init();
    settings.begin(&oled, BUTTON_UP, BUTTON_DOWN, BUTTON_MODE);
    oled.printProgramChange(current_program);
}

bool read_simple_mode(uint8_t button_pressed) {
    byte b_state = fs3x.get_action_for_button(button_pressed);
    switch(button_pressed) {
        case BUTTON_UP: {
            if (b_state == button_state::pressed) {
                oled.clearDisplay();
                oled.printCurrent(current_program);
                ++next_program;
                if (next_program == 100) {
                  next_program = 1;
                }
                oled.printProgramChange(next_program);
                blink_screen = true;
                if (settings.getDirectSend()) {
                    midi.sendProgramChange(current_program, settings.getMidiChannel());
                }
            }
            break;
        }
        case BUTTON_DOWN: {
            if (b_state == button_state::pressed) {
                oled.clearDisplay();
                oled.printCurrent(current_program);
                --next_program;
                if (next_program == 0) {
                  next_program = 99;
                }
                oled.printProgramChange(next_program);
                blink_screen = true;
                if (settings.getDirectSend()) {
                    midi.sendProgramChange(current_program, settings.getMidiChannel());
                }
            }
            break;
        }
        case BUTTON_MODE: {
            if (b_state == button_state::long_pressed || b_state == button_state::still_long_pressed) {
                return true;
            } else if (b_state == button_state::pressed){
                oled.clearDisplay();
                current_program = next_program;
                oled.printProgramChange(current_program);
                midi.sendProgramChange(current_program, settings.getMidiChannel());
                blink_screen = false;
                break;
            }
        }
    }
    if (settings.getBlinkScreen() && blink_screen) {
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
    return false;
}

void loop() {
    fs3x.read();
    byte button_pressed = fs3x.get_actioned_button();
    byte button_pressed_state = fs3x.get_action_for_button(button_pressed);
    if (simple_mode) {
        if (read_simple_mode(button_pressed)) {
            simple_mode = false;
            settings.displayMenu(true);
        }
    } else {
       if (settings.read(button_pressed, button_pressed_state)) {
            simple_mode = true;
            oled.clearDisplay();
            oled.printProgramChange(current_program);
       }
    }
    delay(300);
}
