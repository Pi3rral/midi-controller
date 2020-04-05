#ifndef OLED_H
#define OLED_H

#include "Arduino.h"

class OLED {
public:
    OLED();
    void init();
    void clearDisplay();
    void print(const char* message);
    void printProgramChange(int program_change);
    void printCurrent(int current);
};

#endif // OLED_H
