//--------------------------------------------------
/*
* temp_ctrl.h
*
*  Created on: 2023/09/14
*      Author: silva_lin
*/

#ifndef _TEMP_CTRL_H__
#define _TEMP_CTRL_H__

#include "Arduino.h"

//--------------------------------------------------
#define NTC_TS1 A0
#define NTC_TS2 A1
#define NTC_TS3 A2
#define NTC_TS4 A3
#define NTC_TS5 A4
#define HEATER_CTRL 6
#define PELTIER_CTRL 7
#define FAN_CTRL 8

#define PID_KP 300  // Proportional constant
#define PID_KI 20 // Integral Constant
#define PID_KD 5 // Derivative Constant

#define PID_WINDOWSIZE 300

#define ANALOG_VA 5.0 //analog Voltage
#define ANALOG_RVD 8250 //analog resistor (ohm)
#define THERM_BASE_DK 273.15 //K@0C
#define THERM_BASE_R 30000 //R@0C
#define THERM_BASE_B 3455 //0~105C
#define THERM_MIN_R 127

//--------------------------------------------------
typedef enum
{
    IDLE  = 0,
    MEASURE,
    PID_EN,
}_TempCtrlStatus;

typedef struct
{
  double PidStartTime;  
  double SetPoint;
  double Input;
  double Output;
}_PidData;

typedef struct
{
  _TempCtrlStatus status;
  float PresentTemp_C;
  float TargetTemp_C;
  double CycleTime;
}_TempCtrl;

//--------------------------------------------------
void TEMP_Initial(void);
float TEMP_ReadTemperature(uint8_t pin);
void TEMP_TempCtrl(float temp_c, uint8_t pin);
//--------------------------------------------------

#endif