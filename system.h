//--------------------------------------------------
/*
* system.h
*
*  Created on: 2023/09/14
*      Author: silva_lin
*/

#ifndef _SYSTEM_H__
#define _SYSTEM_H__

#include "Arduino.h"

//--------------------------------------------------
//paramters define
#define SYS_POR_P1T 5000
#define SYS_POR_P2T 10000
#define SYS_POR_P3T 5000
#define SYS_POR_P4T 5000
#define SYS_POR_P5T 5000
#define SYS_POR_P6T 5000

//--------------------------------------------------
typedef enum
{
  SYS_OK = 0,
  SYS_ERROR,
}_SysErrCode;

typedef enum
{
  SYS_INIT = 0,
  SYS_TOP,
    SYS_USERMODE,
    SYS_ENGMODE,
    SYS_PTMODE,
}_SysOpMode;

typedef struct
{
  _SysOpMode Status;
  double Time;
}_SysStatus;

typedef struct
{
  uint8_t Name[10];
  float Value;
}_PrmDataType;

typedef struct
{
  _PrmDataType Name;
}_PrmEngMode;

typedef struct
{
  uint8_t Name[10];
  _PrmDataType Mode[5];
}_PrmPtMode;

//--------------------------------------------------
void SYS_ChangeTopMode(void);
void SYS_ChangeEmMode(void);
void SYS_ChangePtMode(void);
void SYS_Initial(void);
void SYS_SetOpmode(_SysOpMode status);
_SysOpMode SYS_GetOpmode(void);
void SYS_SystemRun(void);
//--------------------------------------------------

#endif