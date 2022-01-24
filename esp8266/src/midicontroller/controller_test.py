import curses
from time import sleep
from resource import getrusage, RUSAGE_SELF
import psutil
from .controller import Controller
from .lcd.lcd_api import LcdApi
from .midi import MidiPort


stdscr = curses.initscr()


class PrintMidi:
    def send(self, msg, channel=None):
        stdscr.addstr(6, 0, f"PrintMIDI send channel {channel}: {msg}")
        stdscr.refresh()
        sleep(1)
        # print(f"PrintMIDI send channel {channel}: {msg}")


class PrintLcd(LcdApi):
    def __init__(self):
        super().__init__(0, 0)
        curses.noecho()
        curses.cbreak()
        stdscr.timeout(100)

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


MidiPort.midi_object = PrintMidi()


class ControllerTest(Controller):
    banks_directory = "/Users/pleca/Documents/midi-controller/esp8266/banks_dir"

    def init_lcd(self):
        self.lcd = PrintLcd()

    def read_buttons(self):
        # stdscr.addstr(4, 0, f"Memory usage: {getrusage(RUSAGE_SELF).ru_maxrss/1000}")
        # stdscr.addstr(5, 0, f"Memory used: {psutil.virtual_memory()[3]}")
        # stdscr.refresh()
        self.button_values = [0, 0, 0, 0, 0, 0, 0, 0]
        try:
            button = int(stdscr.getkey())
            if button > 9:
                return False
            elif button == 9:
                self.button_values[6] = 1
                self.button_values[7] = 1
            else:
                self.button_values[button - 1] = 1
            return True
        except Exception as e:
            pass
        return False
