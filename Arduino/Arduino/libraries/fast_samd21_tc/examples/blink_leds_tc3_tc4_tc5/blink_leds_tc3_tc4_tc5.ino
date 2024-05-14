/*
  fast_samd21_tc5 example

  3 LEDs/pins a switched on and off by 3 interrupts with frequencies
  relatively prime. Each interrupt handles their own LED/pin.

  Author: Daniel Mohr
  Date: 2022-09-15
*/

#include <fast_samd21_tc.h>

#define LED_PIN0 0
#define LED_PIN1 1
#define LED_PIN2 2

void TC3_Handler(void) {
  digitalWrite(LED_PIN0, !digitalRead(LED_PIN0));
  TC3->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
}

void TC4_Handler(void) {
  digitalWrite(LED_PIN1, !digitalRead(LED_PIN1));
  TC4->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
}

void TC5_Handler(void) {
  digitalWrite(LED_PIN2, !digitalRead(LED_PIN2));
  TC5->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
}

void setup() {
  pinMode(LED_PIN0, OUTPUT);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  fast_samd21_tc3_configure(100000); // starts the timer/trigger with 0.100 s
  fast_samd21_tc4_configure(105000); // starts the timer/trigger with 0.105 s
  fast_samd21_tc5_configure(110000); // starts the timer/trigger with 0.110 s
}

void loop() {
}
