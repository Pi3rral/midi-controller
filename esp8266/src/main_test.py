try:
    import uasyncio as asyncio
except ImportError:
    import asyncio

from midicontroller.controller_test import ControllerTest

app = ControllerTest()

from webserver import web


async def midicontroller():
    await app.main()


async def webserver():
    await web.start_server(debug=True)


async def main():
    w = asyncio.create_task(webserver())
    m = asyncio.create_task(midicontroller())
    await asyncio.gather(w, m)


asyncio.run(main())
