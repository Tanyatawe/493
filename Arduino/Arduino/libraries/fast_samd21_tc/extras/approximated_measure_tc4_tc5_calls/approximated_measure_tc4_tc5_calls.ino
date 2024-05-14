/*
  fast_samd21_tc4_tc5 example

  This example tries to measure the time between calling the interrupts.
  Using 'micros' inside an interrupt is not recommended. But here for this
  test it seems to work.

  Author: Daniel Mohr
  Date: 2023-10-05
*/

#include <fast_samd21_tc4_tc5.h>

#define TRIGGERUS 2345678
#define CALCIT 2

// storing data here in the stack is faster
// than storing them using 'static' flag in the function
unsigned long start_microstime = 0;
unsigned long stop_microstime = 0;
volatile uint8_t it = 0;
volatile unsigned long duration = 0;

void TC4_Handler(void) {
  if (it++ == 1) {
    stop_microstime = micros();
    duration = stop_microstime - start_microstime;
    start_microstime = stop_microstime;
    it = 0;
  }
  TC4->COUNT32.INTFLAG.bit.MC0 = 1; // clears the interrupt
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1000);
  while (!Serial);
  Serial.println("######");
  fast_samd21_tc4_tc5_configure(TRIGGERUS);
  delay(3*TRIGGERUS/1000);
}

void loop() {
    delay(max(3, CALCIT*TRIGGERUS/1000));
    static unsigned long mtime;
    mtime = millis();
    Serial.print("mtime: ");
    Serial.print(mtime);
    Serial.print(" microstime_delta: ");
    Serial.print(((double) duration) / CALCIT);
    Serial.println(" us");
}
