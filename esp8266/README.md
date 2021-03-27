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
python -m esptool --port /dev/tty.usbserial-1410 erase_flash
python -m esptool --port /dev/tty.usbserial-1410 --baud 460800 write_flash --flash_size=detect 0 firmware/esp8266-20210202-v1.14.bin
```

### Test live

```shell
ampy --port /dev/tty.usbserial-1410 run src/main.py
```

### Connect to REPL

#### Using `screen`

```shell
screen /dev/tty.usbserial-1410 115200
```
exit with `Ctrl-A; Ctrl-\`

#### Using `rshell`

```shell
rshell -p rshell -p /dev/tty.usbserial-1410
```
exit with `Ctrl-D`

from `rshell`
```shell
repl
```
exit with `Ctrl-X`

## Wiring Schema

![diagram](img/diagram.png)

Note: Connect 74HC165 to Pin D3 (GPIO0) make the board fail to boot.


