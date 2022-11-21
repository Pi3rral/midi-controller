from utime import sleep_us, sleep_ms
from machine import Pin, SoftI2C
from .controller import Controller
from .bank import Bank
from .lcd.esp8266_i2c_lcd import I2cLcd
from .midi.ports import ESP32TXPort
from .midi.adafruit_midi import MIDI
from .midi import MidiPort
from .env import Env

PULSE_WIDTH_USEC = 5
PEDAL_LCD_ADDRESS = 0x3F


class ESPController(Controller):

    pin_scl = 22
    pin_sda = 21
    pin_pl = 14
    pin_clk = 12
    pin_data = 13

    # WARNING! 0x27 is the breadbord address
    # actual pedal is 0x3F
    # lcd_address = 0x27
    # lcd_address = 0x3F

    def __init__(self):
        # init pins
        self.pl_pin = Pin(self.pin_pl, Pin.OUT)
        self.clk_pin = Pin(self.pin_clk, Pin.OUT)
        self.data_pin = Pin(self.pin_data, Pin.IN)
        self.clk_pin.value(0)
        self.lcd_address = Env.get("lcd_address", PEDAL_LCD_ADDRESS)

        # init midi
        midi_port = ESP32TXPort(enable_tx0=False, enable_tx1=False, enable_tx2=True)
        midi = MIDI(midi_out=midi_port)
        MidiPort.midi_object = midi

        # init bank and LCD
        self.bank = Bank()
        self.lcd = I2cLcd(
            SoftI2C(scl=Pin(self.pin_scl), sda=Pin(self.pin_sda)),
            self.lcd_address,
            4,
            20,
        )
        self.print_menu()

    def read_buttons(self):
        self.pl_pin.value(0)
        sleep_us(PULSE_WIDTH_USEC)
        self.pl_pin.value(1)
        button_pressed = False

        for i in range(0, 8):
            pressed = not self.data_pin.value()
            self.button_values[i] = pressed
            self.clk_pin.value(1)
            sleep_us(PULSE_WIDTH_USEC)
            self.clk_pin.value(0)
            if pressed:
                button_pressed = True
        return button_pressed

    def wait_bounce(self):
        sleep_ms(200)
