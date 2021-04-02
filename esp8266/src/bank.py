import os

from midi.ports import ESP8266TX1Port
from midi.message import Message

current_bank = 1
max_bank = 0

midi_port = ESP8266TX1Port()


def set_max_bank():
    global max_bank
    max_bank = len(os.listdir("/banks"))


def swap_page():
    print("swap_page")


def bank_up():
    global current_bank, max_bank
    print("bank_up")
    current_bank += 1
    if current_bank > max_bank:
        current_bank = 1
    load_bank()


def bank_down():
    global current_bank, max_bank
    print("bank_down")
    current_bank -= 1
    if current_bank <= 0:
        current_bank = max_bank
    load_bank()


def load_bank():
    global current_bank
    print("load_bank: {}".format(current_bank))


def button_pressed(button_number):
    global midi_port
    print("button_number: {}".format(button_number))
    message = Message("program_change", channel=current_bank, data1=button_number + 1)
    print("send midi message")
    print(message)
    midi_port.send(message)
