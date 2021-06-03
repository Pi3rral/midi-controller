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

    def print_menu(self):
        if not self.lcd:
            return
        patch_names = self.bank.get_current_presets_names()
        self.lcd.move_to(0, 0)
        self.lcd.putstr(" ".join(patch_names[:3]))
        self.lcd.move_to(0, 1)
        self.lcd.putstr("Bank: " + self.bank.get_current_bank_name())
        self.lcd.move_to(0, 2)
        self.lcd.putstr("Page: " + str(self.bank.get_current_page()))
        self.lcd.move_to(0, 3)
        self.lcd.putstr(" ".join(patch_names[3:]))

    def loop(self):
        if self.read_buttons():
            if self.button_values[FS3X_TIP_IDX]:
                if self.button_values[FS3X_RING_IDX]:
                    self.bank.bank_up()
                else:
                    self.bank.swap_page()
            elif self.button_values[FS3X_RING_IDX]:
                self.bank.bank_down()
            else:
                for i in range(0, 6):
                    if self.button_values[i]:
                        self.bank.button_pressed(i)
            self.wait_bounce()
            self.print_menu()

    def main(self):
        while True:
            self.loop()
