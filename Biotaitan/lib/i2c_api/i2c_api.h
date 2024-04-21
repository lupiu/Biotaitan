//--------------------------------------------------
/*
* i2c_api.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _I2C_API_H__
#define _I2C_API_H__

#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

//--------------------------------------------------
#define I2C0_MASTER_SCL_IO  GPIO_NUM_7
#define I2C0_MASTER_SDA_IO  GPIO_NUM_6
#define I2C0_MASTER_NUM     0
#define I2C0_MASTER_FREQ_HZ 400000

//--------------------------------------------------
void I2C_Init(void);
i2c_master_bus_handle_t I2C_GetHandle(void);
//--------------------------------------------------

#endif
