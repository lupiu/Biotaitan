#include "Arduino.h"
#include "LT768_LCD.h"
#include <Wire.h>
#include <SPI.h>

// ------------------------------------------------------------ SPI Drive --------------------------------------------------------------------
#if Arduino_SPI
void LT768LCD::SPIInit()
{
	pinMode(LCM_SPI_CSN, OUTPUT);
	SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  	SPI.begin(SPI_MASTER_CLK, SPI_MASTER_MISO, SPI_MASTER_MOSI);
}
void LT768LCD::SPISetCs(int cs)
{
	if(cs)
		digitalWrite(LCM_SPI_CSN,HIGH);
	else
	  digitalWrite(LCM_SPI_CSN,LOW);
}
unsigned char LT768LCD::SPIRwByte(unsigned char value)
{
	unsigned char rec;
	rec = SPI.transfer(value);
	return rec;
}
void LT768LCD::SPI_CmdWrite(int cmd)
{
  LT768_LCD.SPISetCs(0);    //SS_RESET;
  LT768_LCD.SPIRwByte(0x00);
  LT768_LCD.SPIRwByte(cmd);
  LT768_LCD.SPISetCs(1);    //SS_SET;
}
void LT768LCD::SPI_DataWrite(int data)
{
  LT768_LCD.SPISetCs(0);    //SS_RESET;
  LT768_LCD.SPIRwByte(0x80);
  LT768_LCD.SPIRwByte(data);
  LT768_LCD.SPISetCs(1);    //SS_SET;
}
void LT768LCD::SPI_DataWrite_Pixel(int data)
{
  LT768_LCD.SPISetCs(0);    //SS_RESET;
  LT768_LCD.SPIRwByte(0x80);
  LT768_LCD.SPIRwByte(data);
  LT768_LCD.SPISetCs(1);    //SS_SET;
  
  LT768_LCD.SPISetCs(0);    //SS_RESET;
  LT768_LCD.SPIRwByte(0x80);
  LT768_LCD.SPIRwByte(data>>8);
  LT768_LCD.SPISetCs(1);    //SS_SET;
}
int LT768LCD::SPI_StatusRead(void)
{
  int temp = 0;
  LT768_LCD.SPISetCs(0);    //SS_RESET;
  LT768_LCD.SPIRwByte(0x40);
  temp = LT768_LCD.SPIRwByte(0x00);
  LT768_LCD.SPISetCs(1);    //SS_SET;
  return temp;
}

int LT768LCD::SPI_DataRead(void)
{
  int temp = 0;
  LT768_LCD.SPISetCs(0);    //SS_RESET;
  LT768_LCD.SPIRwByte(0xc0);
  temp = LT768_LCD.SPIRwByte(0x00);
  LT768_LCD.SPISetCs(1);    //SS_SET;
  return temp;
}
#endif
// ------------------------------------------------------------ IIC Drive --------------------------------------------------------------------
#if Arduino_IIC
void LT768LCD::IICInit()
{`
	Wire.begin();
	Wire.setClock(400000);
}
void LT768LCD::IIC_CmdWrite(int cmd)
{
  Wire.beginTransmission(0x1e);
  Wire.write(cmd);
  Wire.endTransmission();
}
void LT768LCD::IIC_DataWrite(int data)
{
  Wire.beginTransmission(0x1f);
  Wire.write(data);
  Wire.endTransmission();
}
void LT768LCD::IIC_DataWrite_Pixel(int data)
{
  Wire.beginTransmission(0x1f);
  Wire.write(data);
  Wire.endTransmission();

  Wire.beginTransmission(0x1f);
  Wire.write(data>>8);
  Wire.endTransmission();
}
int LT768LCD::IIC_StatusRead(void)
{
  int temp = 0; 
  temp = Wire.requestFrom(0x1e,1);
  while(Wire.available())
  { 
    temp = Wire.read();
  }
  return temp;
}

int LT768LCD::IIC_DataRead(void)
{
  int temp = 0;   
  temp = Wire.requestFrom(0x1f,1);
  while(Wire.available())
  { 
    temp = Wire.read();
  }
  return temp;
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------

void LT768LCD::Parallel_Init(void)
{
	#if Arduino_SPI
	LT768_LCD.SPIInit();
	#endif
	
	#if Arduino_IIC
	LT768_LCD.IICInit();
	#endif
}
void LT768LCD::LCD_CmdWrite(unsigned char cmd)
{
	#if Arduino_SPI
	LT768_LCD.SPI_CmdWrite(cmd);
	#endif
	
	#if Arduino_IIC
	LT768_LCD.IIC_CmdWrite(cmd);
	#endif
}

void LT768LCD::LCD_DataWrite(unsigned char data)
{
	#if Arduino_SPI
	LT768_LCD.SPI_DataWrite(data);
	#endif
	
	#if Arduino_IIC
	LT768_LCD.IIC_DataWrite(data);
	#endif
}

void LT768LCD::LCD_DataWrite_Pixel(unsigned int data)
{
	#if Arduino_SPI
	LT768_LCD.SPI_DataWrite_Pixel(data);
	#endif
	
	#if Arduino_IIC
	LT768_LCD.IIC_DataWrite_Pixel(data);
	#endif
}


unsigned char LT768LCD::LCD_StatusRead(void)
{
	unsigned char temp = 0;
	
	#if Arduino_SPI
	temp = LT768_LCD.SPI_StatusRead();
	#endif
	
	#if Arduino_IIC
	temp = LT768_LCD.IIC_StatusRead();
	#endif
	
	return temp;
}

unsigned int LT768LCD::LCD_DataRead(void)
{
	unsigned int temp = 0;

	#if Arduino_SPI
	temp = LT768_LCD.SPI_DataRead();
	#endif
	
	#if Arduino_IIC
	temp = LT768_LCD.IIC_DataRead();
	#endif
	
	return temp;
}
void LT768LCD::LCD_RegisterWrite(unsigned char Cmd,unsigned char Data)
{
	LT768_LCD.LCD_CmdWrite(Cmd);
	LT768_LCD.LCD_DataWrite(Data);
}  
//---------------------//
unsigned char LT768LCD::LCD_RegisterRead(unsigned char Cmd)
{
	unsigned char temp;
	
	LT768_LCD.LCD_CmdWrite(Cmd);
	temp=LT768_LCD.LCD_DataRead();
	return temp;
}
LT768LCD LT768_LCD=LT768LCD();
