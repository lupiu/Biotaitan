//--------------------------------------------------
/*
* system.cpp
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "Arduino.h"
#include "system.h"
#include "temp_ctrl.h"

//--------------------------------------------------
_SysOpmode g_SysOpMode;
_SysStatus g_SysStatus;
uint8_t g_BoardSel = 0;

//--------------------------------------------------
_SysOpmode SYS_GetOpMode()
{
  return g_SysOpMode;
}

//--------------------------------------------------
uint8_t SYS_GetBoardSel()
{
  return g_BoardSel;
}

//--------------------------------------------------
void(* resetFunc) (void) = 0;

//--------------------------------------------------
void SYS_ErrorReset()
{
  analogWrite(HEATER_CTRL, 0);
  analogWrite(FAN_CTRL, 0);
  Serial.println(F("System Reset!!!"));
  delay(10);
  resetFunc();
}

//--------------------------------------------------
void System_Task(void * pvParametersoid)
{
    while(1)
    {
    }
}
//--------------------------------------------------
void SYS_Initial(void)
{
    g_SysStatus = SYS_INIT;
    
    xTaskCreatePinnedToCore((TaskFunction_t)System_Task, "System_Task", 4096, NULL, 0, NULL, tskNO_AFFINITY);
}

//--------------------------------------------------