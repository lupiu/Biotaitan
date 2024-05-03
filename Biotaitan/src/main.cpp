//--------------------------------------------------
/*
* main.cpp
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "Arduino.h"
#include "system.h"
#include "temp_ctrl.h"
#include "optic.h"
#include "lcm.h"
#include "spi_api.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


//--------------------------------------------------
void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    //SYS_Initial();
    //TEMP_Initial();
    //OPT_Initial();

    SPI_Init();
    LCM_Initial();
}

//--------------------------------------------------
void loop()
{
    
}