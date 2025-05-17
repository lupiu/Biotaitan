//--------------------------------------------------
/*
* rtc_module.cpp
*
*  Created on: 2025/05/10
*      Author: silva_lin
*/

#include "rtc_module.h"

RTC_DS1307 rtc;
DateTime g_RtcData = {2025, 01, 01, 00, 00, 00};
//--------------------------------------------------
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//--------------------------------------------------
void RTC_AdjustTime(DateTime settime)
{
    rtc.adjust(settime);
}

//--------------------------------------------------
void RTC_ReadTime(DateTime *nowtime)
{
    nowtime[0] = rtc.now();
}

//--------------------------------------------------
void RTC_Initial(void)
{
    Wire.begin(RTC_SDA, RTC_SCL);
    if (!rtc.begin())
    {
        Serial.println("RTC Initial Fail！");
        while(1);
    }
    Serial.println("RTC Initial OK！");
}