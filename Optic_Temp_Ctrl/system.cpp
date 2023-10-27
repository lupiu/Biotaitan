//--------------------------------------------------
/*
* system.cpp
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "Arduino.h"
#include "system.h"

//--------------------------------------------------
_SysOpmode g_SysOpMode;

//--------------------------------------------------
_SysOpmode SYS_GetOpMode()
{
  return g_SysOpMode;
}

//--------------------------------------------------
void SYS_Initial(void)
{
  pinMode(BTN_START, INPUT_PULLUP);
  pinMode(SW_SEL3, INPUT_PULLUP);
  pinMode(SW_SEL2, INPUT_PULLUP);
  pinMode(SW_SEL1, INPUT_PULLUP);

  g_SysOpMode = ((digitalRead(SW_SEL1) * 4) + (digitalRead(SW_SEL2) * 2) + digitalRead(SW_SEL3));
}

//--------------------------------------------------