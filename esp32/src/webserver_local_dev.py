import asyncio
from midicontroller.webserver import web


async def webserver():
    await web.start_server(debug=True, host="127.0.0.1", port=5000)


asyncio.run(webserver())
