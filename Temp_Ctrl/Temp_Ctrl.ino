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

#define TOP_TEMP 50
#define BOTTOM_TEMP 30
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
  double now_time;
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
}

//--------------------------------------------------
void loop()
{
  uint8_t dir;

  dir = digitalRead(IO0);
  if (dir == 0)
    TEMP_TempCtrl(BOTTOM_TEMP, NTC_TS3);
  else
    TEMP_TempCtrl(TOP_TEMP, NTC_TS3);

}
