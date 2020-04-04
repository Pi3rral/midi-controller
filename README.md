# Arduino MIDI Foot Controller

Simple MIDI foot controller made with an arduino and an external 3 buttons footswitch (type Digitech FS3X).

## Features

- Input Jack for a 3 switches controller (type Digitech FS3X)
- Output Jack MIDI TRS-A
- OLED Display


## Schematic

![Schematic](doc/MIDIControllerSchematic.png)

## Usage

### MIDI Channel

The controller is sending Program Change (PC) instructions on MIDI Channel 1.

### Footswitch

#### MODE Button

Pressing MODE button is actually sending the Program Change (PC) to the MIDI output.
Selecting the PC number is done by pressing UP and DOWN buttons.

#### UP/DOWN Buttons

Pressing UP or DOWN button will define which PC send when we will press the MODE button.

WARNING: UP or DOWN will NOT send the PC instruction. It will just select it for pressing MODE.
 