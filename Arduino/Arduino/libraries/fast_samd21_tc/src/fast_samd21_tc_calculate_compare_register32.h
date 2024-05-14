/*
  Author: Daniel Mohr
  Date: 2022-10-05

  This header file is used by 'fast_samd21_tc4_tc5.h'.

  This file calculates a compare register value and a prescaler for a 32-bit
  counter.
*/

#ifndef fast_samd21_tc_calculate_compare_register32_h
#define fast_samd21_tc_calculate_compare_register32_h

static inline uint64_t
_fast_samd21_tc_calculate_compare_register32(double us,
					     uint32_t prescaler) {
  return (uint64_t) round(us * (SystemCoreClock / prescaler) / 1e6) - 1;
}

uint8_t fast_samd21_tc_calculate_compare_register32(double us,
						    uint16_t *prescaler,
						    uint32_t *compare_register) {
  // find prescaler and compare register value
  // try TC_CTRLA_PRESCALER_DIV1
  uint16_t _prescaler = 1;
  uint64_t _compare_register =
    _fast_samd21_tc_calculate_compare_register32(us, _prescaler);
  if (_compare_register > UINT32_MAX) {
    // try TC_CTRLA_PRESCALER_DIV2
    _prescaler = 2;
    _compare_register =
      _fast_samd21_tc_calculate_compare_register32(us, _prescaler);
    if (_compare_register > UINT32_MAX) {
      // try TC_CTRLA_PRESCALER_DIV4
      _prescaler = 4;
      _compare_register =
	_fast_samd21_tc_calculate_compare_register32(us, _prescaler);
      if (_compare_register > UINT32_MAX) {
        // try TC_CTRLA_PRESCALER_DIV8
        _prescaler = 8;
        _compare_register =
	  _fast_samd21_tc_calculate_compare_register32(us, _prescaler);
        if (_compare_register > UINT32_MAX) {
          // try TC_CTRLA_PRESCALER_DIV16
          _prescaler = 16;
          _compare_register =
	    _fast_samd21_tc_calculate_compare_register32(us, _prescaler);
          if (_compare_register > UINT32_MAX) {
            // try TC_CTRLA_PRESCALER_DIV64
            _prescaler = 64;
            _compare_register =
	      _fast_samd21_tc_calculate_compare_register32(us, _prescaler);
            if (_compare_register > UINT32_MAX) {
              // try TC_CTRLA_PRESCALER_DIV256
              _prescaler = 256;
              _compare_register =
		_fast_samd21_tc_calculate_compare_register32(us, _prescaler);
              if (_compare_register > UINT32_MAX) {
                // try TC_CTRLA_PRESCALER_DIV1024
                _prescaler = 1024;
                _compare_register =
		  _fast_samd21_tc_calculate_compare_register32(us, _prescaler);
		if (_compare_register > UINT32_MAX)
		  return 3;
              }
            }
          }
        }
      }
    }
  }
  *prescaler = _prescaler;
  *compare_register = (uint32_t) _compare_register;
  return 0;
}

#endif /* fast_samd21_tc_calculate_compare_register32_h */
