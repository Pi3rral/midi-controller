#include "MIDI.h"

MIDI::MIDI() {}

void MIDI::init(Stream* _stream, uint8_t _default_channel) {
    this->stream = _stream;
    this->default_channel = _default_channel;
}

void MIDI::sendMIDI(uint8_t message_type, uint8_t channel, uint8_t data1, uint8_t data2) {
    channel--; // Decrement the channel, because MIDI channel 1 corresponds to binary channel 0
    message_type &= 0b11110000; // Make sure that only the high nibble of the message type is set
    channel &= 0b00001111; // Make sure that only the low nibble of the channel is set
    uint8_t status_byte = message_type | channel; // Combine the message_type (high nibble)
    // with the channel (low nibble)
    // Both the message type and the channel
    // should be 4 bits wide
    status_byte |= 0b10000000; // Set the most significant bit of the status byte
    data1 &= 0b01111111; // Clear the most significant bit of the data bytes
    data2 &= 0b01111111;
    this->stream->write(status_byte); // Send over Serial
    this->stream->write(data1);
    this->stream->write(data2);
}

void MIDI::sendMIDIMessage(MIDIMessage message) {
    this->sendMIDI(message.message_type, message.channel, message.data1, message.data2);
}

void MIDI::sendProgramChange(uint8_t program_number, uint8_t channel) {
    this->sendMIDI(PROGRAM_CHANGE, (channel == 0) ? this->default_channel : channel, program_number, 0);
}

void MIDI::sendControlChange(uint8_t controller_number, uint8_t controller_value, uint8_t channel) {
    this->sendMIDI(CONTROL_CHANGE, (channel == 0) ? this->default_channel : channel, controller_number, controller_value);
}
