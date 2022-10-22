try:
    import uasyncio as asyncio
except ImportError:
    import asyncio

from midicontroller import app

asyncio.run(app.main())
