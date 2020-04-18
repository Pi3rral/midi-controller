#include <Arduino.h>
#include "ButtonReaderAbstract.h"


ButtonReaderAbstract::ButtonReaderAbstract(byte _number_of_buttons, int _long_press_time) {
    number_of_buttons = _number_of_buttons;
    long_press_time = _long_press_time;
    buttons_state = new byte[number_of_buttons];
    buttons_action_time = new unsigned long[number_of_buttons];
    for (int i = 0; i < number_of_buttons; ++i) {
        buttons_state[i] = button_state::rest;
        buttons_action_time[i] = 0;
    }
}

bool ButtonReaderAbstract::is_multiple_button_pressed() {
    int nb_button_actioned = 0;
    for (int i = 0; i < number_of_buttons; ++i) {
        if (buttons_state[i] != button_state::rest) {
            ++nb_button_actioned;
        }
    }
    return nb_button_actioned > 0;
}

byte ButtonReaderAbstract::get_number_of_buttons() {
    return number_of_buttons;
}

int ButtonReaderAbstract::get_actioned_button() {
    for (int i = 0; i < number_of_buttons; ++i) {
        if (buttons_state[i] != button_state::rest) {
            return i;
        }
    }
    return -1;
}

byte ButtonReaderAbstract::get_action_for_button(int _button) {
    return buttons_state[_button];
}

void ButtonReaderAbstract::update_button_state(byte button_number, byte bit_state) {
    int m = millis();
    if (bit_state == 1) {
        switch(buttons_state[button_number]) {
            case button_state::released:
            case button_state::rest:
                if (m - buttons_action_time[button_number] < DOUBLE_PRESSED_MAX_INTERVAL) {
                    buttons_state[button_number] = button_state::double_pressed;
                } else {
                    buttons_state[button_number] = button_state::pressed;
                    buttons_action_time[button_number] = m;
                }
                break;
            case button_state::long_pressed:
                buttons_state[button_number] = button_state::still_long_pressed;
                break;
            case button_state::pressed:
                buttons_state[button_number] = button_state::still_pressed;
                break;
            case button_state::still_pressed:
                if (m - buttons_action_time[button_number] > long_press_time) {
                    buttons_state[button_number] = button_state::long_pressed;
                }
                break;
        }
    } else {
        switch(buttons_state[button_number]) {
            case button_state::long_pressed:
            case button_state::pressed: 
            case button_state::still_pressed:
            case button_state::still_long_pressed: {
                buttons_state[button_number] = button_state::released;
                buttons_action_time[button_number] = m;
                break;
            }
            default: {
                buttons_state[button_number] = button_state::rest;
                break;
            }
        }
    }
}
