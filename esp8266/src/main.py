from machine import Pin, ADC
from utime import sleep_us, sleep_ms

PULSE_WIDTH_USEC = 5

pl_pin = Pin(16, Pin.OUT)
clk_pin = Pin(2, Pin.OUT)
data_pin = ADC(0)

clk_pin.value(0)

button_values = [0, 0, 0, 0, 0, 0, 0, 0]


def read_buttons():
    pl_pin.value(0)
    sleep_us(PULSE_WIDTH_USEC)
    pl_pin.value(1)
    button_pressed = False

    for i in range(0, 8):
        bitVal = data_pin.read()
        # print("{i}: {bitVal}".format(i=i, bitVal=bitVal))
        if bitVal > 1000:
            button_values[i] = 1
            button_pressed = True
        else:
            button_values[i] = 0
        clk_pin.value(1)
        sleep_us(PULSE_WIDTH_USEC)
        clk_pin.value(0)

    if button_pressed:
        print(
            "buttons: {},{},{},{},{},{},{},{}".format(
                button_values[0],
                button_values[1],
                button_values[2],
                button_values[3],
                button_values[4],
                button_values[5],
                button_values[6],
                button_values[7],
            )
        )


def connect_home_wifi():
    import network

    sta_if = network.WLAN(network.STA_IF)
    ap_if = network.WLAN(network.AP_IF)

    print("Trying to connect to home network...")
    sta_if.active(True)
    sleep_ms(500)
    sta_if.connect("WiGoat", "")
    for i in range(0, 15):
        print(".")
        sleep_ms(1000)
        if sta_if.isconnected():
            print("Connected to home: {}".format(sta_if.ifconfig()[0]))
            ap_if.active(False)
            return
    sta_if.active(False)
    ap_if.active(True)
    print("Could not connect to home, Creating MiGoat access point")
    ap_if.config(essid="MiGoat", password="")
    sleep_ms(5000)


connect_home_wifi()

print("Read buttons...")
while True:
    read_buttons()
