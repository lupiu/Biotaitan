//--------------------------------------------------
/*
* msc.cpp
*
*  Created on: 2025/06/18
*      Author: silva_lin
*/

#include "msc.h"

USBMSC MSC;
extern SPIClass spiSD;

//--------------------------------------------------
const uint32_t SECTOR_SIZE = 512; //SECTOR_SIZE must be 512

//--------------------------------------------------
void MSC_SpiRelease(void)
{
    spiSD.end();
    SPI.end();
    SPI.begin(SPI_MASTER_CLK, SPI_MASTER_MISO, SPI_MASTER_MOSI,-1);
    //spiSD.setFrequency(8000000); //for LCM
    spiSD.setHwCs(1);
}

//--------------------------------------------------
void MSC_UsbStart(void)
{
    SPI.end();
    SPI.begin(SPI_MASTER_CLK, SPI_MASTER_MISO, SPI_MASTER_MOSI,-1);
    spiSD.end();
    spiSD.begin(SPI_MASTER_CLK, SPI_MASTER_MISO, SPI_MASTER_MOSI, -1);
    //spiSD.setFrequency(4000000); //for SD Class 4
    if (!SD.begin(SD_SPI_CSN, spiSD)) {
        while(true) {
            Serial.println("MSC_UsbStart: SD Card Begin Fail！");
            delay(5000);
        }
    }
    USB.begin();
    MSC_SpiRelease();
    
}

//--------------------------------------------------
static int32_t MSC_OnRead(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize)
{
    uint32_t bytedata;

    SPI.end();
    SPI.begin(SPI_MASTER_CLK, SPI_MASTER_MISO, SPI_MASTER_MOSI,-1);
    spiSD.end();
    spiSD.begin(SPI_MASTER_CLK, SPI_MASTER_MISO, SPI_MASTER_MOSI, -1);
    //spiSD.setFrequency(4000000); //for SD Class 4
    if (!SD.begin(SD_SPI_CSN, spiSD)) {
        bytedata = -1 ;
    } else {
        bytedata = SD.readRAW( (uint8_t*) buffer, lba) ? SECTOR_SIZE : -1 ;
    }
    MSC_SpiRelease();
    return bytedata;
}

//--------------------------------------------------
static int32_t MSC_OnWrite(uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize)
{
    uint32_t bytedata;

    SPI.end();
    SPI.begin(SPI_MASTER_CLK, SPI_MASTER_MISO, SPI_MASTER_MOSI,-1);
    spiSD.end();
    spiSD.begin(SPI_MASTER_CLK, SPI_MASTER_MISO, SPI_MASTER_MOSI, -1);
    //spiSD.setFrequency(4000000); //for SD Class 4
    if (!SD.begin(SD_SPI_CSN, spiSD)) {
        bytedata = -1 ;
    } else {
        bytedata = SD.writeRAW( (uint8_t*) buffer, lba) ? SECTOR_SIZE : -1 ;
    }
    MSC_SpiRelease();
    return bytedata;
}

//--------------------------------------------------
bool MSC_OnStartStop(uint8_t power_condition, bool start, bool load_eject)
{
    if (start) {
        //Serial.println("USB MSC Started! (Computer connected)");
        // 可以執行一些初始化或同步操作
    } else {
        //erial.println("USB MSC Stopped! (Computer disconnected)");
        // 可以執行一些清理或同步操作
    }
    return true; // 返回 true 表示操作成功
}

//--------------------------------------------------
void MSC_Initial(void)
{
    spiSD.end();
    spiSD.begin(SPI_MASTER_CLK, SPI_MASTER_MISO, SPI_MASTER_MOSI, -1);
    //spiSD.setFrequency(4000000); // 保持在 4MHz 以便穩定初始化

    if (!SD.begin(SD_SPI_CSN, spiSD)) {
        while(true) {
            Serial.println("MSC: SD Card Initial Fail！");
            delay(5000);
        }
    }
    Serial.println("MSC: SD Card Initial OK！");

    MSC.vendorID("BioTMC");
    MSC.productID("USB_Drive");
    MSC.productRevision("1.0");
    MSC.onRead(MSC_OnRead);
    MSC.onWrite(MSC_OnWrite);
    MSC.onStartStop(MSC_OnStartStop);
    MSC.mediaPresent(true);
    MSC.isWritable(true);

    MSC.begin(SD.numSectors(), SECTOR_SIZE);
    Serial.println("USB MSC Config OK!");

    MSC_SpiRelease();
}