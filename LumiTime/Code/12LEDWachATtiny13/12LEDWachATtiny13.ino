/*
  Tech01 12 LED ATtiny "Perpetual" Wristwatch By Serhii Trush with MIT License.
  https://github.com/techn0man1ac/PerpetualLEDWristwatch

  The simple and "Perpetual" WristWatch(project code name "LumiTime") with 12 red LEDs, it have LIR2430 Li-Ion battery and 12 solar cells,
  based on a simple Microchip microcontroller ATtiny13. It full open source(firmware and hardware).

  Sketch uses 988 bytes (96%) of program storage space. Maximum is 1024 bytes.
  Global variables use 11 bytes (17%) of dynamic memory, leaving 53 bytes for local variables. Maximum is 64 bytes.

  By Tech01 labs 2024.
*/

#define F_CPU 1200000UL  // 1.2 MHz CPU mode(low_fuses=0x6A high_fuses=0xFF - defalt fuses)
#include <avr/io.h>      // AVR I/O library for ATtiny13
#include <avr/wdt.h>     // Watchdog Timer library
#include <avr/sleep.h>   // Sleep mode library
#include <avr/power.h>   // Power management library
#include <avr/interrupt.h>  // Interrupt library

// Constants for time calculations
#define msPerCycleReal 560    // it's mean 500 ms in real life
#define MsIn12Hours 43200000  // 43200000 ms -> 43200 sec = 12h
#define ButtonPin PINB4 // PB4 button pin

// Global variables to store the current time and mode
uint8_t Hours = 11;    // <- Set defalt time in hours here(0..11)
uint8_t Minutes = 35;  // <- Set defalt time in minutes here(5..55)
volatile uint32_t MSec = 0;  // 33300000 is 09:15
uint8_t MinutesDiscreteFive = 0;  // 5,10..55
uint8_t FourMinutesCap = 0;  // 0..4, like 49 minutes -> 45 + 4
uint8_t Mode = 0;
uint8_t NextMode = 0;
uint8_t ButtonPressCount = 0;

// Function prototypes for readability
void initHardware();
uint8_t ButtonPress();
uint8_t ShowTime(uint8_t);
void ledOn(uint8_t);
void Up5Minutes();
void TimeToMs();

// If button not press code add "tick". Iteration time is 100 nS
ISR(WDT_vect) {
  uint8_t Button = ButtonPress();

  MSec += msPerCycleReal;  // 500 ms per cycle

  if (MSec >= MsIn12Hours) {
    MSec = MSec - MsIn12Hours;  // increment MSec value compensation -> 43199999 + 555 = 43200554 -> 554
  }

  if (Button || Mode > 0) {  // if (digitalRead(4) == HIGH) or we in show time mode
    Mode = ShowTime(Mode);                    // Show time on LED in binary format

    ButtonPressCount += 1;

    if (ButtonPressCount >= 40 && Mode == 1) { // Same like 10 second button press
      Up5Minutes();
    }

  } else {
    ButtonPressCount = 0;
  }

  // Enable watchdog interrupt
  WDTCR |= (1 << WDTIE);
}

int main() {
  initHardware();

  TimeToMs(); // Calculate defalt time

  sei(); // Enable global interrupts
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  while (1) {
    sleep_enable();
    sleep_cpu();
  }
  return 0;
}

void initHardware() {
  ADCSRA &= ~(1 << ADEN);  // Disable ADC
  ACSR = (1 << ACD);       // Disable analog comparator
  DDRB = 0b00000;          // Set Port B as input with 470 ohm resistor per LED
  wdt_reset();
  wdt_enable(WDTO_500MS);  // Set watchdog timer to trigger every 500 ms
  WDTCR |= (1 << WDTIE);   // Enable watchdog interrupt
}

uint8_t ButtonPress() {
  return PINB & (1 << ButtonPin);
}

uint8_t ShowTime(uint8_t currState) {
  uint8_t LEDValue = 0;
  switch (currState) {  //Final state machine
    case 0:
      if (!MSec) {  // protect divide by zero(if MSec not = 0), thanks ChatGPT
        Hours = 0;
        Minutes = 0;
        MinutesDiscreteFive = 0;
        FourMinutesCap = 0;
      } else {
        Hours = MSec / 3600000;  // 3600 sec per hour * 1000(ms)
        Minutes = (MSec / 60000) % 60;
        MinutesDiscreteFive = (Minutes / 5) * 5; // 47 -> 45
        FourMinutesCap = Minutes - MinutesDiscreteFive;
      }

      LEDValue = Hours;
      currState = 2;  // No leds
      NextMode = 1; // Show minutes
      break;

    case 1:
      LEDValue = MinutesDiscreteFive / 5; // 45 min -> LED number 9

      if (FourMinutesCap > 1) {
        FourMinutesCap -= 1;
        currState = 2;
        NextMode = 1;
      } else {
        FourMinutesCap = 0;
        currState = 2;
        NextMode = 0;
        break;
      }

      break;

    case 2:
      LEDValue = 13; // turn off all LED
      currState = NextMode;
      break;

    default:
      return 3;  // some wrong
  }

  DisplayOnLED(LEDValue);  // Set Port B to LEDValue
  return currState;
}

void DisplayOnLED(uint8_t led) {  // Charlieplexing
  switch (led) {
    case 0: DDRB = 0b1001; PORTB = 0b0001; break;
    case 1: DDRB = 0b0011; PORTB = 0b0001; break;
    case 2: DDRB = 0b0011; PORTB = 0b0010; break;
    case 3: DDRB = 0b0110; PORTB = 0b0010; break;
    case 4: DDRB = 0b0110; PORTB = 0b0100; break;
    case 5: DDRB = 0b1100; PORTB = 0b0100; break;
    case 6: DDRB = 0b1100; PORTB = 0b1000; break;
    case 7: DDRB = 0b0101; PORTB = 0b0100; break;
    case 8: DDRB = 0b0101; PORTB = 0b0001; break;
    case 9: DDRB = 0b1010; PORTB = 0b1000; break;
    case 10: DDRB = 0b1010; PORTB = 0b0010; break;
    case 11: DDRB = 0b1001; PORTB = 0b1000; break;
    case 12: DDRB = 0b1001; PORTB = 0b0001; break; // Same like case 0
    case 13: DDRB = 0b0000; PORTB = 0b0000; break;
  }
}

void Up5Minutes() {
  Minutes = MinutesDiscreteFive; // 47 Minutes -> 45
  TimeToMs();
  MSec += 300000; // Add 5 minutes
  MinutesDiscreteFive += 5;
  FourMinutesCap = 0;
}

void TimeToMs() {
  MSec = Hours * 3600000 + Minutes * 60000;
}
