//--------------------------------------------------
/*
* i2c_api.c
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "i2c_api.h"

//--------------------------------------------------

//--------------------------------------------------
esp_err_t i2c_master_init(int port) {
    int i2c_master_port = port;
    i2c_config_t conf;

    if (i2c_master_port == 0)
    {
        conf.mode = I2C_MODE_MASTER;
        conf.sda_io_num = I2C0_MASTER_SDA_IO;
        conf.scl_io_num = I2C0_MASTER_SCL_IO;
        conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
        conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
        conf.master.clk_speed = I2C0_MASTER_FREQ_HZ;
    }
    else
    {
        conf.mode = I2C_MODE_MASTER;
        conf.sda_io_num = I2C0_MASTER_SDA_IO;
        conf.scl_io_num = I2C0_MASTER_SCL_IO;
        conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
        conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
        conf.master.clk_speed = I2C0_MASTER_FREQ_HZ;
    }

    i2c_param_config(i2c_master_port, &conf);
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}
