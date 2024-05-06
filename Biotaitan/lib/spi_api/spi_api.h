//--------------------------------------------------
/*
* spi_api.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _SPI_API_H__
#define _SPI_API_H__

#include "Arduino.h"
#include "SPI.h"

extern SemaphoreHandle_t g_SPI_Semaphore;
//--------------------------------------------------
#define SPI_MASTER_MISO  GPIO_NUM_37
#define SPI_MASTER_MOSI  GPIO_NUM_39
#define SPI_MASTER_CLK  GPIO_NUM_38
#define SPI_MASTER_NUM  SPI2_HOST

#define LCM_SPI_CSN  GPIO_NUM_40
#define SD_SPI_CSN  GPIO_NUM_36
//--------------------------------------------------
void SPI_Init(void);
//--------------------------------------------------

#endif
