/*
  fast_samd21_tc4_tc5 example

  This is an example to blink one LED/pin very fast.
  It uses a 32-bit counter.

  Author: Daniel Mohr
  Date: 2023-10-06
*/

#include <fast_samd21_tc4_tc5.h>

#define LED_PIN 0

#define REGTYPE uint32_t
volatile REGTYPE *trigger_output_register =
  portOutputRegister(digitalPinToPort(LED_PIN));
const REGTYPE trigger_pin_mask = digitalPinToBitMask(LED_PIN);
const REGTYPE not_trigger_pin_mask = ~trigger_pin_mask;

void TC4_Handler(void) {
  *trigger_output_register =
    (*trigger_output_register & not_trigger_pin_mask) |
    ~(*trigger_output_register & trigger_pin_mask);
  TC4->COUNT32.INTFLAG.bit.MC0 = 1; // clears the interrupt
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  fast_samd21_tc4_tc5_configure(2); // starts the timer/trigger with 2 us
}

void loop() {
}
