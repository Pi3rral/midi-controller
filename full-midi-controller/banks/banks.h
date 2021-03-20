#include "Arduino.h"
#include <avr/pgmspace.h>
#include "MIDIController.h"

#define BANK_NAME_LEN 10
#define BUTTON_NAME_LEN 6

#define BANK_VERSION 1
#define NB_MESSAGE_PER_ACTION 1

#define NB_INT_BUTTONS 6
#define NB_PAGES 2
#define NB_PRESETS 12

#define BUTTON_ACTION_MIDI_MESSAGE 4

// Structs definition
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

const button_property empty_preset PROGMEM = {
    .action = 0,
    .is_toggle = 0,
    .next_action = 0,
    {
        .name_action_1 = "EMPTY",
    },
};

typedef struct
{
    uint8_t version;
    uint8_t nb_pages;
    char name[BANK_NAME_LEN + 1];
    button_property *presets;
} bank; // size: 238 bytes

button_property empty_presets[NB_PRESETS] = {
    empty_preset,
    empty_preset,
    empty_preset,
    empty_preset,
    empty_preset,
    empty_preset,
    empty_preset,
    empty_preset,
    empty_preset,
    empty_preset,
    empty_preset,
    empty_preset,
};

// Default bank
void load_default_bank(bank &to_bank, uint8_t bank_number)
{
    to_bank.version = BANK_VERSION;
    sprintf(to_bank.name, "BANK %d", bank_number);
    to_bank.presets = empty_presets;
    for (int i = 0; i < NB_PRESETS; ++i)
    {
        to_bank.presets[i].action = BUTTON_ACTION_MIDI_MESSAGE;
        to_bank.presets[i].is_toggle = 1;
        to_bank.presets[i].next_action = 0;
        sprintf(to_bank.presets[i].name_action_1, "PC1 %02d", bank_number * i + 1);
        sprintf(to_bank.presets[i].name_action_2, "PC2 %02d", bank_number + i + 1);
        to_bank.presets[i].action_1[0].midi_channel = 1;
        to_bank.presets[i].action_1[0].message_type = PROGRAM_CHANGE;
        to_bank.presets[i].action_1[0].message_number = bank_number + i + 1;
        to_bank.presets[i].action_1[0].message_value = 0;
        to_bank.presets[i].action_2[0].midi_channel = 1;
        to_bank.presets[i].action_2[0].message_type = PROGRAM_CHANGE;
        to_bank.presets[i].action_2[0].message_number = bank_number + i + 1;
        to_bank.presets[i].action_2[0].message_value = 0;
    }
}

// Terraform messages and bank
#define TERRAFORM_MIDI_CHANNEL 1
const message terraform_off PROGMEM = {
    .midi_channel = TERRAFORM_MIDI_CHANNEL,
    .message_type = PROGRAM_CHANGE,
    .message_number = 9,
    .message_value = 0,
};

const message terraform_phaser PROGMEM = {
    .midi_channel = TERRAFORM_MIDI_CHANNEL,
    .message_type = PROGRAM_CHANGE,
    .message_number = 1,
    .message_value = 0,
};
const message terraform_flanger PROGMEM = {
    .midi_channel = TERRAFORM_MIDI_CHANNEL,
    .message_type = PROGRAM_CHANGE,
    .message_number = 2,
    .message_value = 0,
};
const message terraform_uvibe PROGMEM = {
    .midi_channel = TERRAFORM_MIDI_CHANNEL,
    .message_type = PROGRAM_CHANGE,
    .message_number = 3,
    .message_value = 0,
};
const message terraform_envelope_filter PROGMEM = {
    .midi_channel = TERRAFORM_MIDI_CHANNEL,
    .message_type = PROGRAM_CHANGE,
    .message_number = 4,
    .message_value = 0,
};
const message terraform_chorus PROGMEM = {
    .midi_channel = TERRAFORM_MIDI_CHANNEL,
    .message_type = PROGRAM_CHANGE,
    .message_number = 5,
    .message_value = 0,
};
const message terraform_dimension PROGMEM = {
    .midi_channel = TERRAFORM_MIDI_CHANNEL,
    .message_type = PROGRAM_CHANGE,
    .message_number = 6,
    .message_value = 0,
};
const message terraform_tremolo PROGMEM = {
    .midi_channel = TERRAFORM_MIDI_CHANNEL,
    .message_type = PROGRAM_CHANGE,
    .message_number = 7,
    .message_value = 0,
};
const message terraform_harmonic_tremolo PROGMEM = {
    .midi_channel = TERRAFORM_MIDI_CHANNEL,
    .message_type = PROGRAM_CHANGE,
    .message_number = 7,
    .message_value = 0,
};

button_property terraform_buttons[NB_PRESETS] = {
    {
        .action = BUTTON_ACTION_MIDI_MESSAGE,
        .is_toggle = 1,
        .next_action = 0,
        .name_action_1 = "phaser",
        .name_action_2 = "PHASER",
        .action_1 = {terraform_phaser},
        .action_2 = {terraform_off},
    },
    {
        .action = BUTTON_ACTION_MIDI_MESSAGE,
        .is_toggle = 1,
        .next_action = 0,
        .name_action_1 = "flange",
        .name_action_2 = "FLANGE",
        .action_1 = {terraform_flanger},
        .action_2 = {terraform_off},
    },
    {
        .action = BUTTON_ACTION_MIDI_MESSAGE,
        .is_toggle = 1,
        .next_action = 0,
        .name_action_1 = "u-vibe",
        .name_action_2 = "U-VIBE",
        .action_1 = {terraform_uvibe},
        .action_2 = {terraform_off},
    },
    {
        .action = BUTTON_ACTION_MIDI_MESSAGE,
        .is_toggle = 1,
        .next_action = 0,
        .name_action_1 = "chorus",
        .name_action_2 = "CHORUS",
        .action_1 = {terraform_chorus},
        .action_2 = {terraform_off},
    },
    {
        .action = BUTTON_ACTION_MIDI_MESSAGE,
        .is_toggle = 1,
        .next_action = 0,
        .name_action_1 = "dimcho",
        .name_action_2 = "DIMCHO",
        .action_1 = {terraform_dimension},
        .action_2 = {terraform_off},
    },
    {
        .action = BUTTON_ACTION_MIDI_MESSAGE,
        .is_toggle = 1,
        .next_action = 0,
        .name_action_1 = "trmolo",
        .name_action_2 = "TRMOLO",
        .action_1 = {terraform_tremolo},
        .action_2 = {terraform_off},
    },
    {
        .action = BUTTON_ACTION_MIDI_MESSAGE,
        .is_toggle = 1,
        .next_action = 0,
        .name_action_1 = "envfil",
        .name_action_2 = "ENVFIL",
        .action_1 = {terraform_envelope_filter},
        .action_2 = {terraform_off},
    },
    {
        .action = BUTTON_ACTION_MIDI_MESSAGE,
        .is_toggle = 1,
        .next_action = 0,
        .name_action_1 = "h-trem",
        .name_action_2 = "H-TREM",
        .action_1 = {terraform_harmonic_tremolo},
        .action_2 = {terraform_off},
    },
    &empty_preset,
    &empty_preset,
    &empty_preset,
    &empty_preset,
};

void load_terraform_bank(bank &to_bank)
{
    to_bank.version = BANK_VERSION;
    to_bank.nb_pages = NB_PAGES;
    sprintf(to_bank.name, "TERRAFORM");
    to_bank.presets = terraform_buttons;
}
