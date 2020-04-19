#include <Arduino.h>
#include "ButtonReaderFS3X.h"

ButtonReaderFS3X::ButtonReaderFS3X(byte _fs_tip_pin, byte _fs_ring_pin):
    ButtonReaderAbstract(3) {
    this->fs_tip_pin = _fs_tip_pin;
    this->fs_ring_pin = _fs_ring_pin;
}

void ButtonReaderFS3X::init() {
    pinMode(this->fs_tip_pin, INPUT_PULLUP);
    pinMode(this->fs_ring_pin, INPUT_PULLUP);
}

void ButtonReaderFS3X::read() {
    if (digitalRead(this->fs_tip_pin) == LOW) {
        if (digitalRead(this->fs_ring_pin) == LOW) {
            update_button_state(BUTTON_UP, 1);
            update_button_state(BUTTON_MODE, 0);
            update_button_state(BUTTON_DOWN, 0);
        }
        update_button_state(BUTTON_MODE, 1);
        update_button_state(BUTTON_UP, 0);
        update_button_state(BUTTON_DOWN, 0);
    } else if (digitalRead(this->fs_ring_pin) == LOW) {
        update_button_state(BUTTON_DOWN, 1);
        update_button_state(BUTTON_UP, 0);
        update_button_state(BUTTON_MODE, 0);
    } else {
        update_button_state(BUTTON_MODE, 0);
        update_button_state(BUTTON_DOWN, 0);
        update_button_state(BUTTON_UP, 0);
    }
}
