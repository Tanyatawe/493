/*
  fast_samd21_tc5 example

  3 LEDs/pins a switched on and off by 2 interrupts with frequencies
  relatively prime.
  Each interrupt handles their own LED/pin. And both handle the third LED/pin.

  Author: Daniel Mohr
  Date: 2022-09-15
*/

#define fast_samd21_no_tc4
#include <fast_samd21_tc.h>

#define LED_PIN0 0
#define LED_PIN1 1
#define LED_PIN2 2

void TC3_Handler(void) {
  digitalWrite(LED_PIN0, !digitalRead(LED_PIN0));
  digitalWrite(LED_PIN2, !digitalRead(LED_PIN2));
  TC3->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
}

void TC5_Handler(void) {
  digitalWrite(LED_PIN1, !digitalRead(LED_PIN1));
  digitalWrite(LED_PIN2, !digitalRead(LED_PIN2));
  TC5->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
}

void setup() {
  pinMode(LED_PIN0, OUTPUT);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  fast_samd21_tc3_configure(101000); // starts the timer/trigger with 0.101 s
  fast_samd21_tc5_configure(100000); // starts the timer/trigger with 0.1 s
}

void loop() {
}
