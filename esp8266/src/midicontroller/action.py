# from .midi.ports import ESP8266TX1Port
# from .midi.adafruit_midi import MIDI
from .midi.adafruit_midi.program_change import ProgramChange
from .midi.adafruit_midi.control_change import ControlChange

# midi_port = ESP8266TX1Port()
# midi = MIDI(midi_out=midi_port)


class ActionType:
    MIDI = "midi"


class MIDIMessage:
    PROGRAM_CHANGE = "program_change"
    CONTROL_CHANGE = "control_change"


class Action:

    type = None
    parameters = {}

    def __init__(self, type, parameters):
        self.type = type
        self.parameters = parameters

    def do_action(self):
        # print("do_action: " + self.type + str(self.parameters))
        if self.type == ActionType.MIDI:
            self.do_midi_action()
        # raise NotImplemented

    def do_midi_action(self):
        if self.parameters["type"] == MIDIMessage.PROGRAM_CHANGE:
            message = ProgramChange(
                patch=int(self.parameters["patch"]),
                channel=int(self.parameters["channel"]),
            )
        print("send midi message: " + str(self.parameters))
