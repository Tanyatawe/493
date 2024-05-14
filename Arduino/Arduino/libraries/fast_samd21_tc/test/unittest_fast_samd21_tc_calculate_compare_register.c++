// https://github.com/Arduino-CI/arduino_ci
#include <ArduinoUnitTests.h>

#include <Arduino.h>

#ifndef SystemCoreClock
#if defined(ARDUINO_ARCH_SAMD)
// arduino_ci does not provide platform specific information
// therefore we have to define it here on our own
#define SystemCoreClock 48000000
#endif
#endif

#include <fast_samd21_tc_calculate_compare_register.h>

unittest(fast_samd21_tc_calculate_compare_register) {
  assertEqual((uint32_t) 0,
	      _fast_samd21_tc_calculate_compare_register(0.021, 1));
  assertEqual((uint32_t) 0,
	      _fast_samd21_tc_calculate_compare_register(0.1, 4));
  assertEqual((uint32_t) 1,
	      _fast_samd21_tc_calculate_compare_register(0.1, 2));
  assertEqual((uint32_t) 4,
	      _fast_samd21_tc_calculate_compare_register(0.1, 1));
  assertEqual((uint32_t) 47,
	      _fast_samd21_tc_calculate_compare_register(1, 1));
  assertEqual((uint32_t) 479,
	      _fast_samd21_tc_calculate_compare_register(10, 1));
  assertEqual((uint32_t) 143,
	      _fast_samd21_tc_calculate_compare_register(6, 2));
  assertEqual((uint32_t) 183,
	      _fast_samd21_tc_calculate_compare_register(23, 6));
  uint16_t prescaler;
  uint16_t compare_register;
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register(6,
							&prescaler,
							&compare_register));
  assertEqual(1, prescaler);
  assertEqual(287, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register(23,
							&prescaler,
							&compare_register));
  assertEqual(1, prescaler);
  assertEqual(1103, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register(2000,
							&prescaler,
							&compare_register));
  assertEqual(2, prescaler);
  assertEqual(47999, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register(4000,
							&prescaler,
							&compare_register));
  assertEqual(4, prescaler);
  assertEqual(47999, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register(10000,
							&prescaler,
							&compare_register));
  assertEqual(8, prescaler);
  assertEqual(59999, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register(20000UL,
							&prescaler,
							&compare_register));
  assertEqual(16, prescaler);
  assertEqual(59999, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register(30000UL,
							&prescaler,
							&compare_register));
  assertEqual(64, prescaler);
  assertEqual(22499, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register(100000UL,
							&prescaler,
							&compare_register));
  assertEqual(256, prescaler);
  assertEqual(18749, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register(1398091UL,
							&prescaler,
							&compare_register));
  assertEqual(1024, prescaler);
  assertEqual(65535UL, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register(1398111UL,
							&prescaler,
							&compare_register));
  assertEqual(1024, prescaler);
  assertEqual(65535UL, compare_register);
  assertEqual((uint8_t) 3,
	      fast_samd21_tc_calculate_compare_register(1398112UL,
							&prescaler,
							&compare_register));
  assertEqual((uint8_t) 3,
	      fast_samd21_tc_calculate_compare_register(1e9,
							&prescaler,
							&compare_register));
}

unittest_main()
