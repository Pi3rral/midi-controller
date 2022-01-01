try:
    import uasyncio as asyncio
except ImportError:
    import asyncio

from midicontroller.controller_test import midi_controller_test as midi_controller


from webserver import web


async def midicontroller():
    while True:
        midi_controller.loop()
        # print("Midi controller await 4000ms")
        await asyncio.sleep(0.004)
        # print("Midi controller bank up")
        # app.bank.bank_up()
        # app.print_menu()


async def webserver():
    await web.start_server(debug=False)


async def main():
    asyncio.create_task(midicontroller())
    asyncio.create_task(webserver())
    while True:
        await asyncio.sleep(10)


asyncio.run(main())
