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

//--------------------------------------------------
_SysStatus g_SystemStatus = {0};
_LcmMenuType g_TopMenu[6] = 
{
  {"CH1_SEL", SYS_ChangeChannel},
  {"CH1_RE",  NULL},
  {"CH1_STA", NULL},
  {"CH2_SEL", SYS_ChangeChannel},
  {"CH2_RE",  NULL},
  {"CH2_STA", NULL}
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
  g_SystemStatus.Channel = SYS_CH1;
  LCM_ShowTitleString("Biotaitan System");
  LCM_ShowInfoString("Select operation mode...", 0);
  LCM_DisplayFuncKey(g_TopMenu);
}

//--------------------------------------------------
void SYS_ChangeChannel(_SysChannel ch)
{
  char str[] = "";
  g_SystemStatus.Channel = ch;
  sprintf(str, "Channel %d Active", ch);
  LCM_ShowInfoString(str, 0);
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
}

//--------------------------------------------------
void SYS_Initial(void)
{
  LCM_DisplayGrid();
  LCM_DisplayKeyBoard();
  SYS_ChangeTopMode();
}
//--------------------------------------------------