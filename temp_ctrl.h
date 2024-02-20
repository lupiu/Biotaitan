//--------------------------------------------------
/*
* temp_ctrl.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _TEMP_CTRL_H__
#define _TEMP_CTRL_H__

#include "Arduino.h"

//--------------------------------------------------
#define TEMP_GROUP 2

#define NTC_CH1 A4
#define NTC_CH2 A5

#define HEATER_CTRL1 2
#define FAN_CTRL1 3
#define HEATER_CTRL2 4
#define FAN_CTRL2 5

#define PID_KP 300  // Proportional constant
#define PID_KI 20 // Integral Constant
#define PID_KD 5 // Derivative Constant

#define ANALOG_VA 5.0 //analog Voltage
#define ANALOG_RVD 8250 //analog resistor (ohm)
#define THERM_BASE_DK 273.15 //K@0C
#define THERM_BASE_R 32800 //R@0C
#define THERM_BASE_B 3950 //0~105C
#define THERM_MIN_R 127

#define BASE_ENABLE 1 //1:enable 0:disable //僅第一個Cycle有作用
#define BASE_TEMP 55 //degrees Celsius
#define BASE_HOLDTIME 2 //unit:sec

#define TOP_TEMP 95
#define BOTTOM_TEMP 65
#define TOP_HOLDTIME 2
#define BOTTOM_HOLDTIME 5
#define TEMP_CYCLE 4

#define SAFTY_TOP_TEMP 180
#define SAFTY_BOTTOM_TEMP 10

#define CYCLE_STATUS 3 //0:BASE 1:BOTTOM 2:TOP 3:All close

//===================//
typedef struct
{
  double PidStartTime;  
  double SetPoint;
  double Input;
  double Output;
}_PidData;

typedef struct
{
  uint8_t NTC;
  uint8_t Heater;
  uint8_t Fan;
  float PresentTemp_C;
  float TargetTemp_C;
  double AchieveTime;
  uint8_t AchieveFlag;
}_TempCtrl;

//--------------------------------------------------
void TEMP_Initial(void);
void TEMP_PidCal(uint8_t ch,float temp_c);
void TEMP_PidCtrl(uint8_t ch);
uint8_t TEMP_CycleCtrl(uint8_t ch, float temp_c, uint8_t dir, uint16_t holdtime);
void TEMP_AllOff(uint8_t ch);
//--------------------------------------------------

#endif