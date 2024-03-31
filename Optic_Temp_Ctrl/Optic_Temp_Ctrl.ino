//--------------------------------------------------
/*
* main.cpp
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "Arduino.h"
#include "system.h"
#include "temp_ctrl.h"
#include "optic.h"

//--------------------------------------------------
void setup() {
	Serial.begin(115200);

  SYS_Initial();
  TEMP_Initial();
  OPT_Initial();
}

//--------------------------------------------------
void loop()
{
  TEMP_Test(1);
  if (digitalRead(BTN_START) == 0)
  {
    switch (SYS_GetOpMode())
    {
      case SYS_LED_SINGLE :
        Serial.println(F(" SYS_LED_SINGLE START"));
        OPT_Test(0);
        Serial.println(F(" SYS_LED_SINGLE END"));
      break;

      case SYS_LED_DUAL :
        Serial.println(F(" SYS_LED_DUAL START"));
        OPT_Test(1);
        Serial.println(F(" SYS_LED_DUAL END"));
      break;

      case SYS_TEMP_MANUAL_H :
      case SYS_TEMP_MANUAL_L :
        Serial.println(F(" SYS_TEMP_MANUAL START"));
        TEMP_Test(1);
        Serial.println(F(" SYS_TEMP_MANUAL END"));
      break;

      case SYS_TEMP_AUTO :
        Serial.println(F(" SYS_TEMP_AUTO START"));
        TEMP_Test(0);
        Serial.println(F(" SYS_TEMP_AUTO END"));
      break;

      default :;
    }
  }
}
