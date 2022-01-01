from .microdot_asyncio import Microdot, Response

from midicontroller.esp_controller_asyncio import midi_controller

# from midicontroller.controller_test import midi_controller_test as midi_controller


web = Microdot()


@web.route("/")
async def index(request):
    return "Hello, world!"


@web.route("/bank_up")
async def bank_up(request):
    try:
        midi_controller.bank.bank_up()
        midi_controller.print_menu()
        return Response(status_code=200)
    except Exception as e:
        return Response(body=str(e), status_code=404)


@web.route("/bank_down")
async def bank_up(request):
    midi_controller.bank.bank_down()
    midi_controller.print_menu()
    return "Bank Down!"


@web.route("/bank_info/<int:bank_number>")
async def get_bank_info(request, bank_number):
    try:
        return Response(
            body=midi_controller.bank.get_bank_info(bank_number), status_code=200
        )
    except Exception as e:
        return Response(body=str(e), status_code=404)


@web.route("/bank_info/<int:bank_number>/preset_info/<int:preset_number>")
async def get_preset_info(request, bank_number, preset_number):
    try:
        return Response(
            body=midi_controller.bank.get_preset_info(bank_number, preset_number),
            status_code=200,
        )
    except Exception as e:
        return Response(body=str(e), status_code=404)
