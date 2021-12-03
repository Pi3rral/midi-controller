from machine import UART


class ESP8266TXPort:
    def __init__(self, enable_tx0=False, enable_tx1=False) -> None:
        self.tx0 = None
        self.tx1 = None
        if enable_tx0:
            self.tx0 = UART(0, baudrate=31250)
        if enable_tx1:
            self.tx1 = UART(1, baudrate=31250)

    def write(self, packet, num) -> None:
        if self.tx0:
            self.tx0.write(packet)
        if self.tx1:
            self.tx1.write(packet)
