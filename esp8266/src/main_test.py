try:
    import uasyncio as asyncio
except ImportError:
    import asyncio


from midicontroller import app_test

asyncio.run(app_test.main())
