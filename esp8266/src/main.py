try:
    import uasyncio as asyncio
except ImportError:
    import asyncio

import wifi
from midicontroller.esp_controller import ESP32Controller

from webserver import web

wifi.connect_home_wifi()

app = ESP32Controller()


async def midicontroller():
    while True:
        app.loop()
        print("Midi controller await 4000ms")
        await asyncio.sleep_ms(4000)
        print("Midi controller bank up")
        app.bank.bank_up()
        app.print_menu()


async def webserver():
    await web.start_server(debug=True)
    print("Webserver end!")


async def main():
    asyncio.create_task(midicontroller())
    asyncio.create_task(webserver())
    while True:
        await asyncio.sleep(10)


asyncio.run(main())
