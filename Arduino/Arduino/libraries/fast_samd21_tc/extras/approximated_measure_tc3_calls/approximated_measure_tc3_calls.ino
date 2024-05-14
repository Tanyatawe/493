/*
  fast_samd21_tc3 example

  This example tries to measure the time between calling the interrupts.
  Using 'micros' inside an interrupt is not recommended. But here for this
  test it seems to work.

  Author: Daniel Mohr
  Date: 2023-10-05
*/

#include <fast_samd21_tc3.h>

#define TRIGGERUS 10
#define CALCIT 256

// storing data here in the stack is faster
// than storing them using 'static' flag in the function
unsigned long start_microstime = 0;
unsigned long stop_microstime = 0;
volatile uint8_t it = 0;
volatile unsigned long duration = 0;

void TC3_Handler(void) {
  if (it++ == 0) {
    stop_microstime = micros();
    duration = stop_microstime - start_microstime;
    start_microstime = stop_microstime;
  }
  TC3->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1000);
  while (!Serial);
  Serial.println("######");
  fast_samd21_tc3_configure(TRIGGERUS);
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
