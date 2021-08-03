#include "OLED.h"

// using https://github.com/lexus2k/ssd1306
#include <Wire.h>
#include "ssd1306.h"
#include "nano_gfx.h"


OLED::OLED() { 
    // empty
}

void OLED::init() {
    ssd1306_setFixedFont(ssd1306xled_font6x8);
    ssd1306_128x64_i2c_init();
    ssd1306_fillScreen(0x00);
    ssd1306_clearScreen();
    ssd1306_positiveMode();
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
