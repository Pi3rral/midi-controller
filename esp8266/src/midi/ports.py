from machine import UART


class ESP8266TX1Port:
    def __init__(self) -> None:
        self.port = UART(1, baudrate=32500)

    def send(self, message):
        self.port.write(message.bytes())
