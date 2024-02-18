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
#define BAR_TX  1
#define BAR_RX  0
//--------------------------------------------------
void BAR_Initial(void);
void BAR_Read(void);
//--------------------------------------------------

#endif