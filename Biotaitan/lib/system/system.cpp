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
#include "optic.h"
#include "lcm.h"

//--------------------------------------------------
_SysHdl g_SysHdl = {0};

_LcmMenuType g_TopMenu[4] = 
{
    {"START",  SYS_BtnF0},
    {"STOP",  SYS_BtnF1},
    {"PREV",  SYS_BtnF2},
    {"NEXT",  SYS_BtnF3}
};

_LcmMenuType g_RunMenu[4] = 
{
    {"STOP",  SYS_BtnF0},
    {"STOP",  SYS_BtnF1},
    {"PREV",  SYS_BtnF2},
    {"NEXT",  SYS_BtnF3}
};

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
void SYS_ChangeStatus(_SysStatus status)
{
  g_SysHdl.Status = status;
  g_SysHdl.ScnUpdated = 0;
}

//--------------------------------------------------
void SYS_BtnF0(void)
{
    //LCM_ShowInfoString("Button F0", 1);
    if (g_SysHdl.Status == SYS_MAIN)
    {
        SYS_ChangeStatus(SYS_RUNNIG);
        LCM_ShowInfoString("Start!!!", 1);
    }
    else if (g_SysHdl.Status == SYS_RUNNIG)
    {
        SYS_ChangeStatus(SYS_MAIN);
        LCM_ShowInfoString("Stop!!!", 1);
    }
}

//--------------------------------------------------
void SYS_BtnF1(void)
{
  LCM_ShowInfoString("Button F1", 1);
}

//--------------------------------------------------
void SYS_BtnF2(void)
{
  LCM_ShowInfoString("Button F2", 1);
}

//--------------------------------------------------
void SYS_BtnF3(void)
{
  LCM_ShowInfoString("Button F3", 1);
}

//--------------------------------------------------
void SYS_BtnScan(_LcmMenuType *menu)
{
    uint8_t key_num;

    LCM_TouchScan(&key_num);
    if (key_num < sizeof(g_TopMenu) / sizeof(_LcmMenuType))
    {
      if (g_TopMenu[key_num].CallBack != NULL)
      {
        g_TopMenu[key_num].CallBack();
      }
    }
}

//--------------------------------------------------
void System_Task(void * pvParametersoid)
{
    uint8_t key_num;

    while(1)
    {
        if (g_SysHdl.Status == SYS_INIT)
        {
            LCM_ShowInfoString("System Initialize...", 1);
            delay(500);
            LCM_ShowInfoString("System Ready.", 1);
            SYS_ChangeStatus(SYS_MAIN);
        }
        else if (g_SysHdl.Status == SYS_MAIN)
        {
            if (g_SysHdl.ScnUpdated == 0)
            {
                LCM_DisplayFuncKey(g_TopMenu);
                g_SysHdl.ScnUpdated = 1;
            }
            SYS_BtnScan(g_TopMenu);
            TEMP_AllOff();
            OPT_Led_OffAll();
        }
        else if (g_SysHdl.Status == SYS_RUNNIG)
        {
            if (g_SysHdl.ScnUpdated == 0)
            {
                LCM_DisplayFuncKey(g_RunMenu);
                g_SysHdl.ScnUpdated = 1;
            }
            SYS_BtnScan(g_RunMenu);
            TEMP_Test(0);
        }
        //OPT_Test(0);
        //TEMP_Test(0);
        //delay(50);
    }
}
//--------------------------------------------------
void SYS_Initial(void)
{
    g_SysHdl.Status = SYS_INIT;

    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, 1);

    LT768_Lib.LT768_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,Blue);
    LT768_Lib.LT768_Select_Internal_Font_Init(24,2,2,0,1);
    LT768_Lib.LT768_Print_Internal_Font_String(LCM_MSG_START_X,2,Black,Blue,"Biotaitan System");
    LT768_Lib.LT768_DrawLine_Width(0, 55, 800, 55, Black, 2);
    LT768_Lib.LT768_Select_Internal_Font_Init(LCM_MSG_SIZE,1,1,1,1);
    xTaskCreatePinnedToCore((TaskFunction_t)System_Task, "System_Task", 4096, NULL, 0, NULL, tskNO_AFFINITY);
}

//--------------------------------------------------