#include <stdint.h>
#include <stream.h>

const uint8_t NOTE_OFF = 0x80;
const uint8_t NOTE_ON = 0x90;
const uint8_t KEY_PRESSURE = 0xA0;
const uint8_t CC = 0xB0;
const uint8_t PROGRAM_CHANGE = 0xC0;
const uint8_t CHANNEL_PRESSURE = 0xD0;
const uint8_t PITCH_BEND = 0xE0;


void sendMIDI(Stream *S, uint8_t messageType, uint8_t channel, uint8_t data1, uint8_t data2 = 0);
