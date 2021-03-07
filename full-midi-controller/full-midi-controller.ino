#include <Arduino.h>
#include "MIDIController.h"
#include "LiquidCrystal_I2C.h"

// FS Configuration
#define FS1_TIP_PIN A0
#define FS1_RING_PIN A1

// LCD parameters
#define LCD_ADDRESS 0x3F
#define LCD_COLUMNS 20
#define LCD_ROWS 4

#define NO_BUTTON 0
#define FS3X_BUTTON_UP 7
#define FS3X_BUTTON_DOWN 8
#define FS3X_BUTTON_MODE 9

#define BUTTON_ACTION_BANK_UP 1
#define BUTTON_ACTION_BANK_DOWN 2
#define BUTTON_ACTION_MIDI_MESSAGE 3

#define NB_INT_BUTTONS 6
#define NB_TOTAL_BUTTONS 9
#define NB_BANKS 5

#define BANK_NAME_LEN 10
#define BUTTON_NAME_LEN 5

#define BANK_VERSION 1
#define NB_MESSAGE_PER_ACTION 1

// Global Variables
MIDIController midi;
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

typedef struct
{
    uint8_t midi_channel;
    uint8_t message_type;
    uint8_t message_number;
    uint8_t message_value;
} message; // size: 4 bytes

typedef struct
{
    uint8_t action;
    uint8_t is_toggle;
    uint8_t next_action;
    char name_action_1[BUTTON_NAME_LEN];
    char name_action_2[BUTTON_NAME_LEN];
    message action_1[NB_MESSAGE_PER_ACTION];
    message action_2[NB_MESSAGE_PER_ACTION];
} button_property; // size: 21 bytes (because nb_message=1)

struct
{
    uint8_t version;
    char name[BANK_NAME_LEN];
    button_property buttons[NB_TOTAL_BUTTONS];
} bank; // size: 202 bytes

int buttons[] = {4, 5, 6, 7, 8, 9};

int current_bank = 1;

uint8_t read_button_fs3x_1()
{
    if (digitalRead(FS1_TIP_PIN) == LOW)
    {
        if (digitalRead(FS1_RING_PIN) == LOW)
        {
            return FS3X_BUTTON_UP;
        }
        return FS3X_BUTTON_MODE;
    }
    else if (digitalRead(FS1_RING_PIN) == LOW)
    {
        return FS3X_BUTTON_DOWN;
    }
    return NO_BUTTON;
}

uint8_t read_button()
{
    for (int i = 0; i < NB_INT_BUTTONS; i++)
    {
        if (digitalRead(buttons[i]) == LOW)
        {
            return i + 1;
        }
    }
    return read_button_fs3x_1();
}

void setup()
{
    // init midi
    Serial.begin(MIDI_BAUD_RATE);
    midi.init(&Serial);

    // init buttons
    for (int i = 0; i < NB_INT_BUTTONS; i++)
    {
        pinMode(buttons[i], INPUT_PULLUP); // input pullup used without external resistors
    }
    pinMode(FS1_TIP_PIN, INPUT_PULLUP);
    pinMode(FS1_RING_PIN, INPUT_PULLUP);

    // init LCD
    lcd.init();
    lcd.backlight();
    lcd.clear();
    display_bank_patches();
}

void load_bank()
{
    bank.version = BANK_VERSION;
    sprintf(bank.name, "BANK %d", current_bank);
    for (int i = 0; i < NB_INT_BUTTONS; ++i)
    {
        bank.buttons[i].action = BUTTON_ACTION_MIDI_MESSAGE;
        bank.buttons[i].is_toggle = 1;
        bank.buttons[i].next_action = 0;
        sprintf(bank.buttons[0].name_action_1, "pc %02d", current_bank * NB_BANKS + i + 1);
        sprintf(bank.buttons[0].name_action_1, "PC %02d", current_bank * NB_BANKS + i + 1);
        bank.buttons[0].action_1[0].midi_channel = 1;
        bank.buttons[0].action_1[0].message_type = PROGRAM_CHANGE;
        bank.buttons[0].action_1[0].message_type = current_bank * NB_BANKS + i + 1;
        bank.buttons[0].action_2[0].midi_channel = 1;
        bank.buttons[0].action_2[0].message_type = PROGRAM_CHANGE;
        bank.buttons[0].action_2[0].message_type = current_bank * NB_BANKS + i + 1;
    }
    // hardcode buttons for external footswitch of type FS3X
    bank.buttons[FS3X_BUTTON_UP].action = BUTTON_ACTION_BANK_UP;
    bank.buttons[FS3X_BUTTON_UP].is_toggle = 0;
    bank.buttons[FS3X_BUTTON_UP].next_action = 0;
    bank.buttons[FS3X_BUTTON_DOWN].action = BUTTON_ACTION_BANK_DOWN;
    bank.buttons[FS3X_BUTTON_DOWN].is_toggle = 0;
    bank.buttons[FS3X_BUTTON_DOWN].next_action = 0;
    bank.buttons[FS3X_BUTTON_MODE].action = BUTTON_ACTION_BANK_UP;
    bank.buttons[FS3X_BUTTON_MODE].is_toggle = 0;
    bank.buttons[FS3X_BUTTON_MODE].next_action = 0;
}

void display_bank_patches()
{
    char line[20];
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(bank.name);
    lcd.setCursor(0, 3);
    sprintf(line, "%6s %6s % 6s",
            bank.buttons[3].is_toggle && bank.buttons[3].next_action ? bank.buttons[3].name_action_2 : bank.buttons[3].name_action_1,
            bank.buttons[4].is_toggle && bank.buttons[4].next_action ? bank.buttons[4].name_action_2 : bank.buttons[4].name_action_1,
            bank.buttons[5].is_toggle && bank.buttons[5].next_action ? bank.buttons[5].name_action_2 : bank.buttons[5].name_action_1);
    lcd.print(line);
    lcd.setCursor(0, 0);
    sprintf(line, "%6s %6s % 6s",
            bank.buttons[0].is_toggle && bank.buttons[0].next_action ? bank.buttons[0].name_action_2 : bank.buttons[0].name_action_1,
            bank.buttons[1].is_toggle && bank.buttons[1].next_action ? bank.buttons[1].name_action_2 : bank.buttons[1].name_action_1,
            bank.buttons[2].is_toggle && bank.buttons[2].next_action ? bank.buttons[2].name_action_2 : bank.buttons[2].name_action_1);
    lcd.print(line);
}

void loop()
{
    uint8_t button_pressed = read_button();
    lcd.setCursor(0, 2);
    lcd.print(String("BUTTON ") + button_pressed + String(" "));

    if (button_pressed)
    {
        switch (bank.buttons[button_pressed].action)
        {
        case BUTTON_ACTION_BANK_UP:
        {
            ++current_bank;
            if (current_bank > NB_BANKS)
            {
                current_bank = 1;
            }
            load_bank();
            delay(300);
            break;
        }
        case BUTTON_ACTION_BANK_DOWN:
        {
            --current_bank;
            if (current_bank < 1)
            {
                current_bank = NB_BANKS;
            }
            load_bank();
            delay(300);
            break;
        }
        case BUTTON_ACTION_MIDI_MESSAGE:
        {
            if (bank.buttons[button_pressed].is_toggle && bank.buttons[button_pressed].next_action)
            {
                midi.sendMIDI(bank.buttons[button_pressed].action_2[0].message_type, bank.buttons[button_pressed].action_2[0].midi_channel, bank.buttons[button_pressed].action_2[0].message_number, bank.buttons[button_pressed].action_2[0].message_value);
            }
            else
            {
                midi.sendMIDI(bank.buttons[button_pressed].action_1[0].message_type, bank.buttons[button_pressed].action_1[0].midi_channel, bank.buttons[button_pressed].action_1[0].message_number, bank.buttons[button_pressed].action_1[0].message_value);
            }
            if (bank.buttons[button_pressed].is_toggle)
            {
                bank.buttons[button_pressed].next_action = !bank.buttons[button_pressed].next_action;
            }
            delay(300);
            break;
        }
        }
        display_bank_patches();
    }

    // switch (button_pressed)
    // {
    // case NO_BUTTON:
    // {
    //     break;
    // }
    // case FS3X_BUTTON_UP:
    // {
    //     ++current_bank;
    //     if (current_bank > NB_BANKS)
    //     {
    //         current_bank = 1;
    //     }
    //     display_bank_patches();
    //     delay(300);
    //     break;
    // }
    // case FS3X_BUTTON_DOWN:
    // {
    //     --current_bank;
    //     if (current_bank < 1)
    //     {
    //         current_bank = NB_BANKS;
    //     }
    //     display_bank_patches();
    //     delay(300);
    //     break;
    // }
    // case FS3X_BUTTON_MODE:
    // {
    //     delay(300);
    //     break;
    // }
    // default:
    //     uint8_t pc_value = (current_bank - 1) * NB_BANKS + button_pressed;
    //     lcd.setCursor(10, 2);
    //     lcd.print(String("PC ") + pc_value);
    //     midi.sendProgramChange(pc_value);
    //     delay(300);
    //     break;
    // }
}
