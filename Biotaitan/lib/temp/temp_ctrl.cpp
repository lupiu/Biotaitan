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
#include "optic.h"
#include "lcm.h"

//--------------------------------------------------
_TempCtrl g_TempData = {25, 25, 0};
_PidData g_TempPidData;
PID g_Temp_PID(&g_TempPidData.Input, &g_TempPidData.Output, &g_TempPidData.SetPoint, PID_KP, PID_KI, PID_KD, DIRECT);
uint8_t btn = 0;
uint8_t achieve_flag = 0;
//--------------------------------------------------
float TEMP_ReadTemperature(uint8_t pin)
{
  float temp;
  float thermistor_r;

  temp = analogRead(pin);
  temp = ((temp * ANALOG_VA) / 4095);

  thermistor_r = ((temp * ANALOG_RVD) / (ANALOG_VA - temp));
  if (thermistor_r <= THERM_MIN_R)
    thermistor_r = THERM_MIN_R;

  temp = (1 / ((1 / THERM_BASE_DK)-((log(THERM_BASE_R / thermistor_r)) / THERM_BASE_B))) - THERM_BASE_DK;

  return temp;
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
  if (g_TempData.PresentTemp_C >= SAFTY_TOP_TEMP || g_TempData.PresentTemp_C <= SAFTY_BOTTOM_TEMP)
  {
    Serial.println("");
    Serial.println(F("Tempture over safty range!!!"));
    SYS_ErrorReset();
  }

  if (g_TempPidData.Output >= 0)
  {
    analogWrite(HEATER_CTRL, g_TempPidData.Output);
    analogWrite(FAN_CTRL, 0);
  }
  else
  {
    analogWrite(HEATER_CTRL, HEATER_KEEP_PWM);
    analogWrite(FAN_CTRL, 0 - g_TempPidData.Output);
  }  
}

//--------------------------------------------------
uint8_t TEMP_CycleCtrl(float temp_c, uint8_t dir, double holdtime) //uint8_t TEMP_CycleCtrl(float temp_c, uint8_t dir, uint16_t holdtime)-Gaspard uint8_t TEMP_CycleCtrl(float temp_c, uint8_t dir, uint16_t holdtime)
{
  static double achieve_time;
  //static uint8_t achieve_flag = 0;
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
}

//--------------------------------------------------
void TEMP_Test(uint8_t mode)
{
  static uint8_t cycle_cnt = 0;
  static uint8_t dir = HIGH;
  static uint8_t base_initial = LOW;
  static double log_time = 0;
  static double new_time = 0;
  //static uint8_t btn = 0;
  static uint8_t loop_counter = 0;
  static bool is_difference_large = false;

  if (btn == 0)
  {
    cycle_cnt = 0;
    dir = HIGH;
    base_initial = LOW;
    log_time = 0;
    new_time = millis();
    loop_counter = 0;
    is_difference_large = false;
    btn = 1;
    achieve_flag = 0;
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
      if (cycle_cnt==0)
      {
        cycle_cnt=1;
      }
      else if (cycle_cnt==1)
      {
        if (dir == HIGH)
        {
          if (TEMP_CycleCtrl(INITIAL_TOP_TEMP, dir, INITIAL_TOP_HOLDTIME) == 1)
          {
            dir = LOW;
          }
        }
        else
        {
          if (TEMP_CycleCtrl(INITIAL_BOTTOM_TEMP, dir, INITIAL_BOTTOM_HOLDTIME) == 1)
          {
            dir = HIGH;
            cycle_cnt++;
          }
        }
      }
      else if (cycle_cnt>1 && cycle_cnt < (TEMP_CYCLE+1))
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
        if (loop_counter<250)
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
              if (g_TempData.PresentTemp_C>StopTemp)
              {
                TEMP_CycleCtrl(StopTemp, LOW, 0);
              }
              else
              {
                TEMP_AllOff();
                OPT_Led_OffAll();
              }
            loop_counter++;
            break;
          }
        }
        else
        {
          SYS_ChangeStatus(SYS_MAIN);
          //LCM_ShowInfoString("End!!!", 1);
          if (is_difference_large==true)
          {
            LCM_ShowInfoString_red("Positive", 1);
          }
          else
          {
            LCM_ShowInfoString_red("Negative", 1);
          }
          delay(1000);
          Serial.begin(115200);
          SYS_ChangeStatus(SYS_INIT);
          btn=0;
        }
      }
    } 

    if (millis() - log_time >= OPT_LIGHT_ON_TIME)
    {
      //Serial.print(F(" T:")); Serial.print(millis()); Serial.print("\t");
      //Serial.print(F("NTC_TS3: ")); Serial.print(g_TempData.PresentTemp_C); Serial.print("\t"); Serial.print(F("Out: ")); Serial.print(g_TempPidData.Output); Serial.print("\t");
      Serial.print(millis()-new_time,0);
      delay(8);
      Serial.print("\t");
      delay(5);
      Serial.print(g_TempData.PresentTemp_C*100,0);
      delay(7);
      Serial.print("\t");
      delay(5);
      Serial.print(cycle_cnt);
      delay(5);
      Serial.print("\t");
      delay(5);

      if (WITH_OPT == 1)
      {
        if (dir == HIGH )
        {
          if (g_TempData.PresentTemp_C >= RISE_OPT_ON_T && g_TempData.PresentTemp_C <= RISE_OPT_OFF_T)
          {
            OPT_Ctrl(OPT_ON_SEL, &is_difference_large, cycle_cnt);
          }
          else
          {
            OPT_Ctrl(4, &is_difference_large, cycle_cnt);
          }
        }
        else
        {
          if (g_TempData.PresentTemp_C <= FALL_OPT_ON_T && g_TempData.PresentTemp_C >= FALL_OPT_OFF_T)
          {
            OPT_Ctrl(OPT_ON_SEL, &is_difference_large, cycle_cnt);
          }
          else
          {
            OPT_Ctrl(4, &is_difference_large, cycle_cnt);
          }
        }
      }

      Serial.println();
      delay(5);
      Serial.flush();
      log_time = millis();
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
  pinMode(NTC_TS3, INPUT);

  pinMode(HEATER_CTRL, OUTPUT);
  analogWrite(HEATER_CTRL, 0);

  pinMode(FAN_CTRL, OUTPUT);
  analogWrite(FAN_CTRL, 0);

  TEMP_PidInitial();
}

//--------------------------------------------------
void ResetTempVariables(void)
{
  btn = 0;
}
