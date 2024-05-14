/*
  fast_samd21_tc5 example

  This is a basic example. It blinks one LED/pin.

  Author: Daniel Mohr
  Date: 2022-09-02
*/

#include <fast_samd21_tc5.h>

#define LED_PIN 0

void TC5_Handler(void) {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  TC5->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  fast_samd21_tc5_configure(100000); // starts the timer/trigger with 0.1 s
}

void loop() {
}
