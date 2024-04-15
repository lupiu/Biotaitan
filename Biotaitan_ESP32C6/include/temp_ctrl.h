//--------------------------------------------------
/*
* temp_ctrl.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _TEMP_CTRL_H__
#define _TEMP_CTRL_H__

//--------------------------------------------------
#define NTC_TS3 7

#define HEATER_CTRL 4
#define FAN_CTRL 5

#define PID_KP 300  // Proportional constant
#define PID_KI 20 // Integral Constant
#define PID_KD 5 // Derivative Constant

#define ANALOG_VA 5.0 //analog Voltage
#define ANALOG_RVD 8250 //analog resistor (ohm)
#define THERM_BASE_DK 273.15 //K@0C
#define THERM_BASE_R 33800 //32800//R@0C 30000
#define THERM_BASE_B 3950 //3950//0~105C 3455
#define THERM_MIN_R 127

#define BASE_ENABLE 1 //1:enable 0:disable //僅第一個Cycle有作用
#define BASE_TEMP 46 //degrees Celsius
#define BASE_HOLDTIME 60000 //unit:ms

#define TOP_TEMP 90
#define BOTTOM_TEMP 51
#define TOP_HOLDTIME 000
#define BOTTOM_HOLDTIME 000
#define TEMP_CYCLE 5

#define HEATER_KEEP_PWM 0 //0~255

#define TEC_ENABLE 0 //1 enable; 0 disable
#define TEC_PWM 0 //0~255

#define CYCLE_STATUS 0 //0:BASE 1:BOTTOM 2:TOP 3:All close

#define WITH_OPT 1
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
//--------------------------------------------------

#endif
