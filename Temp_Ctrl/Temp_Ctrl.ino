#include "Arduino.h"
#include "math.h"
#include <PID_v1.h>

//===================//
#define NTC_TS1 A4
#define NTC_TS3 A5
#define HEATER_CTRL 5
#define FAN_CTRL 6

#define IO2 7
#define IO1 4
#define IO0 2

#define PID_KP 300  // Proportional constant
#define PID_KI 20 // Integral Constant
#define PID_KD 5 // Derivative Constant

#define ANALOG_VA 5.0 //analog Voltage
#define ANALOG_RVD 8250 //analog resistor (ohm)
#define THERM_BASE_DK 273.15 //K@0C
#define THERM_BASE_R 30000 //R@0C
#define THERM_BASE_B 3455 //0~105C
#define THERM_MIN_R 127

#define BASE_ENABLE 1 //1:enable 0:disable //僅第一個Cycle有作用
#define BASE_TEMP 55 //degrees Celsius
#define BASE_HOLDTIME 2000 //unit:ms

#define TOP_TEMP 95
#define BOTTOM_TEMP 65
#define TOP_HOLDTIME 2000
#define BOTTOM_HOLDTIME 5000
#define TEMP_CYCLE 12

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
  float PresentTemp_C;
  float TargetTemp_C;
  double CycleTime;
}_TempCtrl;

//===================//
_TempCtrl g_TempData = {25, 25, 0};
_PidData g_PidData;
PID g_Temp_PID(&g_PidData.Input, &g_PidData.Output, &g_PidData.SetPoint, 2, 5, 1, DIRECT);

//===================//
void TEMP_AllOff(void)
{
  analogWrite(HEATER_CTRL, 0);
  analogWrite(FAN_CTRL, 0);
}

//===================//
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
void PID_Initial(void)
{
  g_PidData.PidStartTime = millis();
  g_PidData.SetPoint = 25;
  g_Temp_PID.SetOutputLimits(-255, 255);
  g_Temp_PID.SetMode(AUTOMATIC);
  g_Temp_PID.SetTunings(PID_KP, PID_KI, PID_KD);

}

//--------------------------------------------------
void TEMP_TempCtrl(float temp_c, uint8_t pin)
{
  g_TempData.TargetTemp_C = temp_c;
  g_TempData.PresentTemp_C = TEMP_ReadTemperature(pin);

  g_PidData.Input = g_TempData.PresentTemp_C;
  g_PidData.SetPoint = g_TempData.TargetTemp_C;
  g_Temp_PID.Compute();

  if (g_PidData.Output >= 0)
  {
    analogWrite(HEATER_CTRL, g_PidData.Output);
    analogWrite(FAN_CTRL, 0);
  }
  else
  {
    analogWrite(HEATER_CTRL, 0);
    analogWrite(FAN_CTRL, 0 - g_PidData.Output);
  }

  Serial.print(F("NTC_TS3: ")); Serial.print(g_TempData.PresentTemp_C); Serial.print("\t"); Serial.print(F("Out: ")); Serial.println(g_PidData.Output);
}

//--------------------------------------------------
uint8_t RunToTemp(float temp_c, uint8_t dir, uint16_t holdtime)
{
  static double achieve_time;
  static uint8_t achieve_flag = 0;
  uint8_t done = 0;
  
  TEMP_TempCtrl(temp_c, NTC_TS3);
  if (achieve_flag == 0)
  {
    if ((dir == HIGH && g_TempData.PresentTemp_C >= temp_c) ||
       (dir == LOW && g_TempData.PresentTemp_C <= temp_c))
    {
      achieve_time = millis();
      achieve_flag = 1;
    }
  }
  else
  {
    if ((millis()  - achieve_time) >= holdtime)
    {
      achieve_flag = 0;
      done = 1;
    }
  }
  
  return done;
}

//--------------------------------------------------
void setup() {
	Serial.begin(115200);

  pinMode(NTC_TS1, INPUT);
  pinMode(NTC_TS3, INPUT);
  pinMode(HEATER_CTRL, OUTPUT);
  pinMode(FAN_CTRL, OUTPUT);

  pinMode(IO2, INPUT_PULLUP);
  pinMode(IO1, INPUT_PULLUP);
  pinMode(IO0, INPUT_PULLUP);

  PID_Initial();
  TEMP_AllOff();
}

//--------------------------------------------------
void loop()
{
  uint8_t mode;
  static uint8_t cycle_cnt = 0;
  static uint8_t dir = HIGH;
  static uint8_t base_initial = LOW;

  mode = digitalRead(IO2);
  if (mode == 1)
  {
    if (digitalRead(IO0) == 1)
      TEMP_TempCtrl(TOP_TEMP, NTC_TS3);
    else
      TEMP_TempCtrl(BOTTOM_TEMP, NTC_TS3);
  }
  else
  { 
    if (digitalRead(IO0) == 1)
    {
      cycle_cnt = 0;
      dir = HIGH;
      base_initial = LOW;
    }
    else
    {    
      if (BASE_ENABLE == 1 && base_initial == LOW)
      {
        if (RunToTemp(BASE_TEMP, HIGH, BASE_HOLDTIME) == 1)
        base_initial = HIGH;
      }
      else
      {
        if (cycle_cnt < TEMP_CYCLE)
        {
          if (dir == HIGH)
          {
            if (RunToTemp(TOP_TEMP, dir, TOP_HOLDTIME) == 1)
            {
              dir = LOW;
            }
          }
          else
          {
            if (RunToTemp(BOTTOM_TEMP, dir, BOTTOM_HOLDTIME) == 1)
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
              TEMP_TempCtrl(BASE_TEMP, NTC_TS3);
            break;
            case 1 : 
              TEMP_TempCtrl(BOTTOM_TEMP, NTC_TS3);
            break;
            case 2 : 
              TEMP_TempCtrl(TOP_TEMP, NTC_TS3);
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
