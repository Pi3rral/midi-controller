# Using Micropython on ESP8266 Nodemcu v3 board

## Tools

### Useful Links

https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
https://docs.micropython.org/en/latest/esp8266/tutorial/intro.html
https://www.digikey.ca/en/maker/projects/micropython-basics-load-files-run-code/fb1fcedaf11e4547943abfdd8ad825ce

### Local Virtualenv

Using pyenv-virtualenv `micropython`: `pyenv virtualenv activate micropython`

```shell
pip install -r requirements.txt
```

### Flash firmware

Erase and Flash in one command.

```shell
python -m esptool --port /dev/tty.usbserial-1430 --baud 460800 write_flash --erase-all --flash_size=detect 0 firmware/esp32-generic-midigoat.bin
```

Or Erase first, then flash
```shell
python -m esptool --port /dev/tty.usbserial-1430 erase_flash
python -m esptool --port /dev/tty.usbserial-1430 --baud 460800 write_flash --flash_size=detect 0 firmware/esp8266-20210902-v1.17.bin
```

#### Build Firmware

Building in docker image:

```shell
docker build -f firmware/Dockerfile -t midigoat .
docker run --rm -ti --volume $PWD/firmware:/root/export midigoat /bin/bash -c "cp /micropython/ports/esp32/build-GENERIC/firmware.bin /root/export/esp32-generic-midigoat.bin"
```

Firmware is located in the image at `/micropython/ports/esp32/build-GENERIC/firmware.bin`

### Test live

```shell
ampy --port /dev/tty.usbserial-1430 run src/main.py
```

### Upload file

Don't forget to remove `pyc` files!

```shell
find . | grep -E "(__pycache__|\.pyc|\.pyo$)" | xargs rm -rf
```

```shell
ampy --port /dev/tty.usbserial-1430 put src/wifi.py /wifi.py
ampy --port /dev/tty.usbserial-1430 put src/midicontroller /midicontroller
ampy --port /dev/tty.usbserial-1430 put banks_dir /banks_dir
ampy --port /dev/tty.usbserial-1430 put src/main.py /main.py
```

### Connect to REPL

#### Using `screen`

```shell
screen /dev/tty.usbserial-1430 115200
```
exit with `Ctrl-A; Ctrl-\`

#### Using `rshell`

```shell
rshell -p /dev/tty.usbserial-1430
```
exit with `Ctrl-D`

from `rshell`
```shell
repl
```
exit with `Ctrl-X`

#### Free Flash Space

From REPL

```python
import uos
fs_stat = uos.statvfs('/')
fs_size = fs_stat[0] * fs_stat[2]
fs_free = fs_stat[0] * fs_stat[3]
print("File System Size {:,} - Free Space {:,}".format(fs_size, fs_free))
```

## Wiring Schema

R-Tx = 10 Ohm

R-V3.3 = 47 Ohm

![diagram](img/diagram.png)

Note: Connect 74HC165 to Pin D3 (GPIO0) make the board fail to boot.

## External Libraries Used

https://github.com/adafruit/Adafruit_CircuitPython_MIDI
- added `__str__` to `MIDIMessage`, `ProgramChange` and `ControlChange` classes

https://github.com/dhylands/python_lcd
