//--------------------------------------------------
/*
* lcm.cpp
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "lcm.h"

//--------------------------------------------------
spi_device_handle_t g_lcm_handle;
spi_device_handle_t g_sd_handle;
//--------------------------------------------------
void LCM_Init(void)
{
    spi_device_interface_config_t dev_cfg = {
        .mode = 3, // (0,0)
        .clock_speed_hz = 1000000,
        .spics_io_num = LCM_SPI_CSN,
        .queue_size = 64
    };

    ESP_ERROR_CHECK(spi_bus_add_device(SPI_MASTER_NUM, &dev_cfg, &g_lcm_handle));
        spi_device_interface_config_t dev_cfg1 = {
        .mode = 3, // (0,0)
        .clock_speed_hz = 2000000,
        .spics_io_num = 22,
        .queue_size = 64
    };

    ESP_ERROR_CHECK(spi_bus_add_device(SPI_MASTER_NUM, &dev_cfg1, &g_sd_handle));
}

//--------------------------------------------------
