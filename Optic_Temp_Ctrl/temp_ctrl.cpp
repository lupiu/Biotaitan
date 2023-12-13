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
#include "system.h"

//--------------------------------------------------
_TempCtrl g_TempData = {25, 25, 0};
_PidData g_TempPidData;
PID g_Temp_PID(&g_TempPidData.Input, &g_TempPidData.Output, &g_TempPidData.SetPoint, PID_KP, PID_KI, PID_KD, DIRECT);

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
void TEMP_TecCtrl(uint8_t en)
{
  if (en)
  {
    analogWrite(TEC_FAN_CTRL, 255);
    analogWrite(TEC_CTRL, TEC_PWM);
  }
  else
  {
    analogWrite(TEC_FAN_CTRL, 0);
    analogWrite(TEC_CTRL, 0);
  }
}

//--------------------------------------------------
void TEMP_PidCal(float temp_c, uint8_t pin)
{
  g_TempData.TargetTemp_C = temp_c;
  g_TempData.PresentTemp_C = TEMP_ReadTemperature(pin);

  g_TempPidData.Input = g_TempData.PresentTemp_C;
  g_TempPidData.SetPoint = g_TempData.TargetTemp_C;
  g_Temp_PID.Compute();
}

//--------------------------------------------------
void TEMP_PidCtrl(uint8_t tec_en)
{
  static double log_time = 0;

  if (g_TempPidData.Output >= 0)
  {
    analogWrite(HEATER_CTRL, g_TempPidData.Output);
    analogWrite(FAN_CTRL, 0);
  }
  else
  {
    analogWrite(HEATER_CTRL, HEATER_KEEP_PWM);
    analogWrite(FAN_CTRL, 0 - g_TempPidData.Output);
    TEMP_TecCtrl(tec_en);
  }

  if (millis() - log_time >= 200)
  {
    Serial.print(F("NTC_TS3: ")); Serial.print(g_TempData.PresentTemp_C); Serial.print("\t"); Serial.print(F("Out: ")); Serial.println(g_TempPidData.Output);
    log_time <= millis();
  }  
  
}

//--------------------------------------------------
uint8_t TEMP_CycleCtrl(float temp_c, uint8_t dir, uint16_t holdtime)
{
  static double achieve_time;
  static uint8_t achieve_flag = 0;
  uint8_t done = 0;
  
  TEMP_PidCal(temp_c, NTC_TS3);
  if (achieve_flag == 0)
  {
    TEMP_PidCtrl(1);
    if ((dir == HIGH && g_TempData.PresentTemp_C >= temp_c) ||
       (dir == LOW && g_TempData.PresentTemp_C <= temp_c))
    {
      achieve_time = millis();
      achieve_flag = 1;
    }
  }
  else
  {
    TEMP_PidCtrl(0);
    if ((millis()  - achieve_time) >= holdtime)
    {
      achieve_flag = 0;
      done = 1;
    }
  }
  
  return done;
}

//--------------------------------------------------
void TEMP_AllOff(void)
{
  analogWrite(HEATER_CTRL, 0);
  analogWrite(FAN_CTRL, 0);
  TEMP_TecCtrl(0);
}

//--------------------------------------------------
void TEMP_Test(uint8_t mode)
{
  static uint8_t cycle_cnt = 0;
  static uint8_t dir = HIGH;
  static uint8_t base_initial = LOW;

  while(1)
  {
    if (mode == 1)
    {
      if (digitalRead(SW_SEL3) == 1)
      {
        TEMP_PidCal(TOP_TEMP, NTC_TS3);
        TEMP_PidCtrl(0);
      }
      else
      {
        TEMP_PidCal(BOTTOM_TEMP, NTC_TS3);
        TEMP_PidCtrl(1);
      }
    }
    else
    { 
      if (digitalRead(BTN_START) == 0)
      {
        cycle_cnt = 0;
        dir = HIGH;
        base_initial = LOW;
      }
      else
      {    
        if (BASE_ENABLE == 1 && base_initial == LOW)
        {
          if (TEMP_CycleCtrl(BASE_TEMP, HIGH, BASE_HOLDTIME) == 1)
          base_initial = HIGH;
        }
        else
        {
          if (cycle_cnt < TEMP_CYCLE)
          {
            if (dir == HIGH)
            {
              if (TEMP_CycleCtrl(TOP_TEMP, dir, TOP_HOLDTIME) == 1)
              {
                dir = LOW;
              }
            }
            else
            {
              if (TEMP_CycleCtrl(BOTTOM_TEMP, dir, BOTTOM_HOLDTIME) == 1)
              {
                dir = HIGH;
                cycle_cnt++;
              }
            }
          }
          else
          {
            switch (CYCLE_STATUS)
            {
              case 0 : 
                TEMP_PidCal(BASE_TEMP, NTC_TS3);
                TEMP_PidCtrl(0);
              break;
              case 1 : 
                TEMP_PidCal(BOTTOM_TEMP, NTC_TS3);
                TEMP_PidCtrl(0);
              break;
              case 2 : 
                TEMP_PidCal(TOP_TEMP, NTC_TS3);
                TEMP_PidCtrl(0);
              break;
              default : 
                TEMP_AllOff();
              break;
            }
          }
        } 
      }
    }
  }
}

//--------------------------------------------------
void TEMP_PidInitial(void)
{
  g_TempPidData.PidStartTime = millis();
  g_TempPidData.SetPoint = 25;
  g_Temp_PID.SetOutputLimits(-255, 255);
  g_Temp_PID.SetMode(AUTOMATIC);
  g_Temp_PID.SetTunings(PID_KP, PID_KI, PID_KD);
}

//--------------------------------------------------
void TEMP_Initial(void)
{
  pinMode(NTC_TS1, INPUT);
  pinMode(NTC_TS3, INPUT);

  pinMode(HEATER_CTRL, OUTPUT);
  analogWrite(HEATER_CTRL, 0);

  pinMode(FAN_CTRL, OUTPUT);
  analogWrite(FAN_CTRL, 0);

  pinMode(TEC_FAN_CTRL, OUTPUT);
  analogWrite(TEC_FAN_CTRL, 0);

  pinMode(TEC_CTRL, OUTPUT);
  analogWrite(TEC_CTRL, 0);

  TEMP_PidInitial();
}

//--------------------------------------------------
