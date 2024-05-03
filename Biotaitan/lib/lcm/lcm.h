//--------------------------------------------------
/*
* lcm.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _LCM_H__
#define _LCM_H__

#include "Arduino.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "spi_api.h"
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <lvgl.h>


//--------------------------------------------------
//paramters define
#define LCM_SPI_CSN  GPIO_NUM_40

//--------------------------------------------------


//--------------------------------------------------
void LCM_Initial(void);

//--------------------------------------------------

#endif
