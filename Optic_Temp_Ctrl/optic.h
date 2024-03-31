//--------------------------------------------------
/*
* optic.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _OPTIC_H__
#define _OPTIC_H__

#include "Arduino.h"

//--------------------------------------------------
//paramters define
#define OPT_LED1_INTENS  A0
#define OPT_LED2_INTENS  A1
#define OPT_LED3_INTENS  A2
#define OPT_LED4_INTENS  A3

#define OPT_LED1  5
#define OPT_LED2  6
#define OPT_LED3  10
#define OPT_LED4  11

#define OPT_ADS1115_PGA ADS1115_PGA_1_024
//ADS1115_PGA_6_144
//ADS1115_PGA_4_096
//ADS1115_PGA_2_048
//ADS1115_PGA_1_024
//ADS1115_PGA_0_512
//ADS1115_PGA_0_256

#define OPT_LIGHT_ON_TIME  1000

//--------------------------------------------------


//--------------------------------------------------
void OPT_Initial(void);
void OPT_Test(uint8_t op_mode);
void OPT_Ctrl(void);

//--------------------------------------------------

#endif