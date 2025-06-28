//--------------------------------------------------
/*
* sd_card.cpp
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "sd_card.h"
#include "lcm.h"

SPIClass spiSD(HSPI);
//--------------------------------------------------
void SD_WriteLog(const char *str)
{
    if (xSemaphoreTake(g_SPI_Semaphore, portMAX_DELAY) == pdTRUE)
    {
        spiSD.end();
        spiSD.begin(SPI_MASTER_CLK, SPI_MASTER_MISO, SPI_MASTER_MOSI, -1);
        if (SD.begin(SD_SPI_CSN, spiSD)) {
            // 与SD卡通信
            File dataFile = SD.open("/Log.txt", FILE_APPEND);
            if (dataFile) {
                dataFile.println(str);
                dataFile.close();
                Serial.println("SD卡寫入成功！");
            } else {
                Serial.println("SD卡寫入失敗！");
            }
        } else {
            Serial.println("SD卡啟用失敗！");
        }
        xSemaphoreGive(g_SPI_Semaphore);
    }
    spiSD.endTransaction();
    spiSD.end();
    SPI.end();
    SPI.begin(SPI_MASTER_CLK, SPI_MASTER_MISO, SPI_MASTER_MOSI,-1);
}

//--------------------------------------------------
void SD_Initial(void)
{
    spiSD.begin(SPI_MASTER_CLK, SPI_MASTER_MISO, SPI_MASTER_MOSI, -1);

    pinMode(SD_SPI_CSN, OUTPUT);
    digitalWrite(SD_SPI_CSN, HIGH);

    // 初始化SD卡
    if (!SD.begin(SD_SPI_CSN, spiSD))
    {
        Serial.println("SD Card Initial Fail！");
        while(1);
    }
    Serial.println("SD Card Initial OK！");
    spiSD.end();
    SPI.end();
    SPI.begin(SPI_MASTER_CLK, SPI_MASTER_MISO, SPI_MASTER_MOSI,-1);
    spiSD.setHwCs(1);
}