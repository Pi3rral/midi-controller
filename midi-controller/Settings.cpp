#include "Settings.h"
#include <EEPROM.h>


const char *main_settings_menu[] =
{
    "MIDI Channel",
    "Blink Before Change",
    "Send When Select",
    "Save And Exit",
    "Exit No Save",
};


void Settings::saveMidiChannel(byte _midi_channel) {
    EEPROM.write(MIDI_CHANNEL_ADDRESS, _midi_channel);
}

void Settings::saveBlinkScreen(byte _blink_screen) {
    EEPROM.write(BLINK_SCREEN_WHEN_SELECT_ADDRESS, _blink_screen);
}

void Settings::saveDirectSend(byte _direct_send) {
    EEPROM.write(DIRECT_SEND_ADDRESS, _direct_send);
}

Settings::Settings() {
}

void Settings::begin(OLED* _oled, uint8_t _button_up, uint8_t _button_down, uint8_t _button_select) {
    this->oled = _oled;
    this->button_up = _button_up;
    this->button_down = _button_down;
    this->button_select = _button_select;
}

byte Settings::getMidiChannel() {
    return EEPROM.read(MIDI_CHANNEL_ADDRESS);
}

byte Settings::getBlinkScreen() {
    return EEPROM.read(BLINK_SCREEN_WHEN_SELECT_ADDRESS);
}

byte Settings::getDirectSend() {
    return EEPROM.read(DIRECT_SEND_ADDRESS);
}

void Settings::displayMenu() {
    this->oled->clearDisplay();
    this->oled->displayMenu(main_settings_menu, 5);
}

bool Settings::read(uint8_t button_pressed, uint8_t state) {
    if (button_pressed == this->button_up) {
        if (state == button_state::pressed) {
            this->oled->menuUp();
        }
    } else if (button_pressed == this->button_down) {
        if (state == button_state::pressed) {
            this->oled->menuDown();
        }
    } else if (button_pressed == this->button_select) {
        if (state == button_state::pressed) {
            switch(this->oled->getMenuSelection()) {
                case 4: {
                    return true;
                }
            }
        }
    }
    return false;
}
