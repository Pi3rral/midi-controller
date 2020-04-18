#ifndef OLED_H
#define OLED_H

#include "ssd1306.h"

class OLED {
private:
    SAppMenu menu;

public:
    OLED();
    void init(bool is_sh1106 = false);
    void displayOff();
    void displayOn();
    void clearDisplay();
    void print(const char* message);
    void printProgramChange(int program_change);
    void printCurrent(int current);

    void displayMenu(const char** _menu_items = nullptr);
    void menuUp();
    void menuDown();
    int getMenuSelection();
};

#endif // OLED_H
