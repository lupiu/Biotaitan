//--------------------------------------------------
/*
* sd_card.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _SD_CARD_H__
#define _SD_CARD_H__

#include "Arduino.h"
#include "SD.h"
#include "spi_api.h"

//--------------------------------------------------
//paramters define

//--------------------------------------------------
void SD_Initial(void);
void SD_Task(void * pvParametersoid);

//--------------------------------------------------

#endif
