#ifndef LT768_LCD_h
#define LT768_LCD_h
#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include "spi_api.h"

#define Arduino_SPI     1 //1:use SPI to drive LT768
#define Arduino_IIC     0 //1:use IIC to drive LT768

class LT768LCD
{
	private:
		
	public:
	void SPIInit();	
	void SPISetCs(int cs);
	unsigned char SPIRwByte(unsigned char value);
	void SPI_CmdWrite(int cmd);
	void SPI_DataWrite(int data);
	void SPI_DataWrite_Pixel(int data);
	int SPI_StatusRead(void);
	int SPI_DataRead(void);
	void SPI_RegisterWrite(unsigned char Cmd,unsigned char Data);
	unsigned char SPI_RegisterRead(unsigned char Cmd);
	void IICInit();	
	void IIC_CmdWrite(int cmd);
	void IIC_DataWrite(int data);
	void IIC_DataWrite_Pixel(int data);
	int IIC_StatusRead(void);
	int IIC_DataRead(void);
	void IIC_RegisterWrite(unsigned char Cmd,unsigned char Data);
	unsigned char IIC_RegisterRead(unsigned char Cmd);
	void Parallel_Init(void);
	void LCD_CmdWrite(unsigned char cmd);
	void LCD_DataWrite(unsigned char data);
	void LCD_DataWrite_Pixel(unsigned int data);
	unsigned char LCD_StatusRead(void);
	unsigned int LCD_DataRead(void);
	void LCD_RegisterWrite(unsigned char Cmd,unsigned char Data);
	unsigned char LCD_RegisterRead(unsigned char Cmd);
};
extern LT768LCD LT768_LCD;
#endif
