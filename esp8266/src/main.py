import wifi
from midicontroller.esp8266_controller import ESP8266Controller

wifi.no_wifi()

app = ESP8266Controller()
app.main()
