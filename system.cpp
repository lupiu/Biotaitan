//--------------------------------------------------
/*
* system.cpp
*
*  Created on: 2023/09/14
*      Author: silva_lin
*/

#include <Arduino_FreeRTOS.h>
#include "Arduino.h"
#include "system.h"
#include "lcm.h"

//--------------------------------------------------

_SysStatus g_SystemStatus = {SYSTEM_INIT, 0};

//--------------------------------------------------
void SYS_SetOpmode(_SysOpMode status) 
{
  g_SystemStatus.status = status;
}

//--------------------------------------------------
_SysOpMode SYS_GetOpmode(void) 
{
  return g_SystemStatus.status;
}

//--------------------------------------------------
void TaskSystemCtrl(void *pvParameters) 
{
  if ((g_SystemStatus.status == SYSTEM_POR_P1) && ((millis() - g_SystemStatus.time) >= SYS_POR_P1T))
  {

  }
  g_SystemStatus.time = millis();
}

//--------------------------------------------------
void SYS_Shutdown(void) 
{
  while(1);
}

//--------------------------------------------------
void SYS_POR(void) 
{
  LCM_ShowMsg("System POR...", 1)
  delay(3000);
  LCM_ShowMsg("Barcode Scanner...", 1)

  //Heater Power On Test
  LCM_ShowMsg("Heater On to 95c...", 1)
  g_TempData.TargetTemp_C = 95;
  delay(10000);
  if (TEMP_ReadTemperature(NTC_TS1) != 95;
  {
    LCM_ShowMsg("Heater POR Fail!!", 1)
    SYS_Shutdown();
  }

  //SHT20 Power On Test
  LCM_ShowMsg("SHT20 Initial...", 1)

  //TEC cooler Power On Test
  //Photo Diode Power On Test
  LCM_ShowMsg("TEC cooler Initial...", 1)
  g_TempData.TargetTemp_C = 60;
  LCM_ShowMsg("LED 2 4 On", 1)
  OPT_LedOn(OPT_LED2);
  OPT_LedOn(OPT_LED4);
  delay(5000);
  if (TEMP_ReadTemperature(NTC_TS1) != 95;
  {
    LCM_ShowMsg("Heater POR Fail!!", 1)
    SYS_Shutdown();
  }


  g_SystemStatus.time = millis();
}

//--------------------------------------------------
void SYS_Initial(void)
{
  SYS_POR();

  xTaskCreate(TaskSystemCtrl,"System Control",128,NULL,1,NULL);
}
//--------------------------------------------------