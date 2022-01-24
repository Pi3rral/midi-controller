try:
    import ujson
except:
    import json as ujson
try:
    import uos
except:
    import os as uos
import gc

from .midi.adafruit_midi.program_change import ProgramChange
from .midi.adafruit_midi.control_change import ControlChange
from .midi import MidiPort

FS3X_TIP_IDX = 7
FS3X_RING_IDX = 6


class Controller:

    bank = None
    lcd = None
    button_values = [0, 0, 0, 0, 0, 0, 0, 0]
    banks_directory = "/banks_dir"

    ACTION_TYPE_MIDI = "midi"
    MIDI_MESSAGE_PROGRAM_CHANGE = "program_change"
    MIDI_MESSAGE_CONTROL_CHANGE = "control_change"
    NB_PHYSICAL_BUTTONS = 6
    NB_PAGES = 2

    def __init__(self):
        self.is_loaded = False
        self.load_error = ""
        self.max_bank = 0
        self.banks = []
        self.current_bank = 0
        self.current_page = 0
        self.current_preset = 1
        self.presets = []
        self.presets_name = []
        self.name = None
        self.set_banks()
        self.load_bank()
        self.init_lcd()
        self.print_menu()

    def init_lcd(self):
        self.lcd = None

    def set_banks(self):
        self.banks = sorted(uos.listdir(self.banks_directory))
        self.max_bank = len(self.banks)

    def swap_page(self):
        self.current_page = (self.current_page + 1) % self.NB_PAGES

    def bank_up(self):
        self.presets = []
        gc.collect()
        self.current_bank += 1
        if self.current_bank >= self.max_bank:
            self.current_bank = 0
        self.load_bank()

    def bank_down(self):
        self.presets = []
        gc.collect()
        self.current_bank -= 1
        if self.current_bank < 0:
            self.current_bank = self.max_bank - 1
        self.load_bank()

    def load_bank(self):
        bank_dir = self.banks_directory + "/" + self.banks[self.current_bank]
        nb_preset = len(uos.listdir(bank_dir)) - 1
        with open(bank_dir + "/bank.json") as fp:
            bank_data = ujson.load(fp)
        self.name = bank_data.get("name")
        self.current_page = 0
        self.presets = []
        for i in range(0, nb_preset):
            gc.collect()
            with open(bank_dir + "/preset_" + str(i) + ".json") as fp:
                preset_data = ujson.load(fp)
            self.presets.append(preset_data)
        self.presets_name = [preset["name"] for preset in self.presets]
        for _ in range(len(self.presets), self.NB_PAGES * self.NB_PHYSICAL_BUTTONS):
            self.presets_name.append("NONE")
        self.is_loaded = True
        self.load_error = ""
        on_load_preset = bank_data.get("on_load_preset", None)
        if on_load_preset is not None:
            self.button_pressed(on_load_preset)
        gc.collect()

    def button_pressed(self, button_number):
        preset_number = button_number + (self.current_page * self.NB_PHYSICAL_BUTTONS)
        if preset_number > len(self.presets):
            return
        for action in self.presets[preset_number]["actions"]:
            self.do_action(action)

    def do_action(self, action):
        if action["type"] == self.ACTION_TYPE_MIDI:
            self.do_midi_action(action["parameters"])

    def do_midi_action(self, parameters):
        # remove 1 to the channel to be compliant with everything else
        # channel 1-16 in json file -> channel 0-15 in data transmission
        channel = int(parameters["channel"]) - 1
        if channel < 0 or channel > 15:
            print("Channel must be between 1 and 16")
            return
        if parameters["type"] == self.MIDI_MESSAGE_PROGRAM_CHANGE:
            message = ProgramChange(
                patch=int(parameters["patch"]),
                channel=channel,
            )
        elif parameters["type"] == self.MIDI_MESSAGE_CONTROL_CHANGE:
            message = ControlChange(
                control=int(parameters["control"]),
                value=int(parameters["value"]),
                channel=channel,
            )
        else:
            return
        MidiPort.send(message, channel)

    def read_buttons(self):
        return False

    def wait_bounce(self):
        pass

    def print_menu(self):
        if not self.lcd:
            return
        if not self.is_loaded:
            self.lcd.clear()
            self.lcd.move_to(0, 0)
            self.lcd.putstr("Error Loading Bank " + str(self.current_bank))
            self.lcd.move_to(0, 1)
            self.lcd.putstr(self.load_error)
            return

        patch_names = (
            self.presets_name[6:] if self.current_page == 1 else self.presets_name[:6]
        )
        self.lcd.clear()
        self.lcd.move_to(0, 3)
        self.lcd.putstr(patch_names[0])
        offset = int((6 - len(patch_names[1])) / 2)
        self.lcd.move_to(7 + max(offset, 0), 3)
        self.lcd.putstr(patch_names[1])
        self.lcd.move_to(20 - len(patch_names[2]), 3)
        self.lcd.putstr(patch_names[2])
        self.lcd.move_to(0, 1)
        self.lcd.putstr("Bank: " + self.name)
        self.lcd.move_to(0, 2)
        self.lcd.putstr("Page: " + str(self.current_page))
        self.lcd.move_to(0, 0)
        self.lcd.putstr(patch_names[3])
        offset = int((6 - len(patch_names[4])) / 2)
        self.lcd.move_to(7 + max(offset, 0), 0)
        self.lcd.putstr(patch_names[4])
        self.lcd.move_to(20 - len(patch_names[5]), 0)
        self.lcd.putstr(patch_names[5])

    def loop(self):
        if self.read_buttons():
            if self.button_values[FS3X_TIP_IDX]:
                if self.button_values[FS3X_RING_IDX]:
                    self.swap_page()
                else:
                    self.bank_up()
            elif self.button_values[FS3X_RING_IDX]:
                self.bank_down()
            else:
                for i in range(0, 6):
                    if self.button_values[i]:
                        self.button_pressed(i)
            self.wait_bounce()
            self.print_menu()

    def main(self):
        while True:
            self.loop()
