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
  {"CH1_STOP ",  SYS_BtnCH1_STA},
  {"CH2_SEL  ",  SYS_BtnCH2_SEL},
  {"CH2_STOP ",  SYS_BtnCH2_STA}
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
void SYS_ClearID(_SysChannel ch)
{
  memcpy(g_SystemStatus.Patient_ID[ch], 0, BAR_LENGTH);
  LCM_ShowChannelID(ch, 0, "                  ");
  memcpy(g_SystemStatus.Reagent_ID[ch], 0, BAR_LENGTH);
  LCM_ShowChannelID(ch, 1, "                  ");
}
//--------------------------------------------------
void SYS_BtnCH1_SEL(void)
{
  if (g_SystemStatus.Status[SYS_CH1] == SYS_CH_INIT)
  {
    LCM_ShowInfoString("Channel 1 active!!", 0);
    SYS_ClearID(SYS_CH1);
    LCM_UpdateFuncKey(0, "CH1_RES  ");
    LCM_UpdateFuncKey(2, "CH2_SEL  ");
    LCM_ShowInfoString("Please scan patient id", 1);
    g_SystemStatus.Status[SYS_CH1] = SYS_CH_PAT;
  }
  else if (g_SystemStatus.Status[SYS_CH1] != SYS_CH_RUN)
  {
    LCM_ShowInfoString("Channel 1 rescan!!", 0);
    LCM_ShowInfoString("Please scan patient id", 1);
    
    SYS_ClearID(SYS_CH1);
    LCM_UpdateFuncKey(1, "CH1_STOP ");
    g_SystemStatus.Status[SYS_CH1] = SYS_CH_PAT;
  }
}

//--------------------------------------------------
void SYS_BtnCH1_STA(void)
{
  if (g_SystemStatus.Status[SYS_CH1] == SYS_CH_READY)
  {
    LCM_ShowInfoString("Channel 1 Testing!!", 0);
    g_SystemStatus.Status[SYS_CH1] = SYS_CH_RUN;
  }
  else if (g_SystemStatus.Status[SYS_CH1] == SYS_CH_RUN)
  {
    LCM_ShowInfoString("Channel 1 STOP!!", 1);
    g_SystemStatus.Status[SYS_CH1] = SYS_CH_INIT;
  }
  else if (g_SystemStatus.Status[SYS_CH1] != SYS_CH_INIT)
  {
    LCM_ShowInfoString("Channel 1 STOP!!", 1);
    BAR_TrigOff();
    g_SystemStatus.Status[SYS_CH1] = SYS_CH_INIT;
  }
  
  LCM_UpdateFuncKey(1, "CH1_STOP ");
}

//--------------------------------------------------
void SYS_BtnCH2_SEL(void)
{
  if (g_SystemStatus.Status[SYS_CH2] == SYS_CH_INIT)
  {
    LCM_ShowInfoString("Channel 2 active!!", 0);
    SYS_ClearID(SYS_CH2);
    LCM_UpdateFuncKey(0, "CH1_SEL  ");
    LCM_UpdateFuncKey(2, "CH2_RES  ");
    LCM_ShowInfoString("Please scan patient id", 1);
    g_SystemStatus.Status[SYS_CH2] = SYS_CH_PAT;
  }
  else if (g_SystemStatus.Status[SYS_CH2] != SYS_CH_RUN)
  {
    LCM_ShowInfoString("Channel 2 rescan!!", 0);
    LCM_ShowInfoString("Please scan patient id", 1);
    
    SYS_ClearID(SYS_CH2);
    LCM_UpdateFuncKey(3, "CH2_STOP ");
    g_SystemStatus.Status[SYS_CH2] = SYS_CH_PAT;
  }
}

//--------------------------------------------------
void SYS_BtnCH2_STA(void)
{
  if (g_SystemStatus.Status[SYS_CH2] == SYS_CH_READY)
  {
    LCM_ShowInfoString("Channel 2 Testing!!", 0);
    g_SystemStatus.Status[SYS_CH2] = SYS_CH_RUN;
  }
  else if (g_SystemStatus.Status[SYS_CH2] == SYS_CH_RUN)
  {
    LCM_ShowInfoString("Channel 2 STOP!!", 1);
    g_SystemStatus.Status[SYS_CH2] = SYS_CH_INIT;
  }
  else if (g_SystemStatus.Status[SYS_CH2] != SYS_CH_INIT)
  {
    LCM_ShowInfoString("Channel 2 STOP!!", 1);
    BAR_TrigOff();
    g_SystemStatus.Status[SYS_CH2] = SYS_CH_INIT;
  }
  
  LCM_UpdateFuncKey(3, "CH2_STOP ");
}

//--------------------------------------------------
void SYS_TempCtrl(_SysChannel ch)
{
  uint8_t  msg[18] = "                  ";

  if (g_SystemStatus.Cycle[ch] != 0)
  {
    if (g_SystemStatus.TempDir[ch] == HIGH)
    {
      if (TEMP_CycleCtrl(ch, TOP_TEMP, g_SystemStatus.TempDir[ch], TOP_HOLDTIME) == 1)
      {
        g_SystemStatus.TempDir[ch] = LOW;
      }
    }
    else
    {
      if (TEMP_CycleCtrl(ch, BOTTOM_TEMP, g_SystemStatus.TempDir[ch], BOTTOM_HOLDTIME) == 1)
      {
        g_SystemStatus.TempDir[ch] = HIGH;
        g_SystemStatus.Cycle[ch]--;
        sprintf(msg, "CNT:%d", g_SystemStatus.Cycle[ch]);
        LCM_ShowCycleCnt(ch, msg);
      }
    }
  }
  else
  {
    if (ch == SYS_CH1)
    {
      LCM_ShowInfoString("Channel 1 is done!!", 1);
      LCM_UpdateFuncKey(0, "CH1_SEL  ");
      g_SystemStatus.Status[SYS_CH1] = SYS_CH_INIT;
    }
    else
    {
      LCM_ShowInfoString("Channel 2 is done!!", 1);
      LCM_UpdateFuncKey(2, "CH2_SEL  ");
      g_SystemStatus.Status[SYS_CH2] = SYS_CH_INIT;
    }
  }
}

//--------------------------------------------------
void SYS_StatusCheck(_SysChannel ch)
{
  int i;
  uint8_t  id[16] = "                ";
  uint8_t  msg[18] = "                  ";

  if (g_SystemStatus.Status[ch] == SYS_CH_PAT)
  {
    BAR_TrigOn();
    if (BAR_Read(g_SystemStatus.Patient_ID[ch]) == SYS_OK)
    {
      BAR_TrigOff();
      for (i = 0; i <= 13; i++)
      {
        id[i] = g_SystemStatus.Patient_ID[ch][i + 4];
      }
      LCM_ShowChannelID(ch, 0, id);
      LCM_ShowInfoString("Please scan reagent id", 1);
      g_SystemStatus.Status[ch] = SYS_CH_REA;

    }
  }
  else if (g_SystemStatus.Status[ch] == SYS_CH_REA)
  {
    BAR_TrigOn();
    if (BAR_Read(g_SystemStatus.Reagent_ID[ch]) == SYS_OK)
    {
      BAR_TrigOff();
      for (i = 0; i <= 13; i++)
      {
        id[i] = g_SystemStatus.Reagent_ID[ch][i + 4];
      }
      LCM_ShowChannelID(ch, 1, id);
      if (ch == SYS_CH1)
      {
        LCM_ShowInfoString("Channel 1 is ready!!", 1);
        LCM_UpdateFuncKey(1, "CH1_START");
      }
      else
      {
        LCM_ShowInfoString("Channel 2 is ready!!", 1);
        LCM_UpdateFuncKey(3, "CH2_START");
      }
      g_SystemStatus.Status[ch] = SYS_CH_READY;
    }
  }

  if (g_SystemStatus.Status[ch] == SYS_CH_RUN)
  {
    SYS_TempCtrl(ch);
  }
  else
  {
    if  (g_SystemStatus.Status[ch] == SYS_CH_READY)
    {
      sprintf(msg, "CNT:%d", TEMP_CYCLE);
      LCM_ShowCycleCnt(ch, msg);
      g_SystemStatus.Cycle[ch] = TEMP_CYCLE;
      g_SystemStatus.TempDir[ch] = HIGH;
    }

    if (BASE_ENABLE == 1)
    {
      if (g_SystemStatus.Status[ch] != SYS_CH_INIT)
      {
        TEMP_PidCal(ch, BASE_TEMP);
        TEMP_PidCtrl(ch);
      }
    }
    else
    {
      TEMP_AllOff(ch);
    }
  }
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
  SYS_StatusCheck(SYS_CH1);
  SYS_StatusCheck(SYS_CH2);

}

//--------------------------------------------------
void SYS_Initial(void)
{
  LCM_DisplayGrid();
  SYS_ChangeTopMode();
  SYS_ClearID(SYS_CH1);
  SYS_ClearID(SYS_CH2);
}
//--------------------------------------------------