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
#define NTC_TS3 15

#define HEATER_CTRL 4
#define FAN_CTRL 5

#define PID_KP 300  // Proportional constant
#define PID_KI 20 // Integral Constant
#define PID_KD 5 // Derivative Constant

#define ANALOG_VA 3.3 //analog Voltage
#define ANALOG_RVD 3300 //analog resistor (ohm)
#define THERM_BASE_DK 273.15 //K@0C
#define THERM_BASE_R 33800 //32800//R@0C 30000
#define THERM_BASE_B 3950 //3950//0~105C 3455
#define THERM_MIN_R 127

#define BASE_ENABLE 1 //1:enable 0:disable //僅第一個Cycle有作用
#define BASE_TEMP 53 //degrees Celsius
#define BASE_HOLDTIME 300000 //unit:ms

#define INITIAL_TOP_TEMP 102         // 初始循環的上限溫度，例如 100°C
#define INITIAL_BOTTOM_TEMP 63      // 初始循環的下限溫度，例如 20°C
#define INITIAL_TOP_HOLDTIME 60000      // 初始循環上限溫度的保持時間，單位為毫秒 80000  
#define INITIAL_BOTTOM_HOLDTIME 0    // 初始循環下限溫度的保持時間，單位為毫秒 
#define TOP_TEMP 113
#define BOTTOM_TEMP 63
#define TOP_HOLDTIME 0
#define BOTTOM_HOLDTIME 0
#define TEMP_CYCLE 45


#define StopTemp 40

#define HEATER_KEEP_PWM 0 //0~255

#define TEC_ENABLE 0 //1 enable; 0 disable
#define TEC_PWM 0 //0~255

#define CYCLE_STATUS 3 //0:BASE 1:BOTTOM 2:TOP 3:All close

#define SAFTY_TOP_TEMP 130
#define SAFTY_BOTTOM_TEMP 10

#define WITH_OPT 1
#define OPT_ON_SEL 3 //0, 1, 2, 3 (1 Red 2 Blue 3 Green 0 Yellow Top to bottum RBGY)
#define OPT_ON_SEL2 2 //0, 1, 2, 3 (LED)
#define OPT_ON_SEL3 1//0, 1, 2, 3 (LED)
#define OPT_ON_SEL4 1 //0, 1, 2, 3 (LED)
#define OPT_num 3 //# of LED on
#define RISE_OPT_ON_T 20
#define RISE_OPT_OFF_T 85
#define FALL_OPT_ON_T 140 //70 65
#define FALL_OPT_OFF_T 130
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
  float PresentTemp_C;
  float TargetTemp_C;
  double CycleTime;
}_TempCtrl;

//--------------------------------------------------
void TEMP_Initial(void);
void TEMP_Test(uint8_t mode);
float TEMP_ReadTemperature(uint8_t pin);
uint8_t TEMP_CycleCtrl(float temp_c, uint8_t dir, double holdtime);
void TEMP_AllOff(void);
void ResetTempVariables(void);
//--------------------------------------------------

#endif
