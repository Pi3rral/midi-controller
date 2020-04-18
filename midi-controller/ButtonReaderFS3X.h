#ifndef BUTTONREADERFS3X_H
#define BUTTONREADERFS3X_H

#include "ButtonReaderAbstract.h"

#define BUTTON_MODE 0
#define BUTTON_DOWN 1
#define BUTTON_UP   2

class ButtonReaderFS3X: public ButtonReaderAbstract {
private:
    byte fs_tip_pin;
    byte fs_ring_pin;
public:
    ButtonReaderFS3X(byte _fs_tip_pin, byte _fs_ring_pin);
    void init();
    void read();
};

#endif //BUTTONREADERFS3X_H
