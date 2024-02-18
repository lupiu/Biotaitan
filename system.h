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
  SYS_CH1 = 0,
  SYS_CH2,
  SYS_CH_MAX,
}_SysChannel;

typedef enum
{
  SYS_CH_INIT = 0,
  SYS_CH_PAT,
  SYS_CH_REA,
  SYS_CH_READY,
  SYS_CH_RUN,
}_SysChStatus;

typedef struct
{
  _SysChStatus Status[SYS_CH_MAX];
  char Patient_ID[SYS_CH_MAX][15];
  char Reagent_ID[SYS_CH_MAX][15];
  uint8_t Cycle[SYS_CH_MAX];
}_SysStatus;

//--------------------------------------------------
void SYS_ChangeTopMode(void);
void SYS_BtnCH1_SEL(void);
void SYS_BtnCH1_RE(void);
void SYS_BtnCH1_STA(void);
void SYS_BtnCH2_SEL(void);
void SYS_BtnCH2_RE(void);
void SYS_BtnCH2_STA(void);
void SYS_Initial(void);
void SYS_SystemRun(void);
//--------------------------------------------------

#endif