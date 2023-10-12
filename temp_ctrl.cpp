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
float TEMP_ReadTemperature(uint8_t pin)
{
  float temp;
  float thermistor_r;

  temp = analogRead(pin);
  temp = ((temp * ANALOG_VA) / 1023);

  thermistor_r = ((temp * ANALOG_RVD) / (ANALOG_VA - temp));
  if (thermistor_r <= THERM_MIN_R)
    thermistor_r = THERM_MIN_R;

  temp = (1 / ((1 / THERM_BASE_DK)-((log(THERM_BASE_R / thermistor_r)) / THERM_BASE_B))) - THERM_BASE_DK;

  return temp;
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
void TEMP_TempCtrl(float temp_c, uint8_t pin)
{
  double now_time;
  g_TempData.TargetTemp_C = temp_c;
  g_TempData.PresentTemp_C = TEMP_ReadTemperature(pin);

  g_PidData.Input = g_TempData.PresentTemp_C;
  g_PidData.SetPoint = g_TempData.TargetTemp_C;
  g_Temp_PID.Compute();

  if (g_PidData.Output >= 0)
  {
    analogWrite(HEATER_CTRL, g_PidData.Output);
    analogWrite(PELTIER_CTRL, 0);
  }
  else
  {
    analogWrite(HEATER_CTRL, 0);
    analogWrite(PELTIER_CTRL, 0 - g_PidData.Output);
  }
/*
  if (millis() - g_PidData.PidStartTime >= 200)
  {
    Serial.print(F("NTC3:")); Serial.println(g_TempData.PresentTemp_C);//----------------
    Serial.print(F("Output:")); Serial.println(g_PidData.Output);//----------------
    g_PidData.PidStartTime = millis();
  }
*/
}

//--------------------------------------------------
void PID_Initial(void)
{
  g_PidData.PidStartTime = millis();
  g_PidData.SetPoint = 25;
  g_Temp_PID.SetOutputLimits(-255, 255);
  g_Temp_PID.SetMode(AUTOMATIC);
  g_Temp_PID.SetTunings(PID_KP, PID_KI, PID_KD);

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
