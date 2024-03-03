/*
  Tech01 12 LED ATtiny "Perpetual" Wristwatch By Serhii Trush for MIT License
  https://github.com/techn0man1ac/PerpetualLEDWristwatch

  The simple and "Perpetual" WristWatch(PWW) with 12 res LEDs, it have LIR2430 Li-Ion battery and 12 solar cells,
  based on a simple Microchip microcontroller ATtimy13. It full open source(firmware and hardware).

  By Tech01 labs 2024.

  Fuses to defalt:
  low_fuses=0x6A
  high_fuses=0xFF
  CPU Frequensy 1,2 MHz
*/

#define F_CPU 1200000UL  // 1.2 MHz CPU mode(low_fuses=0x6A high_fuses=0xFF - defalt fuses)
#include <avr/io.h>
#include <avr/wdt.h>  // Need for "wdt_..." macross
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#define msPerCycleReal 559    // it's mean 500 ms in real life
#define MsIn12Hours 43200000  // 43200000 ms -> 43200 sec = 12h

uint8_t Hours = 9;    // <- Set time in hours here(0..11)
uint8_t Minutes = 55;  // <- Set time in minutes here(5..55)

volatile uint32_t MSec = Hours * 3600000 + Minutes * 60000;  // 33300000 is 09:15

uint8_t Mode = 0;

ISR(WDT_vect) {            // If button not press code add "tick". Iteration time is 100 nS
  MSec += msPerCycleReal;  // 500 ms per cycle

  if (MSec >= MsIn12Hours) {
    MSec = MSec - MsIn12Hours;  // increment MSec value compensation -> 43199999 + 555 = 43200554 -> 554
  }

  if (PINB & (1 << PINB4) || Mode > 0) {  // if (digitalRead(4) == HIGH) or we in show time mode
    Mode = Time(Mode);                    // Show time on LED in binary format
  }

  WDTCR |= (1 << WDTIE);
}

int main() {
  ADCSRA &= ~(1 << ADEN);  //Disable ADC
  ACSR = (1 << ACD);       //Disable the analog comparator
  // Set up Port B as Input
  DDRB = 0b01111;  // Use 470 ohm resistor per LED
  wdt_reset();
  wdt_enable(WDTO_500MS);  // Set watchdog timer to trigger every 500 ms
  WDTCR |= (1 << WDTIE);   // Set watchdog timer in interrupt mode
  sei();                   // Enable global interrupts

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  while (1) {
    sleep_enable();
    sleep_cpu();
  }
  return 0;
}

uint8_t Time(uint8_t currState) {
  uint8_t LEDValue = 0;
  switch (currState) {  //Final state machine
    case 0:
      if (!MSec) {  // protect divide by zero(if MSec not = 0), thanks ChatGPT
        Hours = 0;
      } else {
        Hours = MSec / 3600000;  // 3600 sec per hour * 1000(ms)
      }

      if (Hours == 0) {
        LEDValue = 12;  //
      } else {
        LEDValue = Hours;
      }
      // DDRB = 0b1111;  // Turn on ouptuts

      currState = 1;  // Next state
      break;

    case 1:
      LEDValue = 0;
      currState = 2;
      break;

    case 2:
      if (!MSec) {  // protect divide by zero, thanks ChatGPT
        Minutes = 0;
      } else {
        Minutes = ((MSec / 60000) % 60) / 5;  // 60 Seconds per minutes(5 min to discrete)
      }

      LEDValue = Minutes;

      currState = 3;
      break;

    case 3:
      LEDValue = 0;
      currState = 0;
      break;

    default:
      return -1;  // some wrong
  }

  ledOn(LEDValue);  // Set Port B to LEDValue
  return currState;
}

void ledOn(byte led) {  // Charlieplexing
  switch (led) {

    case 1:
      DDRB = 0b0011;
      PORTB = 0b0001;
      break;

    case 2:
      DDRB = 0b0011;
      PORTB = 0b0010;
      break;

    case 3:
      DDRB = 0b0110;
      PORTB = 0b0010;
      break;

    case 4:
      DDRB = 0b0110;
      PORTB = 0b0100;
      break;

    case 5:
      DDRB = 0b1100;
      PORTB = 0b0100;
      break;

    case 6:
      DDRB = 0b1100;
      PORTB = 0b1000;
      break;

    case 7:
      DDRB = 0b0101;
      PORTB = 0b0100;
      break;

    case 8:
      DDRB = 0b0101;
      PORTB = 0b0001;
      break;

    case 9:
      DDRB = 0b1010;
      PORTB = 0b1000;
      break;

    case 10:
      DDRB = 0b1010;
      PORTB = 0b0010;
      break;

    case 11:
      DDRB = 0b1001;
      PORTB = 0b1000;
      break;

    case 12:
      DDRB = 0b1001;
      PORTB = 0b0001;
      break;

    default:
      DDRB = 0b0000;
      PORTB = 0b0000;
  }
}
