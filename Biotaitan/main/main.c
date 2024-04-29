//--------------------------------------------------
/*
* main.c
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "main.h"

//--------------------------------------------------
static const char *TAG = "main";

#define OPMODE SYS_TEMP_AUTO
extern spi_device_handle_t g_lcm_handle;
extern spi_device_handle_t g_sd_handle;
//--------------------------------------------------
void LCM_Task(void * pvParametersoid)
{
    spi_transaction_t trans = {};

    trans.length = 8;
    trans.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA;
    trans.tx_data[0] = 0x55;

    while(1)
    {
        ESP_LOGI(TAG, "LCM_Task");
        ESP_ERROR_CHECK(spi_device_transmit(g_lcm_handle, &trans));
        //vTaskDelay(50 / portTICK_PERIOD_MS);
        trans.tx_data[0] = 0xC3;
        ESP_ERROR_CHECK(spi_device_transmit(g_sd_handle, &trans));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

//--------------------------------------------------
void app_main()
{
    SPI_Init();
    LCM_Init();
    //I2C_Init();
    //OPT_Initial();
    //TEMP_Initial();

    xTaskCreatePinnedToCore(LCM_Task, "LCM_Task", 4096, NULL, 0, NULL, tskNO_AFFINITY);
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
