//--------------------------------------------------
/*
* main.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _MAIN_H__
#define _MAIN_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"

#include "i2c_api.h"
#include "spi_api.h"
//#include "temp_ctrl.h"
#include "optic.h"
#include "lcm.h"

//--------------------------------------------------
#define BTN_START  9

//--------------------------------------------------
typedef enum
{
  SYS_LED_SINGLE = 0,
  SYS_LED_DUAL,
  SYS_TEMP_MANUAL_H,
  SYS_TEMP_MANUAL_L,
  SYS_TEMP_AUTO,
}_SysOpmode;

//--------------------------------------------------

#endif
