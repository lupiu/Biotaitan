//--------------------------------------------------
/*
* i2c_api.c
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "i2c_api.h"

//--------------------------------------------------
i2c_master_bus_handle_t g_i2c_handle;

//--------------------------------------------------
i2c_master_bus_handle_t I2C_GetHandle(void)
{
    return g_i2c_handle;
}

//--------------------------------------------------
void I2C_Init(void)
{
    i2c_master_bus_config_t i2c_mst_config = {
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .i2c_port  = I2C0_MASTER_NUM,
    .sda_io_num = I2C0_MASTER_SDA_IO,
    .scl_io_num = I2C0_MASTER_SCL_IO,
    .glitch_ignore_cnt = 7,
    .flags.enable_internal_pullup = true,
    };  

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &g_i2c_handle));
}
