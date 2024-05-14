/****************************************************************************************************************************
  ISR_8_PWMs_Array.ino
  For Arduino SAM_DUE boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/SAMDUE_Slow_PWM
  Licensed under MIT license
  
  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one megaAVR-based timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.
*****************************************************************************************************************************/

#if !( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
  #error This is designed only for Arduino SAM_DUE board! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "SAMDUE_Slow_PWM.h"
// _PWM_LOGLEVEL_ from 0 to 4
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_      3

// Default is true, uncomment to false
//#define CHANGING_PWM_END_OF_CYCLE     false

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "SAMDUE_Slow_PWM.h"

#include <SimpleTimer.h>              // https://github.com/jfturcot/SimpleTimer

#define LED_OFF             HIGH
#define LED_ON              LOW

//#ifndef LED_BUILTIN
//  #define LED_BUILTIN       13
//#endif

#ifndef LED_BLUE
  #define LED_BLUE          2
#endif

#ifndef LED_RED
  #define LED_RED           3
#endif

#define USING_HW_TIMER_INTERVAL_MS        false   //true

// Don't change these numbers to make higher Timer freq. System can hang
#define HW_TIMER_INTERVAL_US        20L
#define HW_TIMER_INTERVAL_FREQ      50000L

volatile uint32_t startMicros = 0;

// Init SAMDUE_Slow_PWM, each can service 16 different ISR-based PWM channels
SAMDUE_Slow_PWM ISR_PWM;

//////////////////////////////////////////////////////

void TimerHandler()
{ 
  ISR_PWM.run();
}

//////////////////////////////////////////////////////

#define PIN_22      22
#define PIN_23      23
#define PIN_24      24
#define PIN_25      25
#define PIN_26      26
#define PIN_27      27
#define PIN_28      28

//////////////////////////////////////////////////////

// You can assign pins here. Be careful to select good pin to use or crash, e.g pin 0-1
uint32_t PWM_Pin[] =
{
  LED_BUILTIN, PIN_22, PIN_23,  PIN_24,  PIN_25,  PIN_26,  PIN_26,  PIN_28
};

#define NUMBER_ISR_PWMS         ( sizeof(PWM_Pin) / sizeof(uint32_t) )

// You can assign any interval for any timer here, in Hz
float PWM_Freq[] =
{
  1.0f,  2.0f,  3.0f,  5.0f,  10.0f,  20.0f,  30.0f,  50.0f
};

// You can assign any duty-cycle for any PWM channel here, in %
float PWM_DutyCycle[] =
{
  5.0, 10.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0
};


typedef void (*irqCallback)  ();


// In Portenta_H7, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething0()
{
}

void doingSomething1()
{
}

void doingSomething2()
{
}

void doingSomething3()
{
}

void doingSomething4()
{
}

void doingSomething5()
{
}

void doingSomething6()
{
}

void doingSomething7()
{
}


irqCallback irqCallbackStartFunc[NUMBER_ISR_PWMS] =
{
  doingSomething0,  doingSomething1,  doingSomething2,  doingSomething3, 
  doingSomething4,  doingSomething5,  doingSomething6,  doingSomething7
};

////////////////////////////////////////////////

uint16_t attachDueInterrupt(double microseconds, timerCallback callback, const char* TimerName)
{
  DueTimerInterrupt dueTimerInterrupt = DueTimer.getAvailable();
  
  dueTimerInterrupt.attachInterruptInterval(microseconds, callback);

  uint16_t timerNumber = dueTimerInterrupt.getTimerNumber();
  
  Serial.print(TimerName); Serial.print(F(" attached to Timer(")); Serial.print(timerNumber); Serial.println(F(")"));

  return timerNumber;
}

////////////////////////////////////////////////

void setup()
{ 
  Serial.begin(115200);
  while (!Serial);

  delay(2000);

  Serial.print(F("\nStarting ISR_8_PWMs_Array on ")); Serial.println(BOARD_NAME);
  Serial.println(SAMDUE_SLOW_PWM_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));
  Serial.print(F("Timer Frequency = ")); Serial.print(SystemCoreClock / 1000000); Serial.println(F(" MHz"));

  // Interval in microsecs
  attachDueInterrupt(HW_TIMER_INTERVAL_US, TimerHandler, "ITimer");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_PWM
  for (uint16_t i = 0; i < NUMBER_ISR_PWMS; i++)
  {
    //void setPWM(uint32_t pin, float frequency, float dutycycle
    // , timer_callback_p StartCallback = nullptr, timer_callback_p StopCallback = nullptr)

    // You can use this with PWM_Freq in Hz
    ISR_PWM.setPWM(PWM_Pin[i], PWM_Freq[i], PWM_DutyCycle[i], irqCallbackStartFunc[i]);
  }
}

void loop()
{
}
