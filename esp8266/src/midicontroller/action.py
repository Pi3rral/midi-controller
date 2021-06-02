# from .ports import ESP8266TX1Port
# from .adafruit_midi import MIDI

# midi_port = ESP8266TX1Port()
# midi = MIDI(midi_out=midi_port)


class ActionType:
    MIDI = "midi"


class Action:

    type = None
    parameters = {}

    def __init__(self, type, **parameters):
        self.type = type
        self.parameters = parameters

    def do_action(self):
        print(f"do_action: {self.action} - {self.parameters}")
        if self.type == ActionType.MIDI:
            self.do_midi_action()
        raise NotImplemented

    def do_midi_action(self):
        pass
