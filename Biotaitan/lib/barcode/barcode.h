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

//--------------------------------------------------


//--------------------------------------------------
void BCR_Initial(void);
void BCR_Enable(void);
void BCR_Disable(void);
//--------------------------------------------------

#endif
