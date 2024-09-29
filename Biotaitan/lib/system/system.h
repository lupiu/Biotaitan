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
#define BTN_START -1
#define SW_SEL3 -1
#define SW_SEL2 -1
#define SW_SEL1 -1

#define STATUS_LED 41
#define COVER_IN 16

#define OP_MODE SYS_TEMP_AUTO


//--------------------------------------------------
typedef enum
{
  SYS_LED_SINGLE = 0,
  SYS_LED_DUAL,
  SYS_TEMP_MANUAL_H,
  SYS_TEMP_MANUAL_L,
  SYS_TEMP_AUTO,
}_SysOpmode;

typedef enum
{
  SYS_INIT = 0,
  SYS_SELF_TEST,
  SYS_LOGIN,
  SYS_REGISTER,
  SYS_MAIN,
  SYS_RUNNIG,
}_SysStatus;

typedef struct
{
  u8_t ScnUpdated;
  _SysStatus Status;
}_SysHdl;

//--------------------------------------------------
void SYS_Initial(void);
_SysOpmode SYS_GetOpMode();
uint8_t SYS_GetBoardSel();
void SYS_ErrorReset();
void SYS_BtnF0(void);
void SYS_BtnF1(void);
void SYS_BtnF2(void);
void SYS_BtnF3(void);
void SYS_ChangeStatus(_SysStatus status);
//--------------------------------------------------

#endif