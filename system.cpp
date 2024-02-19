//--------------------------------------------------
/*
* system.cpp
*
*  Created on: 2023/09/14
*      Author: silva_lin
*/

#include "Arduino.h"
#include <string.h>
#include "system.h"
#include "optic.h"
#include "lcm.h"
#include "temp_ctrl.h"
#include "barcode.h"

//--------------------------------------------------
_SysStatus g_SystemStatus = {0};

_LcmMenuType g_TopMenu[4] = 
{
  {"CH1_SEL  ",  SYS_BtnCH1_SEL},
  {"         ",  SYS_BtnCH1_STA},
  {"CH2_SEL  ",  SYS_BtnCH2_SEL},
  {"         ",  SYS_BtnCH2_STA}
};

//--------------------------------------------------
void SYS_Shutdown(void) 
{
  LCM_ScreenShowMsg("Initialization failed...", 1);
  while(1);
}

//--------------------------------------------------
void SYS_ChangeTopMode(void)
{
  LCM_ShowTitleString("Biotaitan System");
  LCM_ShowInfoString("Select operation ch...", 0);
  LCM_DisplayFuncKey(g_TopMenu);
}

//--------------------------------------------------
void SYS_BtnCH1_SEL(void)
{
  if (g_SystemStatus.Status[SYS_CH1] == SYS_CH_INIT)
  {
    LCM_ShowInfoString("Channel 1 active!!", 0);
    LCM_ShowChannelID(SYS_CH1, 0, "                  ");
    LCM_ShowChannelID(SYS_CH1, 1, "                  ");
    LCM_UpdateFuncKey(0, "CH1_RES  ");
    LCM_UpdateFuncKey(1, "         ");
    LCM_UpdateFuncKey(2, "CH2_SEL  ");
    LCM_ShowInfoString("Please scan patient id", 1);
    g_SystemStatus.Status[SYS_CH1] = SYS_CH_PAT;
  }
  else if (g_SystemStatus.Status[SYS_CH1] != SYS_CH_RUN)
  {
    LCM_ShowInfoString("Channel 1 rescan!!", 0);
    LCM_ShowInfoString("Please scan patient id", 1);
    
    LCM_ShowChannelID(SYS_CH1, 0, "                  ");
    LCM_ShowChannelID(SYS_CH1, 1, "                  ");
    LCM_UpdateFuncKey(1, "         ");
    g_SystemStatus.Status[SYS_CH1] = SYS_CH_PAT;
  }
  BAR_TrigOn();
}

//--------------------------------------------------
void SYS_BtnCH1_STA(void)
{
  if (g_SystemStatus.Status[SYS_CH1] == SYS_CH_READY)
  {
    LCM_ShowInfoString("Channel 1 Testing!!", 0);
    LCM_UpdateFuncKey(1, "CH1_STOP ");
    g_SystemStatus.Status[SYS_CH1] = SYS_CH_RUN;
  }
  else if (g_SystemStatus.Status[SYS_CH1] == SYS_CH_RUN)
  {
    LCM_ShowInfoString("Channel 1 STOP!!", 1);
    LCM_UpdateFuncKey(1, "         ");
    g_SystemStatus.Status[SYS_CH1] = SYS_CH_INIT;
  }
}

//--------------------------------------------------
void SYS_BtnCH2_SEL(void)
{
  LCM_ShowInfoString("Channel 2 Active!!", 1);
  LCM_ShowInfoString("Please scan reagent id", 1);
}

//--------------------------------------------------
void SYS_BtnCH2_STA(void)
{
  LCM_ShowInfoString("Channel 0 Active!!", 1);
}

//--------------------------------------------------
void SYS_SystemRun(void)
{
  uint8_t key_type;
  uint8_t key_num;

  LCM_TouchScan(&key_type, &key_num);
  if (key_num < sizeof(g_TopMenu) / sizeof(_LcmMenuType))
  {
    if (g_TopMenu[key_num].CallBack != NULL)
    {
      g_TopMenu[key_num].CallBack();
    }
  }

  BAR_AutoOn();

  if (g_SystemStatus.Status[SYS_CH1] == SYS_CH_PAT)
  {
    if (BAR_Read(g_SystemStatus.Patient_ID[SYS_CH1]) == SYS_OK)
    {
      LCM_ShowChannelID(SYS_CH1, 0, g_SystemStatus.Patient_ID[SYS_CH1]);
      LCM_ShowInfoString("Please scan reagent id", 1);
      g_SystemStatus.Status[SYS_CH1] = SYS_CH_REA;
    }
  }
  else if (g_SystemStatus.Status[SYS_CH1] == SYS_CH_REA)
  {
    if (BAR_Read(g_SystemStatus.Reagent_ID[SYS_CH1]) == SYS_OK)
    {
      LCM_ShowChannelID(SYS_CH1, 1, g_SystemStatus.Reagent_ID[SYS_CH1]);
      LCM_ShowInfoString("Ch1 is ready!!", 1);
      BAR_TrigOff();
      LCM_UpdateFuncKey(1, "CH1_START");
      g_SystemStatus.Status[SYS_CH1] = SYS_CH_READY;
    }
  }

  if (g_SystemStatus.Status[SYS_CH1] == SYS_CH_RUN)
  {
    //TEMP_Test(0);
  }
  else
  {
    if (BASE_ENABLE == 1 && g_SystemStatus.BT_Ready[SYS_CH1] == LOW)
    {
    }
  }

}

//--------------------------------------------------
void SYS_Initial(void)
{
  LCM_DisplayGrid();
  SYS_ChangeTopMode();
}
//--------------------------------------------------