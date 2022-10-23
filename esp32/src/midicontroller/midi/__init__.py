class MidiPort:

    midi_object = None

    @classmethod
    def send(cls, message, channel):
        if cls.midi_object:
            cls.midi_object.send(message, channel)
