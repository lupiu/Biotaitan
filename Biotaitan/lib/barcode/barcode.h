//--------------------------------------------------
/*
* barcode.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _BARCODE_H__
#define _BARCODE_H__

#include "Arduino.h"
#include "driver/uart.h"

//--------------------------------------------------
//paramters define
#define BCR_RX    11
#define BCR_TX    12
#define BCR_TRIG  13
#define BCR_BEEP  21
#define BCR_OUT   35

#define BCR_RX_BUF 1024
//--------------------------------------------------
typedef struct
{
  uint8_t enable;  
  uint8_t datavalid;  
  uint16_t rxbytes;
  char data[BCR_RX_BUF];
}_BcrHandle;

//--------------------------------------------------
void BCR_Initial(void);
void BCR_Enable(void);
void BCR_Disable(void);
void BCR_Beep(int ms);
void BCR_GetStatus(uint8_t *enalbe, uint8_t *valid, uint16_t *rxbytes);
void BCR_GetData(char *data, uint16_t size);
//--------------------------------------------------

#endif
