/*
  fast_samd21_tc4_tc5 example

  This is a basic example. It blinks one LED/pin with a very long perdiod.
  It uses a 32-bit counter.

  Author: Daniel Mohr
  Date: 2023-10-05
*/

#include <fast_samd21_tc4_tc5.h>

#define LED_PIN 0

void TC4_Handler(void) {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  TC4->COUNT32.INTFLAG.bit.MC0 = 1; // clears the interrupt
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  // starts the timer/trigger with 2.345678 s:
  fast_samd21_tc4_tc5_configure(2345678);
}

void loop() {
}
