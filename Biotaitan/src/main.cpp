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
#include "sd_card.h"
#include "spi_api.h"
#include "barcode.h"
#include "rtc_module.h"

//--------------------------------------------------

//--------------------------------------------------
void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    TEMP_Initial();
    OPT_Initial();

    SPI_Init();
    LCM_Initial();
    SD_Initial();
    BCR_Initial();
    RTC_Initial();

    SYS_Initial();
}

//--------------------------------------------------
void loop()
{

}
