# ATtiny LED Wristwatch

The simple and "Perpetual" WristWatch(PWW) with 12 res LEDs, it have LIR2430 Li-Ion battery and 12 solar cells, based on a simple Microchip microcontroller ATtimy13. It full open source(firmware and hardware).

/* WristWatch photo */

# Video demonstration:

https://www.youtube.com/shorts/_Q3GRxYN068

# How it's work?

Every 0.5 sec CPU wake up(cycle iteration time 100 nS) and increase milliseconds value and go to the sleep mode(3-5 uA). Wristwatch have one button "NOW" and 12 LED, if you push the button - show times on LEDs in the next algorithm - first, show hours(1, 2, 3, 4, 5, 6 etc....) and second is show minutes with 5 min. accuracy(5, 10, 15, 20, 25, 30 etc....), show 0.5 sec to the hours and 0.5 for minutes -> sleep mode again.

# Our mission and vision

Mission:

"To develop a straightforward and eco-friendly watch that integrates innovative technologies with minimal components, ensuring energy efficiency is within reach for all.".

Vision:

"We aim to achieve a world where energy-saving technology is accessible to everyone, where our watch serves as a symbol of environmental stewardship and innovative solutions in energy efficiency."
