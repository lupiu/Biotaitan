//--------------------------------------------------
/*
* sd_card.cpp
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "sd_card.h"

//--------------------------------------------------

//--------------------------------------------------
//--------------------------------------------------
void SD_Task(void * pvParametersoid)
{
    while(1)
    {
        // 与SD卡通信
        File dataFile = SD.open("data.txt", FILE_WRITE);
        if (dataFile) {
            dataFile.println("Hello, SD card!");
            dataFile.close();
            Serial.println("数据写入到SD卡成功！");
        } else {
            Serial.println("无法打开文件！");
        }

        // 与另一个SPI设备通信
        //digitalWrite(otherDeviceSelectPin, LOW); // 启用设备
        // 在此处执行与另一个SPI设备相关的操作
        //digitalWrite(otherDeviceSelectPin, HIGH); // 禁用设备

        // 可以在此添加其他操作或延时
        delay(1000);
    }
}
//--------------------------------------------------
void SD_Initial(void)
{
    SPI.begin(SPI_MASTER_CLK, SPI_MASTER_MISO, SPI_MASTER_MOSI);
    pinMode(SD_SPI_CSN, OUTPUT);
    digitalWrite(SD_SPI_CSN, HIGH);

    // 初始化SD卡
    if (!SD.begin(SD_SPI_CSN)) {
    Serial.println("SD 卡初始化失败！");
    return;
    }
    Serial.println("SD 卡初始化成功！");

    xTaskCreatePinnedToCore(SD_Task, "SD_Task", 4096, NULL, 0, NULL, tskNO_AFFINITY);
}

//--------------------------------------------------
