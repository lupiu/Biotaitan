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

//--------------------------------------------------

//--------------------------------------------------
void app_main()
{
    i2c_master_init(I2C0_MASTER_NUM);
    OPT_Initial();
    //TEMP_Initial();

    while (1)
    {
        if (BTN_START == 0)
        {
            switch (OPMODE)
            {
                case SYS_LED_SINGLE :
                ESP_LOGI(TAG, "SYS_LED_SINGLE START");
                OPT_Test(0);
                ESP_LOGI(TAG, "SYS_LED_SINGLE END");
                break;

                case SYS_LED_DUAL :
                ESP_LOGI(TAG, "SYS_LED_DUAL START");
                OPT_Test(1);
                ESP_LOGI(TAG, "SYS_LED_DUAL END");
                break;

                case SYS_TEMP_MANUAL_H :
                case SYS_TEMP_MANUAL_L :
                ESP_LOGI(TAG, "SYS_TEMP_MANUAL START");
                //TEMP_Test(1);
                ESP_LOGI(TAG, "SYS_TEMP_MANUAL END");
                break;

                case SYS_TEMP_AUTO :
                ESP_LOGI(TAG, "SYS_TEMP_AUTO START");
                //TEMP_Test(0);
                ESP_LOGI(TAG, "SYS_TEMP_AUTO END");
                break;

                default :;
            }
        }
    }
}
