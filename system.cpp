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
_SysStatus g_SystemStatus = {SYS_INIT, 0};
_LcmMenuType g_TopMenu[9] = 
{
  {"   ",     NULL},
  {"USR",   NULL},
  {"START",    NULL},
  {"   ",     NULL},
  {"ENG",   SYS_ChangeEmMode},
  {"STOP",  NULL},
  {"   ",     NULL},
  {"PT ",   SYS_ChangePtMode},
  {"BACK",  NULL}
};

_LcmMenuType g_EMMenu[9] = 
{
  {"   ",     NULL},
  {"   ",     NULL},
  {"START",     NULL},
  {"   ",     NULL},
  {"   ",     NULL},
  {"STOP",     NULL},
  {"   ",     NULL},
  {"   ",     NULL},
  {"BACK",  SYS_ChangeTopMode}
};

_LcmMenuType g_PTMenu[9] = 
{
  {"   ",     NULL},
  {"   ",     NULL},
  {"START",     NULL},
  {"   ",     NULL},
  {"   ",     NULL},
  {"STOP",     NULL},
  {"   ",     NULL},
  {"   ",     NULL},
  {"BACK",  SYS_ChangeTopMode}
};

_PrmPtMode g_PTParam[4] = 
{
  {"NTC",     {{"TS1",0}, {"TS2", 0}, {"TS3", 0}, {"TS4", 0}, {"TS5", 0}}},
  {"OPTIC",   {{"LED1", 0}, {"LED2", 0}, {"LED3", 0}, {"LED4", 0}, NULL}},
  {"HEATER",  {{"HEAT1", 25}, NULL, NULL, NULL, NULL}},
  {"COOLER",  {{"COOL1", 25}, NULL, NULL, NULL, NULL}}
};
//--------------------------------------------------
void SYS_SetOpmode(_SysOpMode status) 
{
  g_SystemStatus.Status = status;
}

//--------------------------------------------------
_SysOpMode SYS_GetOpmode(void) 
{
  return g_SystemStatus.Status;
}

//--------------------------------------------------
void SYS_Shutdown(void) 
{
  LCM_ScreenShowMsg("Initialization failed...", 1);
  while(1);
}

//--------------------------------------------------
_SysErrCode SYS_PowerOnReset(void) 
{
  if (g_SystemStatus.Status == SYS_INIT)
  {
    LCM_ScreenShowMsg("System POR...", 1);

    //POR P1
    delay(SYS_POR_P1T);
    LCM_ScreenShowMsg("Barcode Scanner...", 1);

    //POR P2
    LCM_ScreenShowMsg("Heater On to 95c...", 1);
    g_SystemStatus.Time = millis();

    while ((millis() - g_SystemStatus.Time) < SYS_POR_P2T)
    {
      TEMP_TempCtrl(95, NTC_TS3);
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
    g_SystemStatus.Time = millis();

    while ((millis() - g_SystemStatus.Time) < SYS_POR_P3T)
    {
      TEMP_TempCtrl(60, NTC_TS3);
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

    g_SystemStatus.Time = millis();
    while ((millis() - g_SystemStatus.Time) < SYS_POR_P2T)
    {
      TEMP_TempCtrl(50, NTC_TS3);
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
    g_SystemStatus.Time = millis();
    return SYS_OK;
  }
}

//--------------------------------------------------
void SYS_ChangeTopMode(void)
{
  g_SystemStatus.Status = SYS_TOP;
  LCM_ShowTitleString("Biotaitan System");
  LCM_ShowParamString("Parameter");
  LCM_ShowValue(0);
  LCM_ShowInfoString("Select operation mode...", 0);
  LCM_DisplayFuncKey(g_TopMenu);
}

//--------------------------------------------------
void SYS_ChangeEmMode(void)
{
  g_SystemStatus.Status = SYS_ENGMODE;
  LCM_ShowTitleString("Engineering Mode");
  LCM_ShowParamString("Parameter");
  LCM_ShowValue(0);
  LCM_ShowInfoString("Select operation mode...", 0);
  LCM_DisplayFuncKey(g_EMMenu);
}

//--------------------------------------------------
void SYS_ChangePtMode(void)
{
  g_SystemStatus.Status = SYS_PTMODE;
  LCM_ShowTitleString("Part Test Mode");
  LCM_ShowParamString("Parameter");
  LCM_ShowValue(0);
  LCM_ShowInfoString("touch UP or DN", 0);
  LCM_DisplayFuncKey(g_PTMenu);
}

//--------------------------------------------------
void SYS_PartTestRun(uint8_t key_type, uint8_t key_num)
{
  if (g_PTMenu[key_num].CallBack != NULL)
  {
    g_PTMenu[key_num].CallBack();
  }

  
}

//--------------------------------------------------
void SYS_SystemRun(void)
{
  uint8_t key_type;
  uint8_t key_num;

  LCM_TouchScan(&key_type, &key_num);
  if (key_type == BUTTON_FUNC)
  {
    switch (g_SystemStatus.Status)
    {
      case SYS_TOP :
        if (g_TopMenu[key_num].CallBack != NULL)
        {
          g_TopMenu[key_num].CallBack();
        }
      break;
      case SYS_ENGMODE :
        if (g_EMMenu[key_num].CallBack != NULL)
        {
          g_EMMenu[key_num].CallBack();
        }
      break;
      case SYS_PTMODE :
        if (g_PTMenu[key_num].CallBack != NULL)
        {
          g_PTMenu[key_num].CallBack();
        }
      break;
      default :;
    }
  }
  else if (key_type == BUTTON_KEY)
  {
    switch (g_SystemStatus.Status)
    {
      case SYS_ENGMODE :

      break;
      case SYS_PTMODE :
        if (g_PTMenu[key_num].CallBack != NULL)
        {
          g_PTMenu[key_num].CallBack();
        }
      break;
      default :;
    }

  }
}

//--------------------------------------------------
void SYS_Initial(void)
{
  /*
  if (SYS_PowerOnReset() != SYS_OK)
  {
    LCM_ScreenShowMsg("Initialization failed...", 1);
    //while(1);
  }
  else
  {
    LCM_ScreenShowMsg("System Initial OK!", 1);
    delay(1000);
    
    g_SystemStatus.Status = SYS_TOP;
    LCM_DisplayGrid();
    LCM_DisplayKeyBoard();
    LCM_DisplayTop();
  }
  */
  LCM_DisplayGrid();
  LCM_DisplayKeyBoard();
  SYS_ChangeTopMode();
}
//--------------------------------------------------