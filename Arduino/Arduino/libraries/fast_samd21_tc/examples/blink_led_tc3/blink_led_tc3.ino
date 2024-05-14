/*
  fast_samd21_tc3 example

  This is a basic example. It blinks one LED/pin.

  Author: Daniel Mohr
  Date: 2022-09-05
*/

#include <fast_samd21_tc3.h>

#define LED_PIN 0

void TC3_Handler(void) {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  TC3->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  fast_samd21_tc3_configure(100000); // starts the timer/trigger with 0.1 s
}

void loop() {
}
