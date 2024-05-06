//--------------------------------------------------
/*
* spi_api.c
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "spi_api.h"

//--------------------------------------------------
SemaphoreHandle_t g_SPI_Semaphore;

//--------------------------------------------------
void SPI_Init(void)
{
    g_SPI_Semaphore = xSemaphoreCreateMutex();

    pinMode(LCM_SPI_CSN, OUTPUT);
    digitalWrite(LCM_SPI_CSN, HIGH);

    pinMode(SD_SPI_CSN, OUTPUT);
    digitalWrite(SD_SPI_CSN, HIGH);

}
