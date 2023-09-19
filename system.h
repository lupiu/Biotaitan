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

//--------------------------------------------------
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

//--------------------------------------------------

#endif