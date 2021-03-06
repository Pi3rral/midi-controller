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
#define BUTTON_UP 7
#define BUTTON_DOWN 8
#define BUTTON_MODE 9

#define NB_BUTTONS 6
#define NB_BANKS 10

// Global Variables
MIDIController midi;
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

int buttons[] = {4, 5, 6, 7, 8, 9};

int current_bank = 1;

uint8_t read_button_fs3x_1()
{
    if (digitalRead(FS1_TIP_PIN) == LOW)
    {
        if (digitalRead(FS1_RING_PIN) == LOW)
        {
            return BUTTON_UP;
        }
        return BUTTON_MODE;
    }
    else if (digitalRead(FS1_RING_PIN) == LOW)
    {
        return BUTTON_DOWN;
    }
    return NO_BUTTON;
}

uint8_t read_button()
{
    for (int i = 0; i < NB_BUTTONS; i++)
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
    for (int i = 0; i < NB_BUTTONS; i++)
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

void display_bank_patches()
{
    char line[20];
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(String("BANK ") + current_bank);
    lcd.setCursor(0, 3);
    sprintf(line, "PC%02d    PC%02d    PC%02d", ((current_bank - 1) * NB_BANKS + 1), ((current_bank - 1) * NB_BANKS + 2), ((current_bank - 1) * NB_BANKS + 3));
    lcd.print(line);
    lcd.setCursor(0, 0);
    sprintf(line, "PC%02d    PC%02d    PC%02d", ((current_bank - 1) * NB_BANKS + 4), ((current_bank - 1) * NB_BANKS + 5), ((current_bank - 1) * NB_BANKS + 6));
    lcd.print(line);
}

void loop()
{
    uint8_t button_pressed = read_button();
    lcd.setCursor(0, 2);
    lcd.print(String("BUTTON ") + button_pressed + String(" "));

    switch (button_pressed)
    {
    case NO_BUTTON:
    {
        break;
    }
    case BUTTON_UP:
    {
        ++current_bank;
        if (current_bank > NB_BANKS)
        {
            current_bank = 1;
        }
        display_bank_patches();
        delay(300);
        break;
    }
    case BUTTON_DOWN:
    {
        --current_bank;
        if (current_bank < 1)
        {
            current_bank = NB_BANKS;
        }
        display_bank_patches();
        delay(300);
        break;
    }
    case BUTTON_MODE:
    {
        delay(300);
        break;
    }
    default:
        uint8_t pc_value = (current_bank - 1) * NB_BANKS + button_pressed;
        lcd.setCursor(10, 2);
        lcd.print(String("PC ") + pc_value);
        midi.sendProgramChange(pc_value);
        delay(300);
        break;
    }
}
