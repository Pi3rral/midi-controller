# SPDX-FileCopyrightText: 2019 Kevin J. Walters for Adafruit Industries
#
# SPDX-License-Identifier: MIT

"""
`adafruit_midi.control_change`
================================================================================

Control Change MIDI message.


* Author(s): Kevin J. Walters

Implementation Notes
--------------------

"""

from .midi_message import MIDIMessage

__version__ = "0.0.0-auto.0"
__repo__ = "https://github.com/adafruit/Adafruit_CircuitPython_MIDI.git"


class ControlChange(MIDIMessage):
    """Control Change MIDI message.

    :param int control: The control number, 0-127.
    :param int value: The 7bit value of the control, 0-127.

    """

    _STATUS = 0xB0
    _STATUSMASK = 0xF0
    LENGTH = 3

    def __init__(self, control, value, *, channel=None):
        self.control = control
        self.value = value
        super().__init__(channel=channel)
        if not 0 <= self.control <= 127 or not 0 <= self.value <= 127:
            raise self._EX_VALUEERROR_OOR

    def __bytes__(self):
        return bytes(
            [self._STATUS | (self.channel & self.CHANNELMASK), self.control, self.value]
        )

    @classmethod
    def from_bytes(cls, msg_bytes):
        return cls(msg_bytes[1], msg_bytes[2], channel=msg_bytes[0] & cls.CHANNELMASK)

    def __str__(self):
        return "control_change - control: {} - value: {} - channel: {}".format(
            self.control, self.value, self.channel
        )


ControlChange.register_message_type()