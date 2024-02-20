//--------------------------------------------------
/*
* barcode.h
*
*  Created on: 2023/09/14
*      Author: silva_lin
*/

#ifndef _BARCODE_H__
#define _BARCODE_H__

#include "Arduino.h"

//--------------------------------------------------
//paramters define
#define BAR_TX  14
#define BAR_RX  15
#define BAR_TRIG  16

#define BAR_TIMEOUT  200

#define BAR_OK  0
#define BAR_ERROR  1

#define BAR_ON   0
#define BAR_OFF  1

#define BAR_LENGTH  64
//--------------------------------------------------

//--------------------------------------------------
void BAR_Initial(void);
int BAR_Read(char *id);
void BAR_AutoOn(void);
void BAR_TrigOn(void);
void BAR_TrigOff(void);
//--------------------------------------------------

#endif