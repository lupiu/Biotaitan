//--------------------------------------------------
/*
* optic.cpp
*
*  Created on: 2023/09/14
*      Author: silva_lin
*/

#include <Arduino_FreeRTOS.h>
#include "Arduino.h"
#include "optic.h"

//--------------------------------------------------


//--------------------------------------------------
void OPT_LedOn(uint8_t pin)
{
  digitalWrite(pin, 1);
}

//--------------------------------------------------
void OPT_LedOff(uint8_t pin)
{
  digitalWrite(pin, 0);
}

//--------------------------------------------------
uint16_t OPT_ReadPhotoDiode(uint8_t pin)
{
  uint16_t temp;

  temp = analogRead(pin);
  return (temp * 16); //TBD
}

//--------------------------------------------------
void TaskOpticCtrl(void *pvParameters) 
{  
  uint16_t i;

}

//--------------------------------------------------
void OPT_Initial(void)
{
  xTaskCreate(TaskOpticCtrl,"OPTIC Control",128,NULL,1,NULL);
}

//--------------------------------------------------
