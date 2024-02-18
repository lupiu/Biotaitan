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
  {"CH1_SEL",  SYS_BtnCH1_SEL},
  {"CH1_START",   SYS_BtnCH1_STA},
  {"CH2_SEL",  SYS_BtnCH2_SEL},
  {"CH2_START",   SYS_BtnCH2_STA}
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
    LCM_ShowInfoString("Please scan patient id", 1);
    g_SystemStatus.Status[SYS_CH1] = SYS_CH_PAT;
  }
  else if (g_SystemStatus.Status[SYS_CH1] == SYS_CH_PAT or g_SystemStatus.Status[SYS_CH1] == SYS_CH_REA)
  {
    LCM_ShowInfoString("Channel 1 rescan!!", 0);
    LCM_ShowInfoString("Please scan patient id", 1);
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
    LCM_ShowInfoString("Channel 1 STOP!!", 0);
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

  char i = '0';
  char a[15] = {0};
  int k = 0;

  LCM_TouchScan(&key_type, &key_num);
  if (key_num < sizeof(g_TopMenu) / sizeof(_LcmMenuType))
  {
    if (g_TopMenu[key_num].CallBack != NULL)
    {
      g_TopMenu[key_num].CallBack();
    }
  }

  if (g_SystemStatus.Status[SYS_CH1] == SYS_CH_PAT)
  {
    if (Serial3.available() > 0)
    {
      //BAR_Read();
      Serial.println("sys ch pat");
      i = Serial3.read();
      a[k] = i;
      k = k + 1;
      if (k == 10)
      {
        Serial.print(a[0]);
        Serial.print(a[2]);
        Serial.print(a[3]);
        Serial.print(a[4]);
        Serial.print(a[5]);
        Serial.print(a[6]);
        Serial.print(a[7]);
        Serial.print(a[8]);
        Serial.print(a[9]);
      }
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