//--------------------------------------------------
/*
* optic.h
*
*  Created on: 2023/09/14
*      Author: silva_lin
*/

#ifndef _OPTIC_H__
#define _OPTIC_H__

#include "Arduino.h"

//--------------------------------------------------
//paramters define
#define OPT_LED1  22
#define OPT_LED2  23
#define OPT_LED3  24
#define OPT_LED4  25

#define OPT_PD1  A8
#define OPT_PD2  A9
#define OPT_PD3  A10
#define OPT_PD4  A11

//--------------------------------------------------


//--------------------------------------------------
void OPT_Initial(void);
void OPT_LedOn(uint8_t pin);
void OPT_LedOff(uint8_t pin);
uint16_t OPT_ReadPhotoDiode(uint8_t pin);
//--------------------------------------------------

#endif