import curses
from time import sleep
from resource import getrusage, RUSAGE_SELF
from .controller import Controller
from .bank import Bank
from .lcd.lcd_api import LcdApi
from .midi import MidiPort


stdscr = curses.initscr()


class PrintMidi:
    def send(self, msg, channel=None):
        stdscr.addstr(5, 0, f"PrintMIDI send channel {channel}: {msg}")
        stdscr.refresh()
        sleep(2)
        # print(f"PrintMIDI send channel {channel}: {msg}")


class PrintLcd(LcdApi):
    def __init__(self):
        super().__init__(0, 0)
        curses.noecho()
        curses.cbreak()

    def __del__(self):
        curses.endwin()

    def hal_write_command(self, cmd):
        pass

    def hal_write_data(self, data):
        pass

    def clear(self):
        stdscr.clear()

    def putstr(self, string):
        stdscr.addstr(self.cursor_y, self.cursor_x, string)
        stdscr.refresh()


class ControllerTest(Controller):
    def __init__(self):
        self.bank = Bank(
            banks_directory="/Users/pleca/Documents/midi-controller/esp8266/banks"
        )
        self.lcd = PrintLcd()
        self.print_menu()
        MidiPort.midi_object = PrintMidi()

    def read_buttons(self):
        stdscr.addstr(4, 0, f"Memory usage: {getrusage(RUSAGE_SELF).ru_maxrss/1000}")
        stdscr.refresh()
        self.button_values = [0, 0, 0, 0, 0, 0, 0, 0]
        button = int(stdscr.getkey())
        if button > 9:
            return False
        elif button == 9:
            self.button_values[6] = 1
            self.button_values[7] = 1
        else:
            self.button_values[button - 1] = 1
        return True


if __name__ == "__main__":
    app = ControllerTest()
    app.main()
