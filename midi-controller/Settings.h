#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "OLED.h"
#include "ButtonREaderAbstract.h"


#define MIDI_CHANNEL_ADDRESS 0
#define BLINK_SCREEN_WHEN_SELECT_ADDRESS 1
#define DIRECT_SEND_ADDRESS 2

class Settings {
protected:
    uint8_t button_up = 0;
    uint8_t button_down = 1;
    uint8_t button_select = 2;
    OLED* oled;
    void saveMidiChannel(byte _midi_channel);
    void saveBlinkScreen(byte _blink_screen);
    void saveDirectSend(byte _direct_send);

public:
    Settings();
    void begin(OLED* _oled, uint8_t _button_up, uint8_t _button_down, uint8_t _button_select);
    byte getMidiChannel();
    byte getBlinkScreen();
    byte getDirectSend();

    void displayMenu();
    bool read(uint8_t button_pressed, uint8_t button_state);
};

#endif //__SETTINGS_H__