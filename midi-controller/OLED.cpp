#include "OLED.h"

// using https://github.com/lexus2k/ssd1306
#include <Wire.h>
#include "ssd1306.h"
#include "nano_gfx.h"


OLED::OLED() { 
    // empty
}

void OLED::init(bool is_sh1106) {
    ssd1306_setFixedFont(ssd1306xled_font6x8);
    if (is_sh1106) {
        sh1106_128x64_i2c_init();
    } else {
        ssd1306_128x64_i2c_init();
    }
    ssd1306_fillScreen(0x00);
    ssd1306_positiveMode();
    ssd1306_clearScreen();
    ssd1306_createMenu(&menu, nullptr, 0);
}

void OLED::clearDisplay() {
    ssd1306_clearScreen();
}

void OLED::displayOff() {
    ssd1306_displayOff();
}

void OLED::displayOn() {
    ssd1306_displayOn();
}

void OLED::print(const char* message) {
    // font size in nano_gfx_types.h
    ssd1306_printFixedN(0, 0, message, STYLE_NORMAL, FONT_SIZE_2X);

    // ssd1306_printFixed(0,  8, "Normal text", STYLE_NORMAL);
    // ssd1306_printFixed(0, 16, "Bold text", STYLE_BOLD);
    // ssd1306_printFixed(0, 24, "Italic text", STYLE_ITALIC);
    // ssd1306_negativeMode();
    // ssd1306_printFixed(0, 32, "Inverted bold", STYLE_BOLD);
    // ssd1306_positiveMode();
    // delay(3000);
    // ssd1306_clearScreen();
    // ssd1306_printFixedN(0, 0, "N3", STYLE_NORMAL, FONT_SIZE_8X);
    // delay(3000);
}

void OLED::printProgramChange(int program_change) {
    ssd1306_printFixedN(0, 10, String(program_change).c_str(), STYLE_NORMAL, FONT_SIZE_8X);
}

void OLED::printCurrent(int current) {
    ssd1306_printFixedN(100, 0, String(current).c_str(), STYLE_NORMAL, FONT_SIZE_2X);
}

void OLED::displayMenu(const char** _menu_items) {
    if (_menu_items != nullptr) {
        ssd1306_createMenu(&menu, _menu_items, sizeof(_menu_items)/sizeof(char *));
    }
    ssd1306_showMenu(&menu);
}

void OLED::menuUp() {
    ssd1306_menuUp(&menu);
    ssd1306_updateMenu(&menu);
}

void OLED::menuDown() {
    ssd1306_menuDown(&menu);
    ssd1306_updateMenu(&menu);
}

int OLED::getMenuSelection() {
    return ssd1306_menuSelection(&menu);
}

