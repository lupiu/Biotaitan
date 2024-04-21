//--------------------------------------------------
/*
* optic.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _OPTIC_H__
#define _OPTIC_H__

#include <driver/gpio.h>
#include <esp_timer.h>
#include "i2c_api.h"
#include "ADS1115.h"

//--------------------------------------------------
//paramters define
#define OPT_LED1  GPIO_NUM_10
#define OPT_LED2  GPIO_NUM_11
#define OPT_LED3  GPIO_NUM_2
#define OPT_LED4  GPIO_NUM_3

#define OPT_ADS1115_PGA ADS1115_FSR_1_024
//ADS1115_FSR_6_144,
//ADS1115_FSR_4_096,
//ADS1115_FSR_2_048,
//ADS1115_FSR_1_024,
//ADS1115_FSR_0_512,
//ADS1115_FSR_0_256,

#define OPT_LIGHT_ON_TIME  120

//--------------------------------------------------


//--------------------------------------------------
void OPT_Initial(void);
void OPT_Test(uint8_t op_mode);
void OPT_Ctrl(void);

//--------------------------------------------------

#endif
