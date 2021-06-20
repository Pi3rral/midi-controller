try:
    import ujson
except:
    import json as ujson
try:
    import uos
except:
    import os as uos
import gc
from .preset import Preset


class Bank:

    DEFAULT_BANKS_DIRECTORY = "/banks"
    NB_PHYSICAL_BUTTONS = 6
    NB_PAGES = 2

    max_bank = 0
    current_bank = 1
    current_page = 0
    current_preset = 1
    presets = []
    presets_name = []
    name = None

    def __init__(self, banks_directory=None):
        self.banks_directory = banks_directory or self.DEFAULT_BANKS_DIRECTORY
        self.set_max_bank()
        self.load_bank()

    def set_max_bank(self):
        self.max_bank = len(uos.listdir(self.banks_directory))

    def swap_page(self):
        print("swap_page")
        self.current_page = (self.current_page + 1) % self.NB_PAGES

    def bank_up(self):
        print("bank_up")
        self.current_bank += 1
        if self.current_bank > self.max_bank:
            self.current_bank = 1
        self.load_bank()

    def bank_down(self):
        print("bank_down")
        self.current_bank -= 1
        if self.current_bank <= 0:
            self.current_bank = self.max_bank
        self.load_bank()

    def load_bank(self):
        print("load_bank: {}".format(self.current_bank))
        bank_file = uos.path.join(
            self.banks_directory, "bank_" + str(self.current_bank) + ".json"
        )
        with open(bank_file) as fp:
            bank_data = ujson.load(fp)
        self.name = bank_data.get("name")
        # del self.presets
        self.presets = []
        for preset in bank_data.get("presets"):
            self.presets.append(Preset(preset.get("name"), preset.get("actions")))
        for _ in range(len(self.presets), self.NB_PAGES * self.NB_PHYSICAL_BUTTONS):
            self.presets.append(Preset("NONE", []))
        self.presets_name = [preset.get_name() for preset in self.presets]
        gc.collect()

    def button_pressed(self, button_number):
        print("button_number: {}".format(button_number))
        self.presets[
            button_number + (self.current_page * self.NB_PHYSICAL_BUTTONS)
        ].pressed()

    def get_current_bank_name(self):
        return self.name

    def get_current_page(self):
        return self.current_page

    def get_current_presets_names(self):
        if self.current_page == 1:
            return self.presets_name[6:]
        return self.presets_name[:6]
