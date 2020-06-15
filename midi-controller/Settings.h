#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "OLED.h"
#include "ButtonREaderAbstract.h"


#define MIDI_CHANNEL_ADDRESS 0
#define BLINK_SCREEN_WHEN_SELECT_ADDRESS 1
#define DIRECT_SEND_ADDRESS 2

#define MAIN_MENU 0
#define MIDI_CHANNEL_MENU 1
#define BLINK_MENU 2
#define DIRECT_SEND_MENU 3

class Settings {
protected:
    uint8_t button_up = 0;
    uint8_t button_down = 1;
    uint8_t button_select = 2;
    uint8_t current_menu = MAIN_MENU;
    byte current_midi_channel = 1;
    bool current_blink_screen = 0;
    bool current_direct_send = 0;
    OLED* oled;

    byte readMidiChannel();
    byte readBlinkScreen();
    byte readDirectSend();
    void saveMidiChannel(byte _midi_channel);
    void saveBlinkScreen(bool _blink_screen);
    void saveDirectSend(bool _direct_send);

    bool readMain(uint8_t button_pressed, uint8_t button_state);
    bool readMidi(uint8_t button_pressed, uint8_t button_state);
    bool readBlink(uint8_t button_pressed, uint8_t button_state);
    bool readDirect(uint8_t button_pressed, uint8_t button_state);

    void displayMainMenu();
    void displayMidiMenu();
    void displayBlinkMenu();
    void displayDirectMenu();

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