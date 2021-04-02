class Message:

    commands = {
        "note_off": 0x80,
        "note_on": 0x90,
        "poly_pressure": 0xA0,
        "control_change": 0xB0,
        "program_change": 0xC0,
        "mono_pressure": 0xD0,
        "pitch_bend": 0xE0,
    }

    def __init__(self, type, data1, data2=0, channel=1):
        self.type = type
        self.data1 = data1
        self.data2 = data2
        self.channel = channel
        self.command = self.commands[self.type] + self.channel - 1

    def bytes(self):
        return bytes([self.command, self.data1, self.data2])

    def __str__(self) -> str:
        return "{} channel={} data1={} data2={}".format(
            self.type, self.channel, self.data1, self.data2
        )
