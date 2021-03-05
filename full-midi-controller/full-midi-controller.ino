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

int buttons[] = {4, 5, 6, 7, 8, 9}; // all button inputs to set global pullup

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
        if (digitalRead(i) == LOW)
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
    lcd.setCursor(0, 1);
    lcd.print(String("BANK ") + current_bank);
}

void loop()
{
    uint8_t button_pressed = read_button();
    lcd.setCursor(0, 4);
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
        lcd.setCursor(0, 1);
        lcd.print(String("BANK ") + current_bank + String(" "));
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
        lcd.setCursor(0, 1);
        lcd.print(String("BANK ") + current_bank + String(" "));
        delay(300);
        break;
    }
    case BUTTON_MODE:
    {
        delay(300);
        break;
    }
    default:
        lcd.setCursor(0, 0);
        lcd.print(String("PC Value: ") + (current_bank - 1) * NB_BANKS + button_pressed);
        midi.sendProgramChange((current_bank - 1) * NB_BANKS + button_pressed);
        delay(300);
        break;
    }
}
