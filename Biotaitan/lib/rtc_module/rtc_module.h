//--------------------------------------------------
/*
* rtc_module.h
*
*  Created on: 2025/05/10
*      Author: silva_lin
*/

#ifndef _RTC_MODULE_H__
#define _RTC_MODULE_H__

#include "Arduino.h"
#include "Wire.h"
#include "RTClib.h"

//--------------------------------------------------
//paramters define
#define RTC_SDA GPIO_NUM_6
#define RTC_SCL GPIO_NUM_7

//--------------------------------------------------
typedef struct
{
    uint16_t year;  
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t dayOfWeek;
}_RtcData;

//--------------------------------------------------
void RTC_Initial(void);
void RTC_ReadTime(DateTime *nowtime);
void RTC_AdjustTime(DateTime settime);

//--------------------------------------------------

#endif
