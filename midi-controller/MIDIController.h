#include <stdint.h>
#include <stream.h>

#define MIDI_BAUD_RATE   31250

#define NOTE_OFF         0x80
#define NOTE_ON          0x90
#define KEY_PRESSURE     0xA0
#define CC               0xB0
#define PROGRAM_CHANGE   0xC0
#define CHANNEL_PRESSURE 0xD0
#define PITCH_BEND       0xE0

class MIDIController {
protected:
    Stream* stream = nullptr;

public:
    MIDIController();
    void init(Stream* _stream);
    void sendMIDI(uint8_t messageType, uint8_t channel, uint8_t data1, uint8_t data2 = 0);
};

