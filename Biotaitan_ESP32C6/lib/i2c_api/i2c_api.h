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

//--------------------------------------------------
#define I2C0_MASTER_SCL_IO  GPIO_NUM_7
#define I2C0_MASTER_SDA_IO  GPIO_NUM_6
#define I2C0_MASTER_NUM     0
#define I2C0_MASTER_FREQ_HZ 400000

#define I2C_MASTER_TX_BUF_DISABLE   0
#define I2C_MASTER_RX_BUF_DISABLE   0
#define I2C_MASTER_TIMEOUT_MS       1000

//--------------------------------------------------
esp_err_t i2c_master_init(int port);

//--------------------------------------------------

#endif
