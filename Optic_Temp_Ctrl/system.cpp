//--------------------------------------------------
/*
* system.cpp
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "Arduino.h"
#include "system.h"
#include "temp_ctrl.h"

//--------------------------------------------------
_SysOpmode g_SysOpMode;
uint8_t g_BoardSel = 0;

//--------------------------------------------------
_SysOpmode SYS_GetOpMode()
{
  return g_SysOpMode;
}

//--------------------------------------------------
uint8_t SYS_GetBoardSel()
{
  return g_BoardSel;
}

//--------------------------------------------------
void(* resetFunc) (void) = 0;

//--------------------------------------------------
void SYS_ErrorReset()
{
  analogWrite(HEATER_CTRL, 0);
  analogWrite(FAN_CTRL, 0);
  Serial.println(F("System Reset!!!"));
  delay(10);
  resetFunc();
}

//--------------------------------------------------
void SYS_Initial(void)
{
  pinMode(BTN_START, INPUT_PULLUP);
  pinMode(SW_SEL3, INPUT_PULLUP);
  pinMode(SW_SEL2, INPUT_PULLUP);
  pinMode(SW_SEL1, INPUT_PULLUP);

  g_SysOpMode = ((digitalRead(SW_SEL1) * 4) + (digitalRead(SW_SEL2) * 2) + digitalRead(SW_SEL3));
  if (g_SysOpMode == 7)
  {
    g_BoardSel = 1;
    g_SysOpMode = OP_MODE;
  }

}

//--------------------------------------------------