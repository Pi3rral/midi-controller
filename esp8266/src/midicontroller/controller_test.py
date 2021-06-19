# import sys

# print(sys.path)

from .controller import Controller
from .bank import Bank
from .lcd.lcd_api import LcdApi


class PrintLcd(LcdApi):
    def __init__(self):
        super().__init__(0, 0)

    def hal_write_command(self, cmd):
        pass

    def hal_write_data(self, data):
        pass

    def putstr(self, string):
        print(string)


class ControllerTest(Controller):
    def __init__(self):
        self.bank = Bank(
            banks_directory="/Users/pleca/Documents/midi-controller/esp8266/banks"
        )
        self.lcd = PrintLcd()
        self.print_menu()

    def read_buttons(self):
        self.button_values = [0, 0, 0, 0, 0, 0, 0, 0]
        button = int(input("Button pressed: "))
        if button > 9:
            return False
        elif button == 9:
            self.button_values[6] = 1
            self.button_values[7] = 1
        else:
            self.button_values[button - 1] = 1
        print("")
        return True


if __name__ == "__main__":
    app = ControllerTest()
    app.main()
