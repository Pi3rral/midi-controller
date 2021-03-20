#include <Arduino.h>
#include <avr/pgmspace.h>
#include "banks/banks.h"
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

#define NB_INT_BUTTONS 6
#define NB_TOTAL_BUTTONS 9
#define NB_BANKS 5

// Global Variables
MIDIController midi;
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

bank bank_loaded;

uint8_t buttons[] = {4, 5, 6, 7, 8, 9};

uint8_t current_bank = 1;
uint8_t current_page = 0;

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
    switch (current_bank)
    {
    case 1:
    {
        load_terraform_bank(bank_loaded);
        break;
    }
    default:
    {
        load_default_bank(bank_loaded, current_bank);
        break;
    }
    }
}

void display_bank_patches()
{
    // char line[20];
    uint8_t col_offset = 0;
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("BANK: ");
    lcd.setCursor(6, 1);
    lcd.print(bank_loaded.name);
    // line 1 - preset 4, 5, 6
    lcd.setCursor(0, 0);
    uint8_t preset_offset = current_page * NB_INT_BUTTONS;
    lcd.print(bank_loaded.presets[3 + preset_offset].is_toggle && bank_loaded.presets[3 + preset_offset].next_action ? bank_loaded.presets[3 + preset_offset].name_action_2 : bank_loaded.presets[3 + preset_offset].name_action_1);
    col_offset = (6 - strlen(bank_loaded.presets[4 + preset_offset].is_toggle && bank_loaded.presets[4 + preset_offset].next_action ? bank_loaded.presets[4 + preset_offset].name_action_2 : bank_loaded.presets[4 + preset_offset].name_action_1)) / 2;
    lcd.setCursor(7 + col_offset, 0);
    lcd.print(bank_loaded.presets[4 + preset_offset].is_toggle && bank_loaded.presets[4 + preset_offset].next_action ? bank_loaded.presets[4 + preset_offset].name_action_2 : bank_loaded.presets[4 + preset_offset].name_action_1);
    col_offset = 5 - strlen(bank_loaded.presets[5 + preset_offset].is_toggle && bank_loaded.presets[5 + preset_offset].next_action ? bank_loaded.presets[5 + preset_offset].name_action_2 : bank_loaded.presets[5 + preset_offset].name_action_1);
    lcd.setCursor(15 + col_offset, 0);
    lcd.print(bank_loaded.presets[5 + preset_offset].is_toggle && bank_loaded.presets[5 + preset_offset].next_action ? bank_loaded.presets[5 + preset_offset].name_action_2 : bank_loaded.presets[5 + preset_offset].name_action_1);
    // line 4 - preset 1, 2, 3
    lcd.setCursor(0, 3);
    lcd.print(bank_loaded.presets[0 + preset_offset].is_toggle && bank_loaded.presets[0 + preset_offset].next_action ? bank_loaded.presets[0 + preset_offset].name_action_2 : bank_loaded.presets[0 + preset_offset].name_action_1);
    col_offset = (6 - strlen(bank_loaded.presets[1 + preset_offset].is_toggle && bank_loaded.presets[1 + preset_offset].next_action ? bank_loaded.presets[1 + preset_offset].name_action_2 : bank_loaded.presets[1 + preset_offset].name_action_1)) / 2;
    lcd.setCursor(7 + col_offset, 3);
    lcd.print(bank_loaded.presets[1 + preset_offset].is_toggle && bank_loaded.presets[1 + preset_offset].next_action ? bank_loaded.presets[1 + preset_offset].name_action_2 : bank_loaded.presets[1 + preset_offset].name_action_1);
    col_offset = 5 - strlen(bank_loaded.presets[2 + preset_offset].is_toggle && bank_loaded.presets[2 + preset_offset].next_action ? bank_loaded.presets[2 + preset_offset].name_action_2 : bank_loaded.presets[2 + preset_offset].name_action_1);
    lcd.setCursor(15 + col_offset, 3);
    lcd.print(bank_loaded.presets[2 + preset_offset].is_toggle && bank_loaded.presets[2 + preset_offset].next_action ? bank_loaded.presets[2 + preset_offset].name_action_2 : bank_loaded.presets[2 + preset_offset].name_action_1);
}

void loop()
{
    uint8_t button_pressed = read_button();

    if (button_pressed != NO_BUTTON)
    {
        if (button_pressed == FS3X_BUTTON_UP)
        {
            if (current_bank >= NB_BANKS)
            {
                current_bank = 0;
            }
            else
            {
                ++current_bank;
            }
            load_bank();
            display_bank_patches();
            delay(300);
        }
        else if (button_pressed == FS3X_BUTTON_DOWN)
        {
            if (current_bank == 1)
            {
                current_bank = NB_BANKS - 1;
            }
            else
            {
                --current_bank;
            }
            load_bank();
            display_bank_patches();
            delay(300);
        }
        else if (button_pressed == FS3X_BUTTON_MODE)
        {
            ++current_page;
            if (current_page >= bank_loaded.nb_pages)
            {
                current_page = 0;
            }
            display_bank_patches();
            delay(300);
        }
        else
        {
            uint8_t preset_number = (current_page * NB_INT_BUTTONS) + button_pressed - 1;
            switch (bank_loaded.presets[preset_number].action)
            {
            case BUTTON_ACTION_MIDI_MESSAGE:
            {
                if (bank_loaded.presets[preset_number].is_toggle && bank_loaded.presets[preset_number].next_action)
                {
                    // midi.sendMIDI(bank_loaded.presets[preset_number].action_2[0].message_type, bank_loaded.presets[preset_number].action_2[0].midi_channel, bank_loaded.presets[preset_number].action_2[0].message_number, bank_loaded.presets[preset_number].action_2[0].message_value);
                    bank_loaded.presets[preset_number].next_action = 0;
                }
                else
                {
                    // midi.sendMIDI(bank_loaded.presets[preset_number].action_1[0].message_type, bank_loaded.presets[preset_number].action_1[0].midi_channel, bank_loaded.presets[preset_number].action_1[0].message_number, bank_loaded.presets[preset_number].action_1[0].message_value);
                    bank_loaded.presets[preset_number].next_action = 1;
                }
                display_bank_patches();
                delay(300);
                break;
            }
            }
        }
    }
}
