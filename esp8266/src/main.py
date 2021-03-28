from machine import Pin, ADC
from utime import sleep_us, sleep_ms
import wifi
import bank

PULSE_WIDTH_USEC = 5
FS3X_TIP_IDX = 7
FS3X_RING_IDX = 6

pl_pin = Pin(14, Pin.OUT)
clk_pin = Pin(12, Pin.OUT)
data_pin = Pin(13, Pin.IN)

clk_pin.value(0)

button_values = [0, 0, 0, 0, 0, 0, 0, 0]


def read_buttons():
    pl_pin.value(0)
    sleep_us(PULSE_WIDTH_USEC)
    pl_pin.value(1)
    button_pressed = False

    for i in range(0, 8):
        bitVal = data_pin.value()
        button_values[i] = bitVal
        clk_pin.value(1)
        sleep_us(PULSE_WIDTH_USEC)
        clk_pin.value(0)
        if bitVal:
            button_pressed = True

    # if button_pressed:
    #     print(
    #         "buttons: {},{},{},{},{},{},{},{}".format(
    #             button_values[0],
    #             button_values[1],
    #             button_values[2],
    #             button_values[3],
    #             button_values[4],
    #             button_values[5],
    #             button_values[6],
    #             button_values[7],
    #         )
    #     )
    return button_pressed


# wifi.connect_home_wifi()
wifi.no_wifi()

print("Read buttons...")
while True:
    sleep_ms(200)
    if read_buttons():
        if button_values[FS3X_TIP_IDX]:
            if button_values[FS3X_RING_IDX]:
                bank.bank_up()
            else:
                bank.swap_page()
        elif button_values[FS3X_RING_IDX]:
            bank.bank_down()
        else:
            for i in range(0, 6):
                if button_values[i]:
                    bank.button_pressed(i)
