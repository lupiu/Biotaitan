//--------------------------------------------------
/*
* lcm.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _LCM_H__
#define _LCM_H__

#include "esp_err.h"
#include "spi_api.h"
#include <driver/gpio.h>

//--------------------------------------------------
//paramters define
#define LCM_SPI_CSN  GPIO_NUM_21

//--------------------------------------------------


//--------------------------------------------------
void LCM_Init(void);

//--------------------------------------------------

#endif
