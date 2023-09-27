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
#define HEATER_CTRL 2
#define PELTIER_CTRL 3
#define FAN_CTRL 4

#define PID_KP 30  // Proportional constant
#define PID_KI 0.7 // Integral Constant
#define PID_KD 200 // Derivative Constant

#define PID_WINDOWSIZE 300
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
  double PresentTemp_C;
  double TargetTemp_C;
  double CycleTime;
}_TempCtrl;

//--------------------------------------------------
void TEMP_Initial(void);
uint16_t TEMP_ReadTemperature(uint8_t pin);
void TEMP_SetTargetTemp_C(double temp_c);
//--------------------------------------------------

#endif