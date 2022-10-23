try:
    import uasyncio as asyncio
except ImportError:
    import asyncio

import wifi
from midicontroller.controller_test import midi_controller_test as midi_controller


from midicontroller.webserver import web

# start_webserver = False
start_webserver = True


async def midicontroller():
    while True:
        midi_controller.loop()
        await asyncio.sleep(0.004)


async def webserver():
    await web.start_server(debug=False)


async def main():
    asyncio.create_task(midicontroller())

    if start_webserver:
        # address = wifi.connect_home_wifi()
        midi_controller.splash_screen("connected", 3)
        asyncio.create_task(webserver())

    while True:
        await asyncio.sleep(10)


asyncio.run(main())
