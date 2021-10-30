import wifi
from midicontroller.midi import MidiPort
from midicontroller.midi.adafruit_midi import MIDI
from midicontroller.midi.ports import ESP8266TX1Port
from midicontroller.action import Action
from utime import sleep_ms

wifi.no_wifi()


midi_port = ESP8266TX1Port()
midi = MIDI(midi_out=midi_port)
MidiPort.midi_object = midi


plumes_on = Action(
    "midi",
    {"type": "control_change", "channel": "6", "control": "10", "value": "1"},
)
plumes_off = Action(
    "midi",
    {"type": "control_change", "channel": "6", "control": "10", "value": "0"},
)
print("sleep 2s")
sleep_ms(2000)
print("Plumes On")
plumes_on.do_action()
print("sleep 3s")
sleep_ms(3000)
print("Plumes Off")
plumes_off.do_action()
sleep_ms(3000)
print("Plumes On")
plumes_on.do_action()
print("sleep 2s")
sleep_ms(2000)
