/****************************************************************************************************************************
  multiFileProject.ino
  
  For Arduino SAM_DUE boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/SAMDUE_Slow_PWM
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#if !( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
  #error This is designed only for Arduino SAM_DUE board! Please check your Tools->Board setting.
#endif

#define SAMDUE_SLOW_PWM_VERSION_MIN_TARGET      F("SAMDUE_Slow_PWM v1.2.2")
#define SAMDUE_SLOW_PWM_VERSION_MIN             1002002

#include "multiFileProject.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "SAMDUE_Slow_PWM.h"

void setup() 
{
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("\nStart multiFileProject");
  Serial.println(SAMDUE_SLOW_PWM_VERSION);

#if defined(SAMDUE_SLOW_PWM_VERSION_MIN)
  if (SAMDUE_SLOW_PWM_VERSION_INT < SAMDUE_SLOW_PWM_VERSION_MIN)
  {
    Serial.print("Warning. Must use this example on Version equal or later than : ");
    Serial.println(SAMDUE_SLOW_PWM_VERSION_MIN_TARGET);
  }
#endif
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
