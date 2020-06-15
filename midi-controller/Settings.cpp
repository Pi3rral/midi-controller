#include "Settings.h"
#include <EEPROM.h>


const char *main_settings_menu[] =
{
    "MIDI Channel",
    "Blink Before Change",
    "Send When Select",
    "Back",
};


void Settings::saveMidiChannel(byte _midi_channel) {
    EEPROM.write(MIDI_CHANNEL_ADDRESS, _midi_channel);
}

void Settings::saveBlinkScreen(bool _blink_screen) {
    EEPROM.write(BLINK_SCREEN_WHEN_SELECT_ADDRESS, _blink_screen);
}

void Settings::saveDirectSend(bool _direct_send) {
    EEPROM.write(DIRECT_SEND_ADDRESS, _direct_send);
}

Settings::Settings() {
}

void Settings::begin(OLED* _oled, uint8_t _button_up, uint8_t _button_down, uint8_t _button_select) {
    this->oled = _oled;
    this->button_up = _button_up;
    this->button_down = _button_down;
    this->button_select = _button_select;
    this->current_midi_channel = this->readMidiChannel();
    this->current_blink_screen = this->readBlinkScreen();
    this->current_direct_send = this->readDirectSend();
    if (this->current_midi_channel > 15 || this->current_midi_channel < 0) {
        this->current_midi_channel = 0;
    }
}

byte Settings::readMidiChannel() {
    return EEPROM.read(MIDI_CHANNEL_ADDRESS);
}

byte Settings::readBlinkScreen() {
    return EEPROM.read(BLINK_SCREEN_WHEN_SELECT_ADDRESS);
}

byte Settings::readDirectSend() {
    return EEPROM.read(DIRECT_SEND_ADDRESS);
}

byte Settings::getMidiChannel() {
    return this->current_midi_channel;
}

byte Settings::getBlinkScreen() {
    return this->current_blink_screen;
}

byte Settings::getDirectSend() {
    return this->current_direct_send;
}

void Settings::displayMenu(bool force_main) {
    if (force_main) {
        this->current_menu = MAIN_MENU;
    }
    switch (this->current_menu) {
        case MAIN_MENU: {
            this->displayMainMenu();
            break;
        }
        case MIDI_CHANNEL_MENU: {
            this->displayMidiMenu();
            break;
        }
        case BLINK_MENU: {
            this->displayBlinkMenu();
            break;
        }
        case DIRECT_SEND_MENU: {
            this->displayDirectMenu();
            break;
        }
    }
}

void Settings::displayMainMenu() {
    this->oled->clearDisplay();
    this->oled->displayMenu(main_settings_menu, 4);
}

void Settings::displayMidiMenu() {
    this->oled->clearDisplay();
    this->oled->print("MIDI Channel");
    this->oled->printProgramChange(this->current_midi_channel + 1);
}

void Settings::displayBlinkMenu() {
    this->oled->clearDisplay();
    this->oled->print("Blink Before");
    if (this->current_blink_screen) {
        this->oled->printProgramChange("Y");
    } else {
        this->oled->printProgramChange("N");
    }
}

void Settings::displayDirectMenu() {
    this->oled->clearDisplay();
    this->oled->print("Direct Send PC");
    if (this->current_direct_send) {
        this->oled->printProgramChange("Y");
    } else {
        this->oled->printProgramChange("N");
    }
}

bool Settings::read(uint8_t button_pressed, uint8_t state) {
    switch (this->current_menu) {
        case MAIN_MENU: {
            if (this->readMain(button_pressed, state)) {
                return true;
            }
            break;
        }
        case MIDI_CHANNEL_MENU: {
            if (this->readMidi(button_pressed, state)) {
                this->displayMainMenu();
            }
            break;
        }
        case BLINK_MENU: {
            if (this->readBlink(button_pressed, state)) {
                this->displayMainMenu();
            }
            break;
        }
        case DIRECT_SEND_MENU: {
            if (this->readDirect(button_pressed, state)) {
                this->displayMainMenu();
            }
            break;
        }
    }
    return false;
}

bool Settings::readMain(uint8_t button_pressed, uint8_t state) {
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
                case 0: {
                    this->current_menu = MIDI_CHANNEL_MENU;
                    this->displayMidiMenu();
                    break;
                }
                case 1: {
                    this->current_menu = BLINK_MENU;
                    this->displayBlinkMenu();
                    break;
                }
                case 2: {
                    this->current_menu = DIRECT_SEND_MENU;
                    this->displayDirectMenu();
                    break;
                }
                case 3: {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Settings::readMidi(uint8_t button_pressed, uint8_t state) {
    if (button_pressed == this->button_up) {
        if (state == button_state::pressed) {
            this->current_midi_channel += 1;
            if (this->current_midi_channel >= 16) {
                this->current_midi_channel = 0;
            }
            this->displayMidiMenu();
        }
    } else if (button_pressed == this->button_down) {
        if (state == button_state::pressed) {
            this->current_midi_channel -= 1;
            if (this->current_midi_channel <= 0) {
                this->current_midi_channel = 15;
            }
            this->displayMidiMenu();
        }
    } else if (button_pressed == this->button_select) {
        if (state == button_state::pressed) {
            this->saveMidiChannel(this->current_midi_channel);
            return true;
        }
    }
    return false;
}

bool Settings::readBlink(uint8_t button_pressed, uint8_t state) {
    if (button_pressed == this->button_up ||
        button_pressed == this->button_down) {
        if (state == button_state::pressed) {
            this->current_blink_screen = !this->current_blink_screen;
        }
        this->displayBlinkMenu();
    } else if (button_pressed == this->button_select) {
        if (state == button_state::pressed) {
            this->saveBlinkScreen(this->current_blink_screen);
            return true;
        }
    }
    return false;
}

bool Settings::readDirect(uint8_t button_pressed, uint8_t state) {
    if (button_pressed == this->button_up ||
        button_pressed == this->button_down) {
        if (state == button_state::pressed) {
            this->current_direct_send = !this->current_direct_send;
        }
        this->displayDirectMenu();
    } else if (button_pressed == this->button_select) {
        if (state == button_state::pressed) {
            this->saveDirectSend(this->current_direct_send);
            return true;
        }
    }
    return false;
}
