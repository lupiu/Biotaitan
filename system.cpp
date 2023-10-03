//--------------------------------------------------
/*
* system.cpp
*
*  Created on: 2023/09/14
*      Author: silva_lin
*/

#include "Arduino.h"
#include "system.h"
#include "optic.h"
#include "lcm.h"
#include "temp_ctrl.h"

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
void SYS_Shutdown(void) 
{
  //LCM_ShowMsg("Initialization failed...", 1);
  //while(1);
  return;
}

//--------------------------------------------------
_SysErrCode SYS_PowerOnReset(void) 
{
  if (g_SystemStatus.status == SYSTEM_INIT)
  {
    LCM_ScreenShowMsg("System POR...", 1);

    //POR P1
    delay(SYS_POR_P1T);
    LCM_ScreenShowMsg("Barcode Scanner...", 1);

    //POR P2
    LCM_ScreenShowMsg("Heater On to 95c...", 1);
    g_SystemStatus.time = millis();

    while ((millis() - g_SystemStatus.time) < SYS_POR_P2T)
    {
      TEMP_TempCtrl(95);
    }

    if (TEMP_ReadTemperature(NTC_TS1) == 95)
    {
      LCM_ScreenShowMsg("Heater POR Pass", 1);
    }
    else
    {
      LCM_ScreenShowMsg("Heater POR Fail!!!", 1);
      SYS_Shutdown();
    }

    LCM_ScreenShowMsg("SHT20 Initial...", 1);

    //POR P3
    LCM_ScreenShowMsg("TEC cooler Initial...", 1);
    LCM_ScreenShowMsg("TEC cooler to 60c...", 1);
    LCM_ScreenShowMsg("LED 1 3 On", 1);
    OPT_LedOn(OPT_LED1);
    OPT_LedOn(OPT_LED3);
    g_SystemStatus.time = millis();

    while ((millis() - g_SystemStatus.time) < SYS_POR_P3T)
    {
      TEMP_TempCtrl(60);
    }

    if (TEMP_ReadTemperature(NTC_TS1) == 60)
    {
      LCM_ScreenShowMsg("TEC cooler POR Pass", 1);
    }
    else
    {
      LCM_ScreenShowMsg("TEC cooler POR Fail!!!", 1);
      SYS_Shutdown();
    }

    if (OPT_ReadPhotoDiode(OPT_PD1) != 60)
    {
      LCM_ScreenShowMsg("LED1 or PD1 POR Fail!!", 1);
      SYS_Shutdown();
    }

    if (OPT_ReadPhotoDiode(OPT_PD3) != 60)
    {
      LCM_ScreenShowMsg("LED3 or PD3 POR Fail!!", 1);
      SYS_Shutdown();
    }

    //POR P4
    LCM_ScreenShowMsg("LED 1 3 Off", 1);
    OPT_LedOff(OPT_LED1);
    OPT_LedOff(OPT_LED3);
    LCM_ScreenShowMsg("LED 2 4 On", 1);
    OPT_LedOn(OPT_LED2);
    OPT_LedOn(OPT_LED4);
    delay(SYS_POR_P4T);
    
    if (OPT_ReadPhotoDiode(OPT_PD2) != 60)
    {
      LCM_ScreenShowMsg("LED2 or PD2 POR Fail!!", 1);
      SYS_Shutdown();
    }

    if (OPT_ReadPhotoDiode(OPT_PD4) != 60)
    {
      LCM_ScreenShowMsg("LED4 or PD4 POR Fail!!", 1);
      SYS_Shutdown();
    }

    LCM_ScreenShowMsg("SHT20 Initial...", 1);

    //POR P5
    LCM_ScreenShowMsg("LED 2 4 Off", 1);
    OPT_LedOff(OPT_LED2);
    OPT_LedOff(OPT_LED4);

    g_SystemStatus.time = millis();
    while ((millis() - g_SystemStatus.time) < SYS_POR_P2T)
    {
      TEMP_TempCtrl(50);
    }

    if (TEMP_ReadTemperature(NTC_TS1) == 50)
    {
      LCM_ScreenShowMsg("TEC cooler POR Pass", 1);
    }
    else
    {
      LCM_ScreenShowMsg("TEC cooler POR Fail!!", 1);
      SYS_Shutdown();
    }

    //POR P6
    delay(SYS_POR_P6T);
    LCM_ScreenShowMsg("SHT20 Initial...", 1);
    g_SystemStatus.time = millis();
    return SYS_OK;
  }
}

//--------------------------------------------------
void SYS_Initial(void)
{
  if (SYS_PowerOnReset() != SYS_OK)
  {
    LCM_ScreenShowMsg("Initialization failed...", 1);
    //while(1);
  }
  else
  {
    LCM_ScreenShowMsg("System Initial OK!", 1);
    delay(1000);
    
    g_SystemStatus.status = LCM_TOP;
    LCM_DisplayGrid();
    LCM_DisplayKeyBoard();
    LCM_DisplayTop();
  }
}
//--------------------------------------------------