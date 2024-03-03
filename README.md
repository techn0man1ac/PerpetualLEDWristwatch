# 12 LED ATtiny "Perpetual" Wristwatch

![Device_face](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/Face.jpg)

The simple and "Perpetual" WristWatch(PWW) with 12 red LEDs, it have LIR2430 Li-Ion battery and 12 solar cells, based on a simple Microchip microcontroller ATtimy13. It full open source(firmware and hardware).

![PCB_back](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/Back.jpg)

# Video demonstration:

https://www.youtube.com/shorts/_Q3GRxYN068

# How it's work?

Every 0.5 sec CPU wake up(cycle iteration time 100 nS) and increase milliseconds value and go to the sleep mode(3-5 uA). Wristwatch have one button "NOW" and 12 LED, if you push the button - show times on LEDs in the next algorithm - first, show hours(1, 2, 3, 4, 5, 6 etc....) and second is show minutes with 5 min. accuracy(5, 10, 15, 20, 25, 30 etc....), show 0.5 sec to the hours and 0.5 for minutes -> sleep mode again.

# 3D model PCB:

PCB Up side:

![PCB_Up_3D_Render](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/PCB_face_3D_render.png)

PCB Down side:

![PCB_Down_side_3D_Render](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/PCB_back_3D_render.png)

PCB have size 35 mm in the diameter:

![PCB_size](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/PCB_size.png)

The thickness of the board is recommended to take 0.8mm otherwise it may not fit into the case.

# Our mission and vision

Mission:

"To develop a straightforward and eco-friendly watch that integrates innovative technologies with minimal components, ensuring energy efficiency is within reach for all.".

Vision:

"We aim to achieve a world where energy-saving technology is accessible to everyone, where our watch serves as a symbol of environmental stewardship and innovative solutions in energy efficiency."
