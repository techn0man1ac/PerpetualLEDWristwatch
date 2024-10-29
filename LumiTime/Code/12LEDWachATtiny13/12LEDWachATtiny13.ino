/*
  Tech01 12 LED ATtiny "Perpetual" Wristwatch By Serhii Trush with MIT License.
  https://github.com/techn0man1ac/PerpetualLEDWristwatch

  The simple and "Perpetual" WristWatch(project code name "LumiTime") with 12 red LEDs, it has LIR2430 Li-Ion battery and 12 solar cells,
  based on a simple Microchip microcontroller ATtiny13. It is fully open source (firmware and hardware).

  Sketch uses 988 bytes (96%) of program storage space. Maximum is 1024 bytes.
  Global variables use 13 bytes (20%) of dynamic memory, leaving 53 bytes for local variables. Maximum is 64 bytes.

  By Tech01 labs 2024.
*/

#define F_CPU 1200000UL  // CPU frequency set to 1.2 MHz
#include <avr/io.h>      // AVR I/O library for ATtiny13
#include <avr/wdt.h>     // Watchdog Timer library
#include <avr/sleep.h>   // Sleep mode library
#include <avr/power.h>   // Power management library
#include <avr/interrupt.h>  // Interrupt library

// Constants for time calculations
#define msPerCycleReal 560    // Real cycle duration in milliseconds (approximately 500ms)
#define MsIn12Hours 43200000  // Total milliseconds in 12 hours
#define ButtonPin PINB4       // PB4 is the button pin

// Global variables to store time and mode states
uint8_t Hours = 9;            // Default hour setting (0..11)
uint8_t Minutes = 18;         // Default minute setting (5..55)
volatile uint8_t InterruptWDT = 0; // Watchdog timer interrupt flag
volatile uint32_t MilliSeconds = 0;   // Milliseconds since the start of the current cycle
uint8_t FiveMinutesDiscrete = 0;  // Discretized minutes in steps of five (5,10..55)
uint8_t MinutesRemainder = 0;   // Remaining minutes after discretization (0..4)
uint8_t MinutesOnLED = 0;     // LED index for displaying time
uint8_t Mode = 0;             // Current mode state
uint8_t NextMode = 0;         // Next mode to transition into
uint8_t ButtonPressCount = 0; // Count of button presses

// Function prototypes for readability and organization
void initHardware();
uint8_t ButtonPress();        // Check if the button is pressed
uint8_t ShowTime(uint8_t);    // Display time on LEDs based on current mode
void ledOn(uint8_t);          // Turn on a specific LED (not used in this code)
void Up5Minutes();            // Increment time by 5 minutes
void TimeToMs();              // Convert hours and minutes to milliseconds

// Watchdog Timer interrupt service routine
ISR(WDT_vect) {
  InterruptWDT = 1;         // Set the watchdog timer interrupt flag
  WDTCR |= (1 << WDTIE);    // Re-enable watchdog interrupt after handling
}

int main() {
  initHardware();           // Initialize hardware components
  TimeToMs();               // Calculate default time in milliseconds
  sei();                    // Enable global interrupts
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Set sleep mode to power down

  while (1) {               // Main loop
    if (InterruptWDT == 1) {
      InterruptWDT = 0;  // Clear the watchdog timer interrupt flag
      uint8_t Button = ButtonPress();  // Check button state
      MilliSeconds += msPerCycleReal;          // Increment time by cycle duration

      if (MilliSeconds >= MsIn12Hours) {       // Handle overflow after 12 hours
        MilliSeconds -= MsIn12Hours;
      }

      if (Button || Mode > 0) {
        Mode = ShowTime(Mode);        // Display current time on LEDs
        ButtonPressCount += 1;

        if (ButtonPressCount >= 40 && Mode == 1) { // Increment time by 5 minutes after long press
          Up5Minutes();
        }
      } else {
        ButtonPressCount = 0;         // Reset button press count when not in use
      }

    }
    sleep_enable();                       // Enable sleep mode
    sleep_cpu();                          // Enter sleep mode
  }
  return 0;
}

void initHardware() {
  ADCSRA &= ~(1 << ADEN);   // Disable Analog-to-Digital Converter (ADC)
  ACSR = (1 << ACD);        // Disable analog comparator
  DDRB = 0b00000;           // Set Port B as input with pull-up resistors for LEDs
  wdt_reset();              // Reset watchdog timer
  wdt_enable(WDTO_500MS);   // Enable watchdog timer to trigger every 500ms
  WDTCR |= (1 << WDTIE);    // Enable watchdog interrupt
}

uint8_t ButtonPress() {
  return PINB & (1 << ButtonPin);  // Check if the button is pressed
}

uint8_t ShowTime(uint8_t currState) {
  uint8_t LEDValue = 0;
  switch (currState) {  // State machine for displaying time on LEDs
    case 0:
      if (!MilliSeconds) {  // Handle division by zero
        Hours = 0;
        Minutes = 0;
        FiveMinutesDiscrete = 0;
        MinutesRemainder = 0;
      } else {
        Hours = MilliSeconds / 3600000;          // Calculate hours from milliseconds
        Minutes = (MilliSeconds / 60000) % 60;   // Calculate minutes from milliseconds
        FiveMinutesDiscrete = (Minutes / 5) * 5;  // Discretize minutes to nearest five
        MinutesOnLED = FiveMinutesDiscrete / 5;
        MinutesRemainder = Minutes - FiveMinutesDiscrete;
      }

      LEDValue = Hours;
      currState = 2;  // No LEDs on
      NextMode = 1;   // Transition to showing minutes
      break;

    case 1:
      LEDValue = MinutesOnLED;  // Display discretized minutes

      if (MinutesRemainder > 1) {
        MinutesRemainder -= 1;
        currState = 2;
        NextMode = 1;
      } else {
        MinutesRemainder = 0;
        currState = 2;
        NextMode = 0;  // Transition to no LEDs on
      }
      break;

    case 2:
      LEDValue = 13;   // Turn off all LEDs
      currState = NextMode;
      break;

    default:
      return 0;  // Invalid state
  }

  DisplayOnLED(LEDValue);  // Set Port B to display the current time on LEDs
  return currState;
}

void DisplayOnLED(uint8_t led) {  // Charlieplexing logic for LED display
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
    case 12: DDRB = 0b1001; PORTB = 0b0001; break; // Same as case 0
    case 13: DDRB = 0b0000; PORTB = 0b0000; break; // Turn off all LEDs
  }
}

void Up5Minutes() {
  Minutes = FiveMinutesDiscrete;   // Reset minutes to the nearest five
  TimeToMs();                      // Recalculate milliseconds from hours and minutes
  MilliSeconds += 300000;                  // Add 5 minutes in milliseconds
  FiveMinutesDiscrete += 5;        // Add 5 minutes to show LEDs
  MinutesRemainder = 0;
}

void TimeToMs() {
  MilliSeconds = Hours * 3600000 + Minutes * 60000;  // Convert hours and minutes to total milliseconds
}
