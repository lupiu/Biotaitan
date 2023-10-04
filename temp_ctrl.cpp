//--------------------------------------------------
/*
* temp_ctrl.cpp
*
*  Created on: 2023/09/14
*      Author: silva_lin
*/

#include "Arduino.h"
#include "math.h"
#include "main.h"
#include "temp_ctrl.h"
#include <PID_v1.h>

//--------------------------------------------------
_TempCtrl g_TempData = {IDLE, 25, 25};
_PidData g_PidData;
PID g_Temp_PID(&g_PidData.Input, &g_PidData.Output, &g_PidData.SetPoint, 2, 5, 1, DIRECT);

//--------------------------------------------------
uint16_t TEMP_ReadTemperature(uint8_t pin)
{
  uint16_t temp;
  uint16_t thermistor_r;

  temp = analogRead(pin);
  temp = (temp * 5 / 1023);
  thermistor_r = ((ANALOG_VA * ANALOG_RVD) / (ANALOG_VA - temp));

  temp = (1 / ((1 / THERM_BASE_DK)-((log(THERM_BASE_R / thermistor_r)) / THERM_BASE_B))) - THERM_BASE_DK;

  return temp; //TBD
}

//--------------------------------------------------
void TEMP_HeaterOn(void)
{
  digitalWrite(HEATER_CTRL, 1);
}

//--------------------------------------------------
void TEMP_HeaterOff(void)
{
  digitalWrite(HEATER_CTRL, 0);
}

//--------------------------------------------------
void TEMP_PeltierOn(void)
{
  digitalWrite(PELTIER_CTRL, 1);
}

//--------------------------------------------------
void TEMP_PeltierOff(void)
{
  digitalWrite(PELTIER_CTRL, 0);
}

//--------------------------------------------------
void TEMP_FanOn(void)
{
  digitalWrite(FAN_CTRL, 1);
}

//--------------------------------------------------
void TEMP_FanOff(void)
{
  digitalWrite(FAN_CTRL, 0);
}

//--------------------------------------------------
void TEMP_TempCtrl(double temp_c)
{
  double now_time;
  g_TempData.TargetTemp_C = temp_c;
  g_TempData.PresentTemp_C = TEMP_ReadTemperature(NTC_TS1);

  g_PidData.Input = g_TempData.PresentTemp_C;
  g_PidData.SetPoint = g_TempData.TargetTemp_C;
  g_Temp_PID.Compute();

  now_time = millis();
  if ((now_time - g_PidData.PidStartTime) > PID_WINDOWSIZE)
  {
    g_PidData.PidStartTime += PID_WINDOWSIZE;
  }

  if (g_PidData.Output > (now_time - g_PidData.PidStartTime))
  {
    TEMP_HeaterOn();
  }
  else
  {
    TEMP_HeaterOff();
  }
}

//--------------------------------------------------
void PID_Initial(void)
{
  g_PidData.PidStartTime = millis();
  g_PidData.SetPoint = 100;
  g_Temp_PID.SetOutputLimits(0, 255);
  g_Temp_PID.SetMode(AUTOMATIC);

}

//--------------------------------------------------
void TEMP_Initial(void)
{
  pinMode(NTC_TS1, INPUT);
  pinMode(NTC_TS2, INPUT);
  pinMode(NTC_TS3, INPUT);
  pinMode(NTC_TS4, INPUT);
  pinMode(NTC_TS5, INPUT);
  pinMode(HEATER_CTRL, OUTPUT);
  pinMode(PELTIER_CTRL, OUTPUT);
  pinMode(FAN_CTRL, OUTPUT);

  PID_Initial();
}

//--------------------------------------------------
