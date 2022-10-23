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
        # remove 1 to the channel to be compliant with everything else
        # channel 1-16 in json file -> channel 0-15 in data transmission
        channel = int(self.parameters["channel"]) - 1
        if channel < 0 or channel > 15:
            print("Channel must be between 1 and 16")
            return
        if self.parameters["type"] == MIDIMessage.PROGRAM_CHANGE:
            message = ProgramChange(
                patch=int(self.parameters["patch"]),
                channel=channel,
            )
        elif self.parameters["type"] == MIDIMessage.CONTROL_CHANGE:
            message = ControlChange(
                control=int(self.parameters["control"]),
                value=int(self.parameters["value"]),
                channel=channel,
            )
        else:
            return
        # print(self.parameters)
        MidiPort.send(message, channel)
