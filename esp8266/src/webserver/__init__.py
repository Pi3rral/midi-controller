from .microdot_asyncio import Microdot

# from .microdot import Microdot


web = Microdot()


@web.route("/")
async def index(request):
    return "Hello, world!"
