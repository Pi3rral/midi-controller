#ifndef BUTTONREADERABSTRACT_H
#define BUTTONREADERABSTRACT_H

#define DEFAULT_LONG_PRESS_TIME 3000
#define DOUBLE_PRESSED_MAX_INTERVAL 500


// Button States
enum button_state {
    rest = 0,
    pressed = 1,
    still_pressed = 2,
    long_pressed = 3,
    still_long_pressed = 4,
    released = 5,
    double_pressed = 6
};

class ButtonReaderAbstract {
    
protected:
    int long_press_time;
    byte number_of_buttons;

    byte* buttons_state;
    unsigned long* buttons_action_time;

    void update_button_state(byte button_number, byte bit_state);

public:
    ButtonReaderAbstract(byte _number_of_buttons, int _long_press_time = DEFAULT_LONG_PRESS_TIME);
    virtual void init() = 0;
    virtual void read() = 0;
    byte get_number_of_buttons();
    bool is_multiple_button_pressed();
    int get_actioned_button();
    byte get_action_for_button(int _button);
};

#endif //BUTTONREADERABSTRACT_H
