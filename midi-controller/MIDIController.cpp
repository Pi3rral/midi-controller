#include "MIDIController.h"

MIDIController::MIDIController() {}

void MIDIController::init(Stream* _stream) {
    this->stream = _stream;
}

void MIDIController::sendMIDI(uint8_t messageType, uint8_t channel, uint8_t data1, uint8_t data2) {
    channel--; // Decrement the channel, because MIDI channel 1 corresponds to binary channel 0
    messageType &= 0b11110000; // Make sure that only the high nibble of the message type is set
    channel &= 0b00001111; // Make sure that only the low nibble of the channel is set
    uint8_t statusByte = messageType | channel; // Combine the messageType (high nibble)
    // with the channel (low nibble)
    // Both the message type and the channel
    // should be 4 bits wide
    statusByte |= 0b10000000; // Set the most significant bit of the status byte
    data1 &= 0b01111111; // Clear the most significant bit of the data bytes
    data2 &= 0b01111111;
    this->stream->write(statusByte); // Send over Serial
    this->stream->write(data1);
    this->stream->write(data2);
}
