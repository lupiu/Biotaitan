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
  SYSTEM_INIT = 0,
  SYSTEM_POR_P1,
  SYSTEM_POR_P2,
  SYSTEM_POR_P3,
  SYSTEM_POR_P4,
  SYSTEM_POR_P5,
  SYSTEM_POR_P6,
  LCM_TOP,
    LCM_USERMODE,
    LCM_ENGMODE,
}_SysOpMode;

typedef struct
{
  _SysOpMode status;
  double time;
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

//--------------------------------------------------
void SYS_Initial(void);
void SYS_SetOpmode(_SysOpMode status);
_SysOpMode SYS_GetOpmode(void);
//--------------------------------------------------

#endif