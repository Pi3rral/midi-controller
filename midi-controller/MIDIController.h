#ifndef __MIDICONTROLLER_H__
#define __MIDICONTROLLER_H__

#include <stdint.h>
#include <stream.h>

#define MIDI_BAUD_RATE   31250
#define DEFAULT_CHANNEL  1

#define NOTE_OFF         0x80
#define NOTE_ON          0x90
#define KEY_PRESSURE     0xA0
#define CONTROL_CHANGE   0xB0
#define PROGRAM_CHANGE   0xC0
#define CHANNEL_PRESSURE 0xD0
#define PITCH_BEND       0xE0

typedef struct MIDIMessage {
    uint8_t message_type;
    uint8_t channel;
    uint8_t data1;
    uint8_t data2;
};

class MIDIController {
protected:
    Stream* stream = nullptr;
    uint8_t default_channel = DEFAULT_CHANNEL;

public:
    MIDIController();
    void init(Stream* _stream, uint8_t _default_channel = DEFAULT_CHANNEL);
    void sendMIDIMessage(MIDIMessage message);
    void sendMIDI(uint8_t message_type, uint8_t channel, uint8_t data1, uint8_t data2);
    void sendProgramChange(uint8_t program_number, uint8_t channel = 0);
    void sendControlChange(uint8_t controller_number, uint8_t controller_value, uint8_t channel = 0);
};


#endif // __MIDICONTROLLER_H__
