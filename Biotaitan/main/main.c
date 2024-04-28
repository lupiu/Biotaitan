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
void LCM_Task(void * pvParametersoid)
{
    while(1)
    {
        printf("MCP2515_send_Task loop\n " );
        vTaskDelay(1000);
    }
}

//--------------------------------------------------
void app_main()
{
    SPI_Init();
    LCM_Init();
    I2C_Init();
    OPT_Initial();
    //TEMP_Initial();

    xTaskCreatePinnedToCore(LCM_Task, "LCM_Task", 4096, NULL, 0, NULL, tskNO_AFFINITY);
    while (1);
}
