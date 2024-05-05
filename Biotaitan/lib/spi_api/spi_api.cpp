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
    spi_bus_config_t SPI_MASTER_buscfg  = {0};
    SPI_MASTER_buscfg.miso_io_num      = SPI_MASTER_MISO;
    SPI_MASTER_buscfg.mosi_io_num      = SPI_MASTER_MOSI;
    SPI_MASTER_buscfg.sclk_io_num      = SPI_MASTER_CLK;
    SPI_MASTER_buscfg.quadwp_io_num    = -1;
    SPI_MASTER_buscfg.quadhd_io_num    = -1;
    SPI_MASTER_buscfg.max_transfer_sz  = 0;

    ESP_ERROR_CHECK(spi_bus_initialize(SPI_MASTER_NUM, &SPI_MASTER_buscfg, SPI_DMA_CH_AUTO));
}
