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


def the_imp_change_boot(channel, n):
    print("------------------------")
    print("change boot on channel " + str(channel))
    message_1 = Action(
        "midi",
        {"type": "control_change", "channel": channel, "control": "8", "value": n},
    )
    message_2 = Action(
        "midi",
        {"type": "control_change", "channel": channel, "control": "9", "value": "18"},
    )
    message_3 = Action(
        "midi",
        {"type": "control_change", "channel": channel, "control": "9", "value": "52"},
    )
    message_4 = Action(
        "midi",
        {"type": "control_change", "channel": channel, "control": "9", "value": "0"},
    )
    message_1.do_action()
    sleep_ms(100)
    message_2.do_action()
    sleep_ms(100)
    message_3.do_action()
    sleep_ms(100)
    message_4.do_action()
    sleep_ms(100)
    print("done")
    print("------------------------")


def test_on_channel(channel):
    print("------------------------")
    print("test channel " + str(channel))
    pedal_on = Action(
        "midi",
        {"type": "control_change", "channel": channel, "control": "10", "value": "1"},
    )
    pedal_off = Action(
        "midi",
        {"type": "control_change", "channel": channel, "control": "10", "value": "0"},
    )
    print("sleep 1s")
    sleep_ms(1000)
    print("Pedal On")
    pedal_on.do_action()
    print("sleep 2s")
    sleep_ms(2000)
    print("Pedal Off")
    pedal_off.do_action()
    print("sleep 2s")
    sleep_ms(2000)
    print("Pedal On")
    pedal_on.do_action()
    print("sleep 1s")
    sleep_ms(1000)
    print("------------------------")


test_on_channel(0)
# the_imp_change_boot(5, 30)


# for i in range(0, 16):
#     test_on_channel(i)
