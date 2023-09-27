//--------------------------------------------------
/*
* optic.cpp
*
*  Created on: 2023/09/14
*      Author: silva_lin
*/

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
void OPT_Initial(void)
{
  pinMode(OPT_LED1, OUTPUT);
  pinMode(OPT_LED2, OUTPUT);
  pinMode(OPT_LED3, OUTPUT);
  pinMode(OPT_LED4, OUTPUT);
  pinMode(OPT_PD1, INPUT);
  pinMode(OPT_PD2, INPUT);
  pinMode(OPT_PD3, INPUT);
  pinMode(OPT_PD4, INPUT);
}

//--------------------------------------------------
