# BREADBOARD ONLY
# from midicontroller.env import Env
# Env.set("lcd_address", 0x27)

import uasyncio as asyncio
from midicontroller import app

asyncio.run(app.main())
