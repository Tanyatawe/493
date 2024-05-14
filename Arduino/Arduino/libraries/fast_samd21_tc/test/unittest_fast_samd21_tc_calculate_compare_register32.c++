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

#include <fast_samd21_tc_calculate_compare_register32.h>

unittest(fast_samd21_tc_calculate_compare_register32) {
  assertEqual((uint64_t) 0,
	      _fast_samd21_tc_calculate_compare_register32(0.021, 1));
  assertEqual((uint64_t) 0,
	      _fast_samd21_tc_calculate_compare_register32(0.1, 4));
  assertEqual((uint64_t) 1,
	      _fast_samd21_tc_calculate_compare_register32(0.1, 2));
  assertEqual((uint64_t) 4,
	      _fast_samd21_tc_calculate_compare_register32(0.1, 1));
  assertEqual((uint64_t) 47,
	      _fast_samd21_tc_calculate_compare_register32(1, 1));
  assertEqual((uint64_t) 479,
	      _fast_samd21_tc_calculate_compare_register32(10, 1));
  assertEqual((uint64_t) 143,
	      _fast_samd21_tc_calculate_compare_register32(6, 2));
  assertEqual((uint64_t) 183,
	      _fast_samd21_tc_calculate_compare_register32(23, 6));
  assertEqual((uint64_t) 864691128455135231,
	      _fast_samd21_tc_calculate_compare_register32(18446744073709551615UL,
							   1024));
  uint16_t prescaler;
  uint32_t compare_register;
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register32(6,
							  &prescaler,
							  &compare_register));
  assertEqual(1, prescaler);
  assertEqual(287, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register32(23,
							  &prescaler,
							  &compare_register));
  assertEqual(1, prescaler);
  assertEqual(1103, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register32(100000000UL,
							  &prescaler,
							  &compare_register));
  assertEqual(2, prescaler);
  assertEqual(2399999999UL, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register32(200000000UL,
							  &prescaler,
							  &compare_register));
  assertEqual(4, prescaler);
  assertEqual(2399999999UL, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register32(400000000UL,
							  &prescaler,
							  &compare_register));
  assertEqual(8, prescaler);
  assertEqual(2399999999UL, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register32(800000000UL,
							  &prescaler,
							  &compare_register));
  assertEqual(16, prescaler);
  assertEqual(2399999999UL, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register32(1600000000UL,
							  &prescaler,
							  &compare_register));
  assertEqual(64, prescaler);
  assertEqual(1199999999UL, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register32(6000000000UL,
							  &prescaler,
							  &compare_register));
  assertEqual(256, prescaler);
  assertEqual(1124999999UL, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register32(30000000000UL,
							  &prescaler,
							  &compare_register));
  assertEqual(1024, prescaler);
  assertEqual(1406249999UL, compare_register);
  assertEqual((uint8_t) 0,
	      fast_samd21_tc_calculate_compare_register32(91625968981UL,
							  &prescaler,
							  &compare_register));
  assertEqual(1024, prescaler);
  assertEqual(4294967295UL, compare_register);
  assertEqual((uint8_t) 3,
	      fast_samd21_tc_calculate_compare_register32(91625968996UL,
							  &prescaler,
							  &compare_register));
  assertEqual((uint8_t) 3,
	      fast_samd21_tc_calculate_compare_register32(100000000000UL,
							  &prescaler,
							  &compare_register));
  assertEqual((uint8_t) 3,
	      fast_samd21_tc_calculate_compare_register32(1e11,
							  &prescaler,
							  &compare_register));
}

unittest_main()
