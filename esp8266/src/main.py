try:
    import uasyncio as asyncio
except ImportError:
    import asyncio

import wifi
from midicontroller.esp8266_controller import ESP8266Controller

wifi.no_wifi()

app = ESP8266Controller()
# app.main()


async def main():
    await app.main()


asyncio.run(main())
