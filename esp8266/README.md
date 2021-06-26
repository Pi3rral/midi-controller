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

```shell
python -m esptool --port /dev/tty.usbserial-1420 erase_flash
python -m esptool --port /dev/tty.usbserial-1420 --baud 460800 write_flash --flash_size=detect 0 firmware/esp8266-20210202-v1.14.bin
```

### Test live

```shell
ampy --port /dev/tty.usbserial-1420 run src/main.py
```

### Upload file

```shell
ampy --port /dev/tty.usbserial-1420 put src/wifi.py /wifi.py
ampy --port /dev/tty.usbserial-1420 put src/midicontroller /midicontroller
ampy --port /dev/tty.usbserial-1420 put banks /banks
ampy --port /dev/tty.usbserial-1420 put src/main.py /main.py
```

### Connect to REPL

#### Using `screen`

```shell
screen /dev/tty.usbserial-1420 115200
```
exit with `Ctrl-A; Ctrl-\`

#### Using `rshell`

```shell
rshell -p /dev/tty.usbserial-1420
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


