/*
  fast_samd21_tc5 example

  This examples uses micros() to measure the timing.
  If you have a small TC5_Handler routine, you can also use a timer
  with 4 us = 4e-6 s. But micros() needs too long here.

  You can do a much more precise measurement using an oscilloscope
  and flipping digital IO (blink LED). Doing this is demonstrated in
  'extras/fast_blink_led_tc5_micros_measure/'.

  Here we skip blinking a LED.

  Author: Daniel Mohr
  Date: 2022-09-19
*/

#include <fast_samd21_tc5.h>

/* serial interface */
#define SERIAL_BAUD_RATE 115200 // baud = bits per second
#define SERIAL_TIMEOUT 1000 // timeout in 1e-6 seconds

// storing data here in the stack is faster
// than storing them using 'static' flag in the function
unsigned long start_microstime = 0;
unsigned long stop_microstime = 0;
volatile uint8_t it = 0;
volatile unsigned long duration = 0;

void TC5_Handler(void) {
  if (it++ == 0) {
    stop_microstime = micros();
    duration = stop_microstime - start_microstime;
    start_microstime = stop_microstime;
  }
  TC5->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
}

void setup() {
  // set serial communication:
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.setTimeout(SERIAL_TIMEOUT);
  while (!Serial);
  fast_samd21_tc5_configure(100000); // starts the timer/trigger with 0.1 s
}

void loop() {
  Serial.println("\nloop starts");
  Serial.println("every measurement is an average of 255 runs");
  static unsigned long mtime;
  fast_samd21_tc5_configure(10000); // starts the timer/trigger with 10 ms
  it = 0; start_microstime = micros(); duration = 0;
  Serial.println("timer runs with 10 ms");
  while (duration == 0) delay(1);
  for (unsigned short i = 0; i < 3; i++) {
    delay(2560);
    mtime = millis();
    Serial.print(" mtime: ");
    Serial.print(mtime);
    Serial.print(" microstime_delta: ");
    Serial.print(((double) duration) / 255);
    Serial.println(" us");
  }
  fast_samd21_tc5_configure(1000); // starts the timer/trigger with 1 ms
  it = 0; start_microstime = micros(); duration = 0;
  Serial.println("timer runs with 1 ms");
  while (duration == 0) delay(1);
  for (unsigned short i = 0; i < 3; i++) {
    delay(256);
    mtime = millis();
    Serial.print(" mtime: ");
    Serial.print(mtime);
    Serial.print(" microstime_delta: ");
    Serial.print(((double) duration) / 255);
    Serial.println(" us");
  }
  fast_samd21_tc5_configure(100); // starts the timer/trigger with 100 us
  it = 0; start_microstime = micros(); duration = 0;
  Serial.println("timer runs with 100 us");
  while (duration == 0) delay(1);
  for (unsigned short i = 0; i < 3; i++) {
    delay(26);
    mtime = millis();
    Serial.print(" mtime: ");
    Serial.print(mtime);
    Serial.print(" microstime_delta: ");
    Serial.print(((double) duration) / 255);
    Serial.println(" us");
  }
  fast_samd21_tc5_configure(10); // starts the timer/trigger with 10 us
  it = 0; start_microstime = micros(); duration = 0;
  Serial.println("timer runs with 10 us");
  while (duration == 0) delay(1);
  for (unsigned short i = 0; i < 3; i++) {
    delay(3);
    mtime = millis();
    Serial.print(" mtime: ");
    Serial.print(mtime);
    Serial.print(" microstime_delta: ");
    Serial.print(((double) duration) / 255);
    Serial.println(" us");
  }
  fast_samd21_tc5_configure(5); // starts the timer/trigger with 5 us
  it = 0; start_microstime = micros(); duration = 0;
  Serial.println("timer runs with 5 us");
  while (duration == 0) delay(1);
  for (unsigned short i = 0; i < 3; i++) {
    delay(2);
    mtime = millis();
    Serial.print(" mtime: ");
    Serial.print(mtime);
    Serial.print(" microstime_delta: ");
    Serial.print(((double) duration) / 255);
    Serial.println(" us");
  }
  fast_samd21_tc5_configure(2.5); // starts the timer/trigger with 2.5 us
  it = 0; start_microstime = micros(); duration = 0;
  Serial.println("timer runs with 2.5 us");
  while (duration == 0) delay(1);
  for (unsigned short i = 0; i < 3; i++) {
    delay(1);
    mtime = millis();
    Serial.print(" mtime: ");
    Serial.print(mtime);
    Serial.print(" microstime_delta: ");
    Serial.print(((double) duration) / 255);
    Serial.println(" us");
  }
}
