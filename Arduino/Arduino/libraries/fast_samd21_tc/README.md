---
author: Daniel Mohr
date: 2023-10-06
url: https://gitlab.com/ug-cp/fast_samd21_tc
license: BSD 3-Clause License
---

# fast_samd21_tc -- TC5, TC4 or TC3 timer library for SAMD21 microcontrollers

[[_TOC_]]

This library allows using the TC3_Handler, TC4_Handler and the TC5_Handler
routine triggered by the 16-bit Timer/Counter TC3, TC4 or TC5 on SAMD21
(e. g. Arduino MKRZERO). It is also possible to use TC4 and TC5 as a
32-bit Timer/Counter.

It is possible to trigger very fast (a few microseconds, e. g. 4 us and less,
depending on the runtime of the handler routine/function).
You can use fraction numbers as interval (e. g. 2.5 us).

Keep in mind, TC4 and TC5 are not completely independent.
But you can use TC4 and TC5 together as a 32-bit counter.
This 32-bit counter allows to call the interrupt with a time between calls
up to about 1 day with a given precision of up to microseconds -- of course the
normal clock on an Arduino has not such a precision, but at least the software
is doing its job correct.

* home: [gitlab.com/ug-cp/fast_samd21_tc](https://gitlab.com/ug-cp/fast_samd21_tc)
* mirror: [github.com/ug-cp/fast_samd21_tc](https://github.com/ug-cp/fast_samd21_tc)

* Arduino Library Manager: [arduino.cc/reference/en/libraries/fast_samd21_tc](https://www.arduino.cc/reference/en/libraries/fast_samd21_tc/)

In contrast to
[SAMD_TimerInterrupt](https://www.arduino.cc/reference/en/libraries/samd_timerinterrupt/)
or
[TimerInterrupt_Generic](https://www.arduino.cc/reference/en/libraries/timerinterrupt_generic/)
this library fast_samd21_tc does not interfere with the
[Ethernet library](https://www.arduino.cc/reference/en/libraries/ethernet/)
on an [Arduino MKR Zero](https://docs.arduino.cc/hardware/mkr-zero) using
[MKR ETH Shield](https://docs.arduino.cc/hardware/mkr-eth-shield).

## Install

To use/install this library, you can open the Library Manager in the
Arduino IDE and install it from there. This is the recommended way.

Or if you use [Arduino CLI](https://arduino.github.io/arduino-cli/) the
recommended way is `arduino-cli lib install fast_samd21_tc`

Or to install this library use `Add .ZIP Library ...` in your Arduino IDE
to add the zip archive `fast_samd21_tc-main.zip` you can download from
this repository. Or you can use archives from
[latest release](https://gitlab.com/ug-cp/fast_samd21_tc/-/releases/permalink/latest).

Or just copy the content of this repository to your Arduino libraries, e. g.:

```shell
git clone https://gitlab.com/ug-cp/fast_samd21_tc.git ~/Arduino/libraries/fast_samd21_tc
```

## Usage

You can use a 16-bit or a 32-bit Counter with this library.

### 16-bit Counter

You can include the whole library `#include <fast_samd21_tc.h>` and choose
what you use.

By defining `fast_samd21_no_tc3` you can skip this part of the library.
Defining `fast_samd21_no_tc4` or `fast_samd21_no_tc5` skips the
corresponding part.

Or you can include the TC3 specific part `#include <fast_samd21_tc3.h>`,
the TC4 specific part `#include <fast_samd21_tc5.h>` or
the TC5 specific part `#include <fast_samd21_tc5.h>`.

For example instead of `#include <fast_samd21_tc5.h>` you could also use:

```c
#define fast_samd21_no_tc3
#define fast_samd21_no_tc4
#include <fast_samd21_tc.h>
```

The 32-bit counter is not imported with the whole library
`#include <fast_samd21_tc.h>`. To use the 32-bit counter you have to import
`#include <fast_samd21_tc4_tc5.h>` and to provide the function `TC4_Handler`.
For the 32-bit counter look at the
[examples `blink_led_tc4_tc5.ino`](examples/blink_led_tc4_tc5/blink_led_tc4_tc5.ino).

We will use here only the TC5 specific part `#include <fast_samd21_tc5.h>`.
For TC3, TC4 or all look at the [examples](examples).

You have to provide the `TC5_Handler` routine, e. g.:

```c
void TC5_Handler(void) {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  TC5->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
}
```

To set up the trigger you only need to configure, e. g.:

```c
#include <fast_samd21_tc5.h>
void setup() {
  fast_samd21_tc5_configure(8); // 8 us = 8e-6 s
}
```

You can stop, start, disable and reconfigure the trigger, e. g.:

```c
void loop() {
  fast_samd21_tc5_stop();
  delay(1000);
  fast_samd21_tc5_configure(16);
  delay(1000);
  fast_samd21_tc5_disable();
  delay(1000);
  fast_samd21_tc5_start();
  delay(1000);
  fast_samd21_tc5_configure(32);
  delay(1000);
}
```

The TC5_Handler routine can call different functions. Since a function call
needs some time, you have to implement this on your own.
But here is an example:

```c
#include <fast_samd21_tc5.h>
void blink1(void) {
  digitalWrite(1, !digitalRead(1));
}
void blink2(void) {
  digitalWrite(2, !digitalRead(2));
}
void (* callback)() = blink1;
void TC5_Handler(void) {
  callback();
  TC5->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
}
void setup() {
  fast_samd21_tc5_configure(1000000); // 1 s
  fast_samd21_tc5_start();
}
void loop() {
  callback = blink1;
  delay(1000);
  callback = blink2;
  delay(1000);
}
```

### 32-bit Counter

The 32-bit Counter works very similar to the 16-bit one.
But in the background the 32-bit Counter needs the 2 timers TC4 and TC5.
The TC4 timer works as a master and you have to provide the
`TC4_Handler` routine. Here is an example:

```c
#include <fast_samd21_tc4_tc5.h>
void TC4_Handler(void) {
  digitalWrite(1, !digitalRead(1));
  TC4->COUNT32.INTFLAG.bit.MC0 = 1; // clears the interrupt
}
void setup() {
  pinMode(1, OUTPUT);
  fast_samd21_tc4_tc5_configure(12345678); // starts timer with 12.345678 s
}
void loop() {
}
```

## Tested

It was tested on SAMD21 (Arduino MKR1000, Arduino MKR Zero and
Arduino MKR WiFi 1010).
But should work on other Arduino MKR or SAMD21 boards as well.

Fast switching a pin
(using [blink_led_tc5.ino](examples/blink_led_tc5/blink_led_tc5.ino))
and measure the period T with an oscilloscope gives something like
(values in 1 us = 1e-6 s):

| set interval | expected T | mean T | min. T | max. T | std T |
| ------ | ------ | ------ | ------ | ------ | ------ |
| 2 | 4 | 8.04 | 7.99 | 8.05 | 0.024 |
| 4 | 8 | 8.06 | 7.99 | 8.08 | 0.030 |
| 8 | 16 | 16.02 | 15.99 | 16.15 | 0.053 |
| 16 | 32 | 31.98 | 31.98 | 32.04 | 0.006 |
| 32 | 64 | 63.98 | 63.98 | 64.04 | 0.012 |
| 64 | 128 | 127.96 | 127.87 | 127.96 | 0.007 |
| 128 | 256 | 255.87 | 255.75 | 255.96 | 0.023 |
| 256 | 512 | 511.85 | 511.71 | 511.92 | 0.038 |

So, we can really fast blink with a period of 8 us = 8e-6 s (125 kHz).
This is an interrupt every 4 us = 4e-6 s (250 kHz).

Consider that a 4 us trigger is possible, but  at the edge of feasibility.
Therefore, if you look very closely, you will find that some trigger events
are missing.

Fast switching a pin
(using [blink_led_tc3.ino](examples/blink_led_tc3/blink_led_tc3.ino))
and measure the period T with an oscilloscope gives something like
(values in 1 us = 1e-6 s):

| set interval | expected T | mean T | min. T | max. T | std T |
| ------ | ------ | ------ | ------ | ------ | ------ |
| 4 | 8 | 8.05 | 7.99 | 8.07 | 0.036 |
| 8 | 16 | 16.00 | 15.98 | 16.15 | 0.037 |
| 16 | 32 | 31.98 | 31.98 | 31.99 | 0.003 |
| 32 | 64 | 63.98 | 63.98 | 64.02 | 0.012 |
| 64 | 128 | 127.95 | 127.92 | 127.96 | 0.016 |
| 128 | 256 | 255.96 | 255.96 | 255.96 | 0.010 |
| 256 | 512 | 511.76 | 511.71 | 511.92 | 0.056 |

Sorry, I do not know why this measurement is not as good as for TC5.

For faster switching the used `digitalWrite()` and `digitalRead()` are too
slow. To achieve faster switching we can use the registers directly. This
is done in the following measurements.

Fast switching a pin (using
[fast_blink_led_tc3.ino](examples/fast_blink_led_tc3/fast_blink_led_tc3.ino))
and measure the period T with an oscilloscope gives something like
(values in 1 us = 1e-6 s):

| set interval | expected T | mean T | min. T | max. T | std T |
| ------ | ------ | ------ | ------ | ------ | ------ |
| 1.3 | 2.6 | 2.6 | 2.58 | 2.66 | 0.030 |
| 2 | 4 | 4.01 | 3.99 | 4.04 | 0.022 |
| 4 | 8 | 8.02 | 7.99 | 8.07 | 0.016 |
| 8 | 16 | 15.99 | 15.99 | 15.99 | 0.006 |
| 16 | 32 | 31.98 | 31.98 | 31.99 | 0.003 |
| 32 | 64 | 63.96 | 63.93 | 63.96 | 0.005 |
| 64 | 128 | 127.92 | 127.92 | 127.96 | 0.010 |
| 128 | 256 | 255.96 | 255.96 | 256.06 | 0.020 |
| 256 | 512 | 511.87 | 511.71 | 511.92 | 0.054 |

Fast switching a pin (using
[fast_blink_led_tc4.ino](examples/fast_blink_led_tc4/fast_blink_led_tc4.ino))
and measure the period T with an oscilloscope gives something like
(values in 1 us = 1e-6 s):

| set interval | expected T | mean T | min. T | max. T | std T |
| ------ | ------ | ------ | ------ | ------ | ------ |
| 1.3 | 2.6 | 2.6 | 2.58 | 2.67 | 0.040 |
| 2 | 4 | 4.01 | 3.99 | 4.06 | 0.025 |
| 4 | 8 | 8.03 | 7.99 | 8.04 | 0.015 |
| 8 | 16 | 15.99 | 15.99 | 15.99 | 0.001 |
| 16 | 32 | 31.98 | 31.98 | 31.99 | 0.003 |
| 32 | 64 | 63.96 | 63.91 | 64.00 | 0.007 |
| 64 | 128 | 127.92 | 127.92 | 127.96 | 0.010 |
| 128 | 256 | 255.92 | 255.85 | 255.96 | 0.050 |
| 256 | 512 | 511.77 | 511.71 | 511.92 | 0.060 |

Fast switching a pin (using
[fast_blink_led_tc5.ino](examples/fast_blink_led_tc5/fast_blink_led_tc5.ino))
and measure the period T with an oscilloscope gives something like
(values in 1 us = 1e-6 s):

| set interval | expected T | mean T | min. T | max. T | std T |
| ------ | ------ | ------ | ------ | ------ | ------ |
| 1.3 | 2.6 | 2.6 | 2.58 | 2.68 | 0.035 |
| 2 | 4 | 4.01 | 3.99 | 4.06 | 0.025 |
| 4 | 8 | 8.02 | 7.99 | 8.03 | 0.017 |
| 8 | 16 | 15.99 | 15.99 | 15.99 | 0.002 |
| 16 | 32 | 31.99 | 31.99 | 32.04 | 0.006 |
| 32 | 64 | 63.96 | 63.96 | 63.98 | 0.005 |
| 64 | 128 | 127.97 | 127.92 | 128.00 | 0.018 |
| 128 | 256 | 255.89 | 255.85 | 255.96 | 0.046 |
| 256 | 512 | 511.92 | 511.71 | 511.92 | 0.080 |

Fast switching a pin (using
[fast_blink_led_tc4_tc5.ino](examples/fast_blink_led_tc4_tc5/fast_blink_led_tc4_tc5.ino))
using a 32-bit counter and measure the period T with an oscilloscope gives
something like (values in 1 us = 1e-6 s or explicitly given):

| set interval | expected T | mean T | min. T | max. T | std T |
| ------ | ------ | ------ | ------ | ------ | ------ |
| 1.3 | 2.6 | 2.6 | 2.58 | 2.70 | 0.033 |
| 2 | 4 | 4.03 | 3.99 | 4.07 | 0.030 |
| 4 | 8 | 8.02 | 7.99 | 8.03 | 0.017 |
| 8 | 16 | 15.99 | 15.99 | 15.99 | 0.002 |
| 16 | 32 | 31.99 | 31.99 | 32.01 | 0.007 |
| 32 | 64 | 63.99 | 63.98 | 64.02 | 0.012 |
| 64 | 128 | 127.94 | 127.92 | 127.96 | 0.018 |
| 128 | 256 | 255.91 | 255.85 | 255.96 | 0.046 |
| 256 | 512 | 512.03 | 512.02 | 512.22 | 0.028 |
| 256 | 512 | 512.03 | 512.02 | 512.22 | 0.028 |
| 524288 | 1048576 | 1.04 s | 1.04 s | 1.04 s | 0 s |
| 1048576 | 2097152 | 2.09 s | 2.09 s | 2.09 s | 0 s |
| 2097152 | 4194304 | 4.19 s | 4.19 s | 4.19 s | 0 s |

The last 3 measurements are presented to show it is really a 32-bit counter.
The precision of the measurement is only seconds.

## Examples

In the folder [examples](examples) you can find some examples.

## License, Copyright: BSD 3-Clause License

Copyright (c) 2022, 2023 Daniel Mohr and
                         University of Greifswald, Institute of Physics,
                         Colloidal (dusty) plasma group

All rights reserved.

see: [LICENSE](LICENSE)
