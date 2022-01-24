import wifi
from midicontroller.midi import MidiPort
from midicontroller.midi.adafruit_midi import MIDI
from midicontroller.midi.adafruit_midi.control_change import ControlChange
from midicontroller.midi.ports import ESP8266TXPort
from utime import sleep_ms

wifi.no_wifi()


midi_port = ESP8266TXPort(enable_tx1=True)
midi = MIDI(midi_out=midi_port)
MidiPort.midi_object = midi


def do_midi_action(parameters):
    # remove 1 to the channel to be compliant with everything else
    # channel 1-16 in json file -> channel 0-15 in data transmission
    channel = int(parameters["channel"]) - 1
    if channel < 0 or channel > 15:
        print("Channel must be between 1 and 16")
        return
    message = ControlChange(
        control=int(parameters["control"]),
        value=int(parameters["value"]),
        channel=channel,
    )
    MidiPort.send(message, channel)


def the_imp_change_boot(channel, n):
    print("------------------------")
    print("change boot on channel " + str(channel))
    message_1 = (
        {"type": "control_change", "channel": channel, "control": "8", "value": n},
    )
    message_2 = (
        {"type": "control_change", "channel": channel, "control": "9", "value": "18"},
    )
    message_3 = (
        {"type": "control_change", "channel": channel, "control": "9", "value": "52"},
    )
    message_4 = (
        {"type": "control_change", "channel": channel, "control": "9", "value": "0"},
    )
    do_midi_action(message_1)
    sleep_ms(100)
    do_midi_action(message_2)
    sleep_ms(100)
    do_midi_action(message_3)
    sleep_ms(100)
    do_midi_action(message_4)
    sleep_ms(100)
    print("done")
    print("------------------------")


def the_imp_reverse_polarity(channel):
    print("------------------------")
    print("reverse polarity on channel " + str(channel))
    message_1 = (
        {"type": "control_change", "channel": channel, "control": "19", "value": "95"},
    )
    message_2 = (
        {"type": "control_change", "channel": channel, "control": "9", "value": "18"},
    )
    message_3 = (
        {"type": "control_change", "channel": channel, "control": "9", "value": "52"},
    )
    message_4 = (
        {"type": "control_change", "channel": channel, "control": "9", "value": "1"},
    )
    do_midi_action(message_1)
    sleep_ms(100)
    do_midi_action(message_2)
    sleep_ms(100)
    do_midi_action(message_3)
    sleep_ms(100)
    do_midi_action(message_4)
    sleep_ms(100)
    print("done")
    print("------------------------")


def the_imp_change_led_threshold(channel, threshold):
    print("------------------------")
    print("change let threshold on channel " + str(channel))
    message_1 = (
        {
            "type": "control_change",
            "channel": channel,
            "control": "29",
            "value": threshold,
        },
    )
    message_2 = (
        {"type": "control_change", "channel": channel, "control": "9", "value": "18"},
    )
    message_3 = (
        {"type": "control_change", "channel": channel, "control": "9", "value": "52"},
    )
    message_4 = (
        {"type": "control_change", "channel": channel, "control": "9", "value": "2"},
    )
    do_midi_action(message_1)
    sleep_ms(100)
    do_midi_action(message_2)
    sleep_ms(100)
    do_midi_action(message_3)
    sleep_ms(100)
    do_midi_action(message_4)
    sleep_ms(100)
    print("done")
    print("------------------------")


def the_imp_test(channel):
    print("------------------------")
    print("test channel " + str(channel))
    pedal_on = (
        {"type": "control_change", "channel": channel, "control": "10", "value": "1"},
    )
    pedal_off = (
        {"type": "control_change", "channel": channel, "control": "10", "value": "0"},
    )
    print("sleep 1s")
    sleep_ms(1000)
    print("Pedal On")
    do_midi_action(pedal_on)
    print("sleep 2s")
    sleep_ms(2000)
    print("Pedal Off")
    do_midi_action(pedal_off)
    print("sleep 2s")
    sleep_ms(2000)
    print("Pedal On")
    do_midi_action(pedal_on)
    print("sleep 2s")
    sleep_ms(2000)
    print("Pedal Off")
    do_midi_action(pedal_off)
    print("sleep 1s")
    sleep_ms(1000)
    print("------------------------")


# for i in range(1, 17):
#     the_imp_test(i)

# Settings for Wampler Pantheon
# the_imp_channel = 5
# the_imp_change_boot(the_imp_channel, 15)
# the_imp_reverse_polarity(the_imp_channel)
# the_imp_change_led_threshold(the_imp_channel, 68)
# the_imp_test(the_imp_channel)

# Settings for Wampler Sovereign
the_imp_channel = 8
# the_imp_change_led_threshold(the_imp_channel, 68)
# the_imp_reverse_polarity(the_imp_channel)
the_imp_test(the_imp_channel)

# Settings for Pitch Fork
# the_imp_channel = 3
# the_imp_change_led_threshold(the_imp_channel, 44)
# the_imp_reverse_polarity(the_imp_channel)
# the_imp_change_boot(the_imp_channel, 15)
# the_imp_test(the_imp_channel)
