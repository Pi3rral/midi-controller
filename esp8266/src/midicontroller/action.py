from .midi.adafruit_midi.program_change import ProgramChange
from .midi.adafruit_midi.control_change import ControlChange
from .midi import MidiPort


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
        if self.type == ActionType.MIDI:
            self.do_midi_action()

    def do_midi_action(self):
        if self.parameters["type"] == MIDIMessage.PROGRAM_CHANGE:
            message = ProgramChange(
                patch=int(self.parameters["patch"]),
                channel=int(self.parameters["channel"]),
            )
            channel = int(self.parameters["channel"])
        elif self.parameters["type"] == MIDIMessage.CONTROL_CHANGE:
            message = ControlChange(
                control=int(self.parameters["control"]),
                value=int(self.parameters["value"]),
                channel=int(self.parameters["channel"]),
            )
            channel = int(self.parameters["channel"])
        else:
            return
        # print(self.parameters)
        MidiPort.send(message, channel)
