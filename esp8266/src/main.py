try:
    import uasyncio as asyncio
except ImportError:
    import asyncio

import wifi
from midicontroller.esp_controller_asyncio import midi_controller

from webserver import web

address = wifi.connect_home_wifi()
midi_controller.splash_screen(address, 3)


async def midicontroller():
    while True:
        midi_controller.loop()
        await asyncio.sleep(0.005)


async def webserver():
    await web.start_server(debug=True)


async def main():
    asyncio.create_task(midicontroller())
    asyncio.create_task(webserver())
    while True:
        await asyncio.sleep(10)


asyncio.run(main())
