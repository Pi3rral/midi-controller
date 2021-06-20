from utime import sleep_us, sleep_ms
from machine import Pin, I2C
from .controller import Controller
from .bank import Bank
from .lcd.esp8266_i2c_lcd import I2cLcd
from .midi.ports import ESP8266TX1Port
from .midi.adafruit_midi import MIDI
from .midi import MidiPort


PULSE_WIDTH_USEC = 5


class ESP8266Controller(Controller):
    def __init__(self):
        # init pins
        self.pl_pin = Pin(14, Pin.OUT)
        self.clk_pin = Pin(12, Pin.OUT)
        self.data_pin = Pin(13, Pin.IN)
        self.clk_pin.value(0)

        # init midi
        midi_port = ESP8266TX1Port()
        midi = MIDI(midi_out=midi_port)
        MidiPort.midi_object = midi

        # init bank and LCD
        self.bank = Bank()
        self.lcd = I2cLcd(I2C(scl=Pin(5), sda=Pin(4)), 0x3F, 4, 20)
        self.print_menu()

    def read_buttons(self):
        self.pl_pin.value(0)
        sleep_us(PULSE_WIDTH_USEC)
        self.pl_pin.value(1)
        button_pressed = False

        for i in range(0, 8):
            bitVal = self.data_pin.value()
            self.button_values[i] = bitVal
            self.clk_pin.value(1)
            sleep_us(PULSE_WIDTH_USEC)
            self.clk_pin.value(0)
            if bitVal:
                button_pressed = True
        return button_pressed

    def wait_bounce(self):
        return sleep_ms(200)
