//--------------------------------------------------
/*
* spi_api.c
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "spi_api.h"

//--------------------------------------------------
static const char *TAG = "spi";

//--------------------------------------------------
void SPI_Init(void)
{
    ESP_LOGI(TAG, "SPI_Init!");
    spi_bus_config_t SPI_MASTER_buscfg  = {
        .miso_io_num      = SPI_MASTER_MISO,
        .mosi_io_num      = SPI_MASTER_MOSI,
        .sclk_io_num      = SPI_MASTER_CLK,
        .quadwp_io_num    = -1,
        .quadhd_io_num    = -1,
        .max_transfer_sz  = 0,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(SPI_MASTER_NUM, &SPI_MASTER_buscfg, SPI_DMA_CH_AUTO));
}
