try:
    import uasyncio as asyncio
except ImportError:
    import asyncio

import wifi
from midicontroller.esp_controller_asyncio import midi_controller

from midicontroller.webserver import web


async def midicontroller():
    while True:
        midi_controller.loop()
        await asyncio.sleep(0.005)


async def webserver():
    # await web.start_server(debug=True)
    pass


async def main():
    print("Start Midi Controller")
    asyncio.create_task(midicontroller())

    # Start webserver at start if any button is pressed
    if midi_controller.read_buttons():
        # if False:
        print("Start Web Server")
        address = wifi.connect_home_wifi()
        midi_controller.splash_screen(address, 3)
        asyncio.create_task(webserver())
    else:
        print("No Web Server")

    print("Start Infinite Loop")
    while True:
        await asyncio.sleep(10)


# asyncio.run(main())
