/****************************************************************************************************************************
  multiFileProject.h
  
  For Arduino SAM_DUE boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/SAMDUE_Slow_PWM
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#pragma once

#define USING_HW_TIMER_INTERVAL_MS        false   //true

// Don't change these numbers to make higher Timer freq. System can hang
#define HW_TIMER_INTERVAL_US        20L
#define HW_TIMER_INTERVAL_FREQ      50000L

// Default is true, uncomment to false
//#define CHANGING_PWM_END_OF_CYCLE     false

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "SAMDUE_Slow_PWM.hpp"
