//--------------------------------------------------
/*
* sd_card.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _SD_CARD_H__
#define _SD_CARD_H__

#include "Arduino.h"
#include "SPI.h"
#include "SD.h"
#include "esp_err.h"
#include "esp_log.h"
#include "spi_api.h"
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"



//--------------------------------------------------
//paramters define
#define SPI_MASTER_MISO  GPIO_NUM_37
#define SPI_MASTER_MOSI  GPIO_NUM_39
#define SPI_MASTER_CLK  GPIO_NUM_38
#define SD_SPI_CSN  GPIO_NUM_36

//--------------------------------------------------
void SD_Initial(void);

//--------------------------------------------------

#endif
