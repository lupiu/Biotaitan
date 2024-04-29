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
#define SPI_MASTER_MISO  GPIO_NUM_18
#define SPI_MASTER_MOSI  GPIO_NUM_20
#define SPI_MASTER_CLK  GPIO_NUM_19
#define SPI_MASTER_NUM  SPI2_HOST

//--------------------------------------------------
void SPI_Init(void);
//--------------------------------------------------

#endif
