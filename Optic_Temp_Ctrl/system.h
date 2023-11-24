//--------------------------------------------------
/*
* system.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _SYSTEM_H__
#define _SYSTEM_H__

#include "Arduino.h"

//--------------------------------------------------
#define BTN_START 25
#define SW_SEL3 24
#define SW_SEL2 23
#define SW_SEL1 22

//--------------------------------------------------
typedef enum
{
  SYS_LED_SINGLE = 0,
  SYS_LED_DUAL,
  SYS_TEMP_MANUAL_H,
  SYS_TEMP_MANUAL_L,
  SYS_TEMP_AUTO,
}_SysOpmode;

//--------------------------------------------------
void SYS_Initial(void);
_SysOpmode SYS_GetOpMode();

//--------------------------------------------------

#endif