//--------------------------------------------------
/*
* temp_ctrl.cpp
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "Arduino.h"
#include "math.h"
#include "PID_v1.h"
#include "temp_ctrl.h"

//--------------------------------------------------
_TempCtrl g_TempData[TEMP_GROUP] = {0, 0, 0, 25, 25};
_PidData g_TempPidData[TEMP_GROUP];
PID g_Temp_PID1(&g_TempPidData[0].Input, &g_TempPidData[0].Output, &g_TempPidData[0].SetPoint, PID_KP, PID_KI, PID_KD, DIRECT);
PID g_Temp_PID2(&g_TempPidData[1].Input, &g_TempPidData[1].Output, &g_TempPidData[1].SetPoint, PID_KP, PID_KI, PID_KD, DIRECT);

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
void TEMP_PidCal(uint8_t ch,float temp_c)
{
  g_TempData[ch].TargetTemp_C = temp_c;
  g_TempData[ch].PresentTemp_C = TEMP_ReadTemperature(g_TempData[ch].NTC);

  g_TempPidData[ch].Input = g_TempData[ch].PresentTemp_C;
  g_TempPidData[ch].SetPoint = g_TempData[ch].TargetTemp_C;
  if (ch == 0)
  {
    g_Temp_PID1.Compute();
  }
  else
  {
    g_Temp_PID2.Compute();
  }
}

//--------------------------------------------------
void TEMP_PidCtrl(uint8_t ch)
{
  static double log_time = 0;

  if (g_TempData[ch].PresentTemp_C >= SAFTY_TOP_TEMP || g_TempData[ch].PresentTemp_C <= SAFTY_BOTTOM_TEMP)
  {
    Serial.println(F("Tempture measure fail!!!"));
    analogWrite(g_TempData[ch].Heater, 0);
    analogWrite(g_TempData[ch].Fan, 0);
    return;
  }

  if (g_TempPidData[ch].Output >= 0)
  {
    analogWrite(g_TempData[ch].Heater, g_TempPidData[ch].Output);
    analogWrite(g_TempData[ch].Fan, 0);
  }
  else
  {
    analogWrite(g_TempData[ch].Heater, 0);
    analogWrite(g_TempData[ch].Fan, 0 - g_TempPidData[ch].Output);
  }

  if (millis() - log_time >= 200)
  {
    Serial.print(F("CH1: ")); Serial.print(g_TempData[0].PresentTemp_C); Serial.print("\t"); Serial.print(F("Out: ")); Serial.print(g_TempPidData[0].Output); Serial.print("\t"); 
    Serial.print(F("CH2: ")); Serial.print(g_TempData[1].PresentTemp_C); Serial.print("\t"); Serial.print(F("Out: ")); Serial.println(g_TempPidData[1].Output);
    log_time = millis();
  }  
  
}

//--------------------------------------------------
uint8_t TEMP_CycleCtrl(uint8_t ch, float temp_c, uint8_t dir, uint16_t holdtime)
{
  uint8_t done = 0;
  
  TEMP_PidCal(ch, temp_c);
  if (g_TempData[ch].AchieveFlag == 0)
  {
    TEMP_PidCtrl(ch);
    if ((dir == HIGH && g_TempData[ch].PresentTemp_C >= temp_c) ||
       (dir == LOW && g_TempData[ch].PresentTemp_C <= temp_c))
    {
      g_TempData[ch].AchieveTime = millis();
      g_TempData[ch].AchieveFlag = 1;
    }
  }
  else
  {
    TEMP_PidCtrl(ch);
    if ((millis()  - g_TempData[ch].AchieveTime) >= holdtime)
    {
      g_TempData[ch].AchieveFlag = 0;
      done = 1;
    }
  }
  
  return done;
}

//--------------------------------------------------
void TEMP_AllOff(uint8_t ch)
{
  analogWrite(g_TempData[ch].Heater, 0);
  analogWrite(g_TempData[ch].Fan, 0);
}

//--------------------------------------------------
void TEMP_PidInitial(void)
{
  uint8_t i;

  for (i = 0; i < TEMP_GROUP; i++)
  {
    g_TempPidData[i].PidStartTime = millis();
    g_TempPidData[i].SetPoint = 25;
  }
  
  g_Temp_PID1.SetOutputLimits(-255, 255);
  g_Temp_PID1.SetMode(AUTOMATIC);
  g_Temp_PID1.SetTunings(PID_KP, PID_KI, PID_KD);
  g_Temp_PID2.SetOutputLimits(-255, 255);
  g_Temp_PID2.SetMode(AUTOMATIC);
  g_Temp_PID2.SetTunings(PID_KP, PID_KI, PID_KD);
}

//--------------------------------------------------
void TEMP_Initial(void)
{
  pinMode(NTC_CH1, INPUT);
  pinMode(NTC_CH2, INPUT);

  pinMode(HEATER_CTRL1, OUTPUT);
  analogWrite(HEATER_CTRL1, 0);
  pinMode(HEATER_CTRL2, OUTPUT);
  analogWrite(HEATER_CTRL2, 0);

  pinMode(FAN_CTRL1, OUTPUT);
  analogWrite(FAN_CTRL1, 0);
  pinMode(HEATER_CTRL2, OUTPUT);
  analogWrite(HEATER_CTRL2, 0);

  g_TempData[0].NTC = NTC_CH1;
  g_TempData[0].Heater = HEATER_CTRL1;
  g_TempData[0].Fan = FAN_CTRL1;
  g_TempData[1].NTC = NTC_CH2;
  g_TempData[1].Heater = HEATER_CTRL2;
  g_TempData[1].Fan = FAN_CTRL2;
  TEMP_PidInitial();
}

//--------------------------------------------------
