[![UA_version_README](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/LumiTime/Imgs/Flags/UA%402x.png)](https://github.com/techn0man1ac/PerpetualLEDWristwatch/blob/main/README_UA.md)
[![GB_version_README](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/LumiTime/Imgs/Flags/GB%402x.png)](https://github.com/techn0man1ac/PerpetualLEDWristwatch)

# 12 LED ATtiny "Perpetual" Wristwatch

![Device_face](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/LumiTime/Imgs/Face.jpg)

The simple and "Perpetual" WristWatch(project code name "LumiTime") with 12 red LEDs, it have LIR2430 Li-Ion battery and 12 solar cells, based on a simple Microchip microcontroller ATtiny13. It full open source(firmware and hardware).

![PCB_back](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/LumiTime/Imgs/Back.jpg)

# Video demonstration:

https://youtu.be/T3b3lKKN2u0

# How it's work?

Every 0.5 sec CPU wake up(cycle iteration time 100 nS) and increase milliseconds value and go to the sleep mode(3-5 uA). Wristwatch have one button "NOW" and 12 LED, if you push the button - show times on LEDs in the next algorithm - first, show hours(1, 2, 3, 4, 5, 6 etc....) and second is show minutes with 5 min. accuracy(5, 10, 15, 20, 25, 30 etc....), show 0.5 sec to the hours and 0.5 for minutes -> sleep mode again.

# 3D model PCB:

PCB face side:

![PCB_Up_3D_Render](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/LumiTime/Imgs/PCB_face_3D_render.png)

PCB back side:

![PCB_Down_side_3D_Render](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/LumiTime/Imgs/PCB_back_3D_render.png)

PCB have size 35 mm in the diameter:

![PCB_size](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/LumiTime/Imgs/PCB_size.png)

The thickness of the board is recommended to take 0.8mm otherwise it may not fit into the case.

# Gerber files and 3D model:

Files for fabrication:

https://github.com/techn0man1ac/PerpetualLEDWristwatch/tree/main/LumiTime/PCB

# PCB home page:

Fresh board project here:

https://oshwlab.com/raznie.podelki/attiny85binarywatches_copy

# Schematic:

It's have simple schematic, for save pins I used Charlieplexing(35 components in total):

https://en.wikipedia.org/wiki/Charlieplexing

![Schematic_Wristwatch](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/LumiTime/PCB/Schematic/Schematic_12LEDWachATtiny13_2024-03-17.png)

In this project i use for charging 12 light sensor BPW34S(PV array), on clear sun one cell make 0.5-0.6 V and ~2 mA, connect its in serial and make simply charging schematic - use 2 components, ZD1 to drop voltage 6 -> 4.7 V and make protection to leakage of current in solar panels in to diode D1 name LL4148(because it have little reverse current):

![1N4148_parameters](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/LumiTime/Imgs/1N4148_parameters.png)

The diode add 0.7 volts drop(4.7-0.7=4.0V) and battery don't overcharge up to 4 V.

# How to flash ATtiny

![AVR ISP programmer pinout](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/LumiTime/Imgs/ProgramingPins.png)

Here is pinout for AVR ISP programmer - unsolder battery and power up 3V pins to programmer 5V, MI->MISO programer(Arduino digital Pin 11), MO->MOSI(digital Pin 12), CK->SCK(digital Pin 13) RS->Reset(digital Pin 10) and GD-> programmer GND. 

How to flash ATtiny13 with Arduino:

https://www.hackster.io/taunoerik/programming-attiny13-with-arduino-uno-07beba

![FLASH](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/LumiTime/Imgs/FLASH.png)

ATtiny13 core selections:

- Board: ATtiny 13

- BOD: BOD Disabled

- Clock: 1.2 MHz internal osc.

- Compiler LTO: Enable

- Port: (Your Arduino port)

- Programmer: Arduino as ISP

![Arduino IDE](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/LumiTime/Imgs/ArduinoIDE.png)

Source code(for Arduino IDE - "12LEDWachATtiny13.ino") here:

https://github.com/techn0man1ac/PerpetualLEDWristwatch/tree/main/LumiTime/Code/12LEDWachATtiny13

The time, which start the clock, set only in the code, these are the 26 and 27 code lines.

This project full open source(PCB and code), so anyone can modernize it. But we must give credit to the previous project, without which there would be no "ATtiny13BinaryWatches":

https://github.com/techn0man1ac/ATtiny13BinaryWatches

# Power consumption

In standby mode (when the button is not pressed), the wristwatch consumes approximately 5 μA, the battery have capacity - 50 mAh, in time show mode watch consumes 2.5 mA, if you look at the time every day 6 times(0.25 per hour), the operating time is 1.5 seconds, using these data we can calculate battery life time. For example - calculator from Oregon Embedded:

https://oregonembedded.com/batterycalc.htm

We have ~year of work, of course, as the voltage drops, the consumption of the watch will also drop (for example - 2.7 V, the consumption is ~3 μA), therefore, the resulting lower value is the guaranteed uptime:

![BatteryLifeCalculator](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/LumiTime/Imgs/BatteryLifeCalculator.png)

However, if you give the device 5-10 minutes a day of street light (for example, take it out of your sleeve), and the working time will immediately increase several times. Here in the video I show how the watch charges even in cloudy weather:

https://youtu.be/5vcrXjH7wVM

It is worth noting that LED bulbs, which are now very popular for indoor lighting, do not provide a sufficient level of light in the spectrum at which solar panels work, so charging is possible only very close LED lamp.

# Our mission and vision

Mission:

"To develop a simply and eco-friendly watch that integrates innovative technologies with minimal components, ensuring energy efficiency is within reach for all.".

Vision:

"We aim to achieve a world where energy-saving technology is accessible to everyone, where our watch serves as a symbol of environmental stewardship and innovative solutions in energy efficiency."
