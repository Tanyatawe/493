/*
  Author: Daniel Mohr
  Date: 2022-09-15

  This header file allows using the TC3_Handler, TC4_Handler and TC5_Handler
  routine triggered by the TC3, TC4 and TC5 timer on SAMD21
  (e. g. Arduino MKRZERO).

  You have to provide the TC3_Handler, TC4_Handler or TC5_Handler routine,
  e. g.:

  void TC3_Handler(void) {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  TC3->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
  }

  For more information look at the README.md.

*/

#ifndef fast_samd21_tc_h
#define fast_samd21_tc_h

#ifndef fast_samd21_no_tc3
#include "fast_samd21_tc3.h"
#endif

#ifndef fast_samd21_no_tc4
#include "fast_samd21_tc4.h"
#endif

#ifndef fast_samd21_no_tc5
#include "fast_samd21_tc5.h"
#endif

#endif /* fast_samd21_tc_h */
