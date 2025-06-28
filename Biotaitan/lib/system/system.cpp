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
#include "barcode.h"
#include "sd_card.h"
#include "rtc_module.h"
#include "msc.h"

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
        LCM_ShowInfoString("System in USB Mode!!!", 1);
        MSC_UsbStart();
    }
    else if (g_SysHdl.Status == SYS_RUNNIG)
    {   
        ESP.restart();
    }
}
/*
//--------------------------------------------------
void SYS_BtnF0(void)
{
    //LCM_ShowInfoString("Button F0", 1);
    if (g_SysHdl.Status == SYS_MAIN)
    {
        SYS_ChangeStatus(SYS_RUNNIG);
        //LCM_ShowInfoString("Start!!!", 1);
        Serial.print(F("Time"));
        delay(5);
        Serial.print("\t");
        delay(5);
        Serial.print(F("Temp(*100)"));
        delay(8);
        Serial.print("\t");
        delay(5);
        Serial.print(F("C"));
        delay(5);
        Serial.print("\t");
        delay(5);
        Serial.print(F("LED"));
        delay(5);
        Serial.print("\t");
        delay(5);
        Serial.print(F("PD0"));
        delay(5);
        Serial.print("\t");
        delay(5);
        Serial.print(F("PD1"));
        delay(5);
        Serial.print("\t");
        delay(5);
        Serial.print(F("PD2"));
        delay(5);
        Serial.print("\t");
        delay(5);
        Serial.print(F("PD3"));
        delay(5);
        Serial.print("\t");
        delay(5);
        Serial.println();
        delay(5);
    }
    else if (g_SysHdl.Status == SYS_RUNNIG)
    {   
        analogWrite(HEATER_CTRL, 0);
        while (TEMP_ReadTemperature(NTC_TS3)>StopTemp)
        {
            analogWrite(FAN_CTRL, 255);
            delay(1000);
        }
        //LCM_ShowInfoString("Stop!!!", 1);
        SYS_ChangeStatus(SYS_MAIN);
        ResetTempVariables();
        delay(1000);
        Serial.begin(115200);
    }
}
*/
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
void SYS_CheckCover(int *status)
{
    *status = 1 - digitalRead(COVER_IN);
}

//--------------------------------------------------
void BCR_Test(void)
{
    static double prev_time = 0;
    uint8_t enable;  
    uint8_t datavalid;  
    uint16_t rxbytes;
    uint16_t i;
    char data[BCR_RX_BUF];

    BCR_GetStatus(&enable, &datavalid, &rxbytes);

    if (datavalid == 1 && rxbytes > 0)
    {
        BCR_GetData(data, rxbytes);
        Serial.print(F("Read "));Serial.print(rxbytes);Serial.println(F(" bytes:"));
        for (i = 0; i < rxbytes; i++)
        {
            Serial.println(data[i]);
        }
        prev_time = millis();
    }   
 
    if ((millis() - prev_time > 3000) && enable == 0)
    {
        BCR_Enable();
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

            DateTime now;
            RTC_ReadTime(&now);
            String yearStr = String(now.year(), DEC);
            String monthStr = (now.month() < 10 ? "0" : "") + String(now.month(), DEC);
            String dayStr = (now.day() < 10 ? "0" : "") + String(now.day(), DEC);
            String hourStr = (now.hour() < 10 ? "0" : "") + String(now.hour(), DEC); 
            String minuteStr = (now.minute() < 10 ? "0" : "") + String(now.minute(), DEC);
            String secondStr = (now.second() < 10 ? "0" : "") + String(now.second(), DEC);
          
            // Complete time string
            String formattedTime = yearStr + "-" + monthStr + "-" + dayStr + " " + hourStr + ":" + minuteStr + ":" + secondStr;
            Serial.println(formattedTime);
            char charArray[40];
            formattedTime.toCharArray(charArray, 40);
            LCM_ShowInfoString(charArray, 1);
            SD_WriteLog(formattedTime.c_str());

        }
        else if (g_SysHdl.Status == SYS_RUNNIG)
        {
            SYS_BtnScan(g_RunMenu);
            /*
            if (g_SysHdl.ScnUpdated == 0)
            {
                LCM_DisplayFuncKey(g_RunMenu);
                g_SysHdl.ScnUpdated = 1;
            }
            SYS_BtnScan(g_RunMenu);
            TEMP_Test(0);

            RTC_AdjustTime(DateTime(2025, 05, 17, 18, 20, 47));
            */
        }
        //OPT_Test(0);
        //TEMP_Test(0);
        delay(1000);
        //BCR_Test();
    }
}
//--------------------------------------------------
void SYS_Initial(void)
{
    g_SysHdl.Status = SYS_INIT;
    
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, 1);
    pinMode(COVER_IN, INPUT);

    LT768_Lib.LT768_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,White);
    LT768_Lib.LT768_Select_Internal_Font_Init(24,2,2,0,1);
    LT768_Lib.LT768_Print_Internal_Font_String(LCM_MSG_START_X,2,Black,White,"Biotaitan System");
    LT768_Lib.LT768_DrawLine_Width(0, 55, 800, 55, Black, 2);
    LT768_Lib.LT768_Select_Internal_Font_Init(LCM_MSG_SIZE,1,1,1,1);

    SPI.endTransaction();
    xTaskCreatePinnedToCore((TaskFunction_t)System_Task, "System_Task", 4096, NULL, 0, NULL, tskNO_AFFINITY);
}
//--------------------------------------------------