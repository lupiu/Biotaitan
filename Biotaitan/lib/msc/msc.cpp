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

const uint32_t SECTOR_SIZE = 512; //SECTOR_SIZE must be 512
const uint32_t SECTOR_COUNT = 2560; //
//--------------------------------------------------
int32_t onRead(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize)
{
    File f = SD.open("/disk.img", "r");
    if (!f) {
        Serial.println("Error: Failed to open /disk.img for read.");
        return 0;
    }

    f.seek(lba * SECTOR_SIZE + offset);

    int32_t bytesRead = f.read((uint8_t*)buffer, bufsize);
    f.close();
    return bytesRead;
}

//--------------------------------------------------
int32_t onWrite(uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize)
{
    File f = SD.open("/disk.img", "r+");
    if (!f) {
        Serial.println("Error: Failed to open /disk.img for write.");
        return 0;
    }

    f.seek(lba * SECTOR_SIZE + offset);

    int32_t bytesWritten = f.write(buffer, bufsize);
    f.close();
    return bytesWritten;
}

//--------------------------------------------------
bool onStartStop(uint8_t power_condition, bool start, bool load_eject)
{
    if (start) {
        Serial.println("USB MSC Started! (Computer connected)");
        // 可以執行一些初始化或同步操作
    } else {
        Serial.println("USB MSC Stopped! (Computer disconnected)");
        // 可以執行一些清理或同步操作
    }
    return true; // 返回 true 表示操作成功
}

//--------------------------------------------------
void MSC_Initial(void)
{
    uint64_t totalBytes = SD.cardSize();
    uint32_t availableSectors = totalBytes / SECTOR_SIZE;
    const uint32_t SECTOR_COUNT_VIRTUAL_DISK = 2560; // 約 1.25 MB

    if (availableSectors < SECTOR_COUNT_VIRTUAL_DISK) {
        while(true) {
            Serial.printf("FATAL ERROR: SD 卡可用空間不足 (%lu 扇區) 以創建虛擬磁碟 (%lu 扇區)！\n", availableSectors, SECTOR_COUNT_VIRTUAL_DISK);
            delay(5000); 
        }
    }

    spiSD.end();
    spiSD.begin(SPI_MASTER_CLK, SPI_MASTER_MISO, SPI_MASTER_MOSI, -1);
    Serial.print("檢查 /disk.img 檔案... ");
    if (!SD.exists("/disk.img")) {
        Serial.println("未找到。正在創建新的 /disk.img 檔案...");
        // 以寫入模式創建 /disk.img 檔案
        File f = SD.open("/disk.img", "w");
        if (f) {
            Serial.printf("正在初始化 /msc_disk.bin (大小: %lu 字節)...\n", SECTOR_COUNT_VIRTUAL_DISK * SECTOR_SIZE);
            for (uint32_t i = 0; i < SECTOR_COUNT_VIRTUAL_DISK * SECTOR_SIZE; i++) {
                f.write(0);
                if (i > 0 && (i % (1024 * 1024) == 0 || i == (SECTOR_COUNT_VIRTUAL_DISK * SECTOR_SIZE -1))) { 
                    Serial.printf("...寫入 %lu MB\n", i / (1024 * 1024));
                }
            }
            f.close();
            Serial.println("/disk.img 檔案已創建並初始化為空白。");
        } else {
            while(true) {
                Serial.println("FATAL ERROR: 無法創建 /disk.img 檔案！");
                delay(5000); 
            }
        }
    } else {
        Serial.println("找到現有的 /disk.img 檔案。");
    }

    Serial.println("配置 USB MSC ...");
    MSC.vendorID("BIOTAITAN");    // 設定供應商 ID (顯示在電腦裝置管理器中)
    MSC.productID("USB_Drive");   // 設定產品 ID (顯示為磁碟機名稱的一部分)
    MSC.productRevision("1.0");   // 設定產品版本
    MSC.onRead(onRead);           // 設定讀取回呼函數
    MSC.onWrite(onWrite);         // 設定寫入回呼函數
    MSC.onStartStop(onStartStop); // 設定開始/停止回呼函數
    MSC.mediaPresent(true);       // 告知電腦媒體USB存在
    MSC.isWritable(true);         // 告知電腦媒體可寫入

    MSC.begin(SECTOR_COUNT, SECTOR_SIZE);
    Serial.println("USB MSC 配置完成。");

    Serial.println("啟動 USB 介面...");
    USB.begin(); // 啟動 ESP32 的原生 USB OTG 介面

    Serial.println("--- USB MSC 裝置已啟動！請將 ESP32-S3 連接到電腦的 USB-OTG 埠。---");
    Serial.println("首次連接電腦可能提示格式化，請格式化為 FAT32。");
}