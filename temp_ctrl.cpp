//--------------------------------------------------
 /*
 * temp_ctrl.cpp
 *
 *  Created on: 2023/09/06
 *      Author: silva_lin
 */

 #include <Arduino_FreeRTOS.h>
 #include "Arduino.h" 
 #include "main.h"
 #include "temp_ctrl.h"
 #include <PID_v1.h>

//--------------------------------------------------
_TempData g_TempData = {IDLE, 25};
_PidData g_PidData;
PID g_Temp_PID(&g_PidData.Input, &g_PidData.Output, &g_PidData.SetPoint, 2, 5, 1, DIRECT);
//--------------------------------------------------
uint16_t TEMP_ReadTemperature(uint8_t pin)
{
  uint16_t temp;

  temp = analogRead(pin);
  return (temp * 16);
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
void TaskTempCtrl(void *pvParameters) 
{
  if (g_TempData.status != IDLE)
  {
    g_TempData.Temp_C = TEMP_ReadTemperature(NTC_0);
  }
}

//--------------------------------------------------
void PID_Initial(void)
{
  g_PidData.SetPoint = 100;
  g_Temp_PID.SetOutputLimits(0, 255);
  g_Temp_PID.SetMode(AUTOMATIC);

}

//--------------------------------------------------
void TEMP_Initial(void)
{
  pinMode(NTC_0, INPUT);
  pinMode(HEATER_CTRL, OUTPUT);
  pinMode(PELTIER_CTRL, OUTPUT);
  pinMode(FAN_CTRL, OUTPUT);

  PID_Initial();

  g_TempData.status = MEASURE;
 
  xTaskCreate(TaskTempCtrl,"Task1",128,NULL,1,NULL);
}

//--------------------------------------------------
