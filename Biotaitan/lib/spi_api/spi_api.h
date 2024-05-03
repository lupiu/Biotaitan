//--------------------------------------------------
/*
* spi_api.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _SPI_API_H__
#define _SPI_API_H__

#include "esp_err.h"
#include "esp_log.h"
#include "driver/spi_master.h"
#include "hal/gpio_types.h"

//--------------------------------------------------
#define SPI_MASTER_MISO  GPIO_NUM_37
#define SPI_MASTER_MOSI  GPIO_NUM_39
#define SPI_MASTER_CLK  GPIO_NUM_38
#define SPI_MASTER_NUM  SPI2_HOST

//--------------------------------------------------
void SPI_Init(void);
//--------------------------------------------------

#endif
