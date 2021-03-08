#include <Arduino.h>
#include <avr/pgmspace.h>
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

#define BUTTON_ACTION_NO_ACTION 0
#define BUTTON_ACTION_BANK_UP 1
#define BUTTON_ACTION_BANK_DOWN 2
#define BUTTON_ACTION_PAGE_SWAP 3
#define BUTTON_ACTION_MIDI_MESSAGE 4

#define NB_INT_BUTTONS 6
#define NB_TOTAL_BUTTONS 9
#define NB_BANKS 5

#define BANK_NAME_LEN 10
#define BUTTON_NAME_LEN 6

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
    char name_action_1[BUTTON_NAME_LEN + 1];
    char name_action_2[BUTTON_NAME_LEN + 1];
    message action_1[NB_MESSAGE_PER_ACTION];
    message action_2[NB_MESSAGE_PER_ACTION];
} button_property; // size: 25 bytes (because nb_message=1)

struct
{
    uint8_t version;
    char name[BANK_NAME_LEN];
    button_property presets[NB_TOTAL_BUTTONS];
} bank; // size: 238 bytes

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

    load_bank();

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
        bank.presets[i].action = BUTTON_ACTION_MIDI_MESSAGE;
        bank.presets[i].is_toggle = 1;
        bank.presets[i].next_action = 0;
        sprintf(bank.presets[i].name_action_1, "PC1 %02d", current_bank * NB_BANKS + i + 1);
        sprintf(bank.presets[i].name_action_2, "PC2 %02d", current_bank * NB_BANKS + i + 1);
        bank.presets[i].action_1[0].midi_channel = 1;
        bank.presets[i].action_1[0].message_type = PROGRAM_CHANGE;
        bank.presets[i].action_1[0].message_number = current_bank * NB_BANKS + i + 1;
        bank.presets[i].action_1[0].message_value = 0;
        bank.presets[i].action_2[0].midi_channel = 1;
        bank.presets[i].action_2[0].message_type = PROGRAM_CHANGE;
        bank.presets[i].action_2[0].message_number = current_bank * NB_BANKS + i + 1;
        bank.presets[i].action_2[0].message_value = 0;
    }
    // hardcode buttons for external footswitch of type FS3X
    bank.presets[FS3X_BUTTON_UP].action = BUTTON_ACTION_BANK_UP;
    bank.presets[FS3X_BUTTON_UP].is_toggle = 0;
    bank.presets[FS3X_BUTTON_UP].next_action = 0;
    bank.presets[FS3X_BUTTON_DOWN].action = BUTTON_ACTION_BANK_DOWN;
    bank.presets[FS3X_BUTTON_DOWN].is_toggle = 0;
    bank.presets[FS3X_BUTTON_DOWN].next_action = 0;
    bank.presets[FS3X_BUTTON_MODE].action = BUTTON_ACTION_PAGE_SWAP;
    bank.presets[FS3X_BUTTON_MODE].is_toggle = 0;
    bank.presets[FS3X_BUTTON_MODE].next_action = 0;
}

void display_bank_patches()
{
    // char line[20];
    uint8_t col_offset = 0;
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("BANK: ");
    lcd.setCursor(6, 1);
    lcd.print(bank.name);
    // line 1 - preset 4, 5, 6
    lcd.setCursor(0, 0);
    lcd.print(bank.presets[3].is_toggle && bank.presets[3].next_action ? bank.presets[3].name_action_2 : bank.presets[3].name_action_1);
    col_offset = (6 - strlen(bank.presets[4].is_toggle && bank.presets[4].next_action ? bank.presets[4].name_action_2 : bank.presets[4].name_action_1)) / 2;
    lcd.setCursor(7 + col_offset, 0);
    lcd.print(bank.presets[4].is_toggle && bank.presets[4].next_action ? bank.presets[4].name_action_2 : bank.presets[4].name_action_1);
    col_offset = 5 - strlen(bank.presets[5].is_toggle && bank.presets[5].next_action ? bank.presets[5].name_action_2 : bank.presets[5].name_action_1);
    lcd.setCursor(15 + col_offset, 0);
    lcd.print(bank.presets[5].is_toggle && bank.presets[5].next_action ? bank.presets[5].name_action_2 : bank.presets[5].name_action_1);
    // line 4 - preset 1, 2, 3
    lcd.setCursor(0, 3);
    lcd.print(bank.presets[0].is_toggle && bank.presets[0].next_action ? bank.presets[0].name_action_2 : bank.presets[0].name_action_1);
    col_offset = (6 - strlen(bank.presets[1].is_toggle && bank.presets[1].next_action ? bank.presets[1].name_action_2 : bank.presets[1].name_action_1)) / 2;
    lcd.setCursor(7 + col_offset, 3);
    lcd.print(bank.presets[1].is_toggle && bank.presets[1].next_action ? bank.presets[1].name_action_2 : bank.presets[1].name_action_1);
    col_offset = 5 - strlen(bank.presets[2].is_toggle && bank.presets[2].next_action ? bank.presets[2].name_action_2 : bank.presets[2].name_action_1);
    lcd.setCursor(15 + col_offset, 3);
    lcd.print(bank.presets[2].is_toggle && bank.presets[2].next_action ? bank.presets[2].name_action_2 : bank.presets[2].name_action_1);
}

void loop()
{
    uint8_t button_pressed = read_button();

    if (button_pressed != NO_BUTTON)
    {
        switch (bank.presets[button_pressed].action)
        {
        case BUTTON_ACTION_BANK_UP:
        {
            ++current_bank;
            if (current_bank > NB_BANKS)
            {
                current_bank = 1;
            }
            load_bank();
            display_bank_patches();
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
            display_bank_patches();
            delay(300);
            break;
        }
        case BUTTON_ACTION_PAGE_SWAP:
        {
            break;
        }
        case BUTTON_ACTION_MIDI_MESSAGE:
        {
            if (bank.presets[button_pressed].is_toggle && bank.presets[button_pressed].next_action)
            {
                // midi.sendMIDI(bank.presets[button_pressed].action_2[0].message_type, bank.presets[button_pressed].action_2[0].midi_channel, bank.presets[button_pressed].action_2[0].message_number, bank.presets[button_pressed].action_2[0].message_value);
                bank.presets[button_pressed].next_action = 0;
            }
            else
            {
                // midi.sendMIDI(bank.presets[button_pressed].action_1[0].message_type, bank.presets[button_pressed].action_1[0].midi_channel, bank.presets[button_pressed].action_1[0].message_number, bank.presets[button_pressed].action_1[0].message_value);
                bank.presets[button_pressed].next_action = 1;
            }
            display_bank_patches();
            delay(300);
            break;
        }
        }
    }
}
