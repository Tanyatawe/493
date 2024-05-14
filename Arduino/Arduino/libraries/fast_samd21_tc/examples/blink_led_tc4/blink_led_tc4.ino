/*
  fast_samd21_tc4 example

  This is a basic example. It blinks one LED/pin.

  Author: Daniel Mohr
  Date: 2022-09-15
*/

#include <fast_samd21_tc4.h>

#define LED_PIN 0

void TC4_Handler(void) {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  TC4->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  fast_samd21_tc4_configure(100000); // starts the timer/trigger with 0.1 s
}

void loop() {
}
