/*
  fast_samd21_tc5 example

  This is an example to blink one LED/pin very fast.
  Further, using micros() we try to get a measurement.

  Author: Daniel Mohr
  Date: 2022-09-19
*/

#include <fast_samd21_tc5.h>

#define LED_PIN 0

#define REGTYPE uint32_t
volatile REGTYPE *trigger_output_register =
  portOutputRegister(digitalPinToPort(LED_PIN));;
const REGTYPE trigger_pin_mask = digitalPinToBitMask(LED_PIN);
const REGTYPE not_trigger_pin_mask = ~trigger_pin_mask;

// storing data here in the stack is faster
// than storing them using 'static' flag in the function
uint16_t it = 0;
unsigned long start_microstime = 0;
unsigned long stop_microstime = 0;
volatile unsigned long duration = 0;

void TC5_Handler(void) {
  *trigger_output_register =
    (*trigger_output_register & not_trigger_pin_mask) |
    ~(*trigger_output_register & trigger_pin_mask);
  if (it++ == 0) {
    stop_microstime = micros();
    duration = stop_microstime - start_microstime;
    start_microstime = stop_microstime;
  }
  TC5->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial);
  fast_samd21_tc5_configure(2); // starts the timer/trigger with 2 us
}

void loop() {
  while (duration == 0) {
    delay(1);
  };
  Serial.print("duration: ");
  Serial.print(((double) duration) / 65536UL);
  Serial.println(" us");
  duration = 0;
}
