try:
    import uasyncio as asyncio
except ImportError:
    import asyncio


from .esp_controller import ESPController


class ESP32ControllerAsync(ESPController):
    async def wait_bounce(self):
        await asyncio.sleep(0.2)

    async def wait(self, seconds):
        await asyncio.sleep(seconds)

    async def main(self):
        while True:
            self.loop()
            await asyncio.sleep(0.004)


midi_controller = ESP32ControllerAsync()
