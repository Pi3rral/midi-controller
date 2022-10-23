from time import sleep
from .bank import Bank


FS3X_TIP_IDX = 7
FS3X_RING_IDX = 6


class Controller:

    bank = None
    lcd = None
    button_values = [0, 0, 0, 0, 0, 0, 0, 0]

    def __init__(self):
        self.bank = Bank()

    def read_buttons(self):
        return False

    def wait_bounce(self):
        pass

    def wait(self, seconds):
        sleep(seconds)

    def print_menu(self):
        if not self.lcd:
            return
        if not self.bank.is_loaded:
            self.lcd.clear()
            if self.bank.max_bank > 0:
                self.lcd.move_to(0, 0)
                self.lcd.putstr("Error Loading Bank " + str(self.bank.current_bank))
            self.lcd.move_to(0, 1)
            self.lcd.putstr(self.bank.load_error)
            return

        patch_names = self.bank.get_current_presets_names()
        self.lcd.clear()
        self.lcd.move_to(0, 3)
        self.lcd.putstr(patch_names[0])
        offset = int((6 - len(patch_names[1])) / 2)
        self.lcd.move_to(7 + max(offset, 0), 3)
        self.lcd.putstr(patch_names[1])
        self.lcd.move_to(20 - len(patch_names[2]), 3)
        self.lcd.putstr(patch_names[2])
        self.lcd.move_to(0, 1)
        self.lcd.putstr("Bank: " + self.bank.get_current_bank_name())
        self.lcd.move_to(0, 2)
        self.lcd.putstr("Page: " + str(self.bank.get_current_page()))
        self.lcd.move_to(0, 0)
        self.lcd.putstr(patch_names[3])
        offset = int((6 - len(patch_names[4])) / 2)
        self.lcd.move_to(7 + max(offset, 0), 0)
        self.lcd.putstr(patch_names[4])
        self.lcd.move_to(20 - len(patch_names[5]), 0)
        self.lcd.putstr(patch_names[5])

    def splash_screen(self, message, seconds):
        if not self.lcd:
            return
        self.lcd.clear()
        self.lcd.move_to(0, 0)
        self.lcd.putstr(message)
        self.wait(seconds)
        self.print_menu()

    def loop(self):
        if self.read_buttons():
            if self.button_values[FS3X_TIP_IDX]:
                if self.button_values[FS3X_RING_IDX]:
                    self.bank.swap_page()
                else:
                    self.bank.bank_up()
            elif self.button_values[FS3X_RING_IDX]:
                self.bank.bank_down()
            else:
                for i in range(0, 6):
                    if self.button_values[i]:
                        self.bank.button_pressed(i)
            self.wait_bounce()
            self.print_menu()

    def bank_up(self):
        self.bank.bank_up()

    def bank_down(self):
        self.bank.bank_down()

    def main(self):
        while True:
            self.loop()
