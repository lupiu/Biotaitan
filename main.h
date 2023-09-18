//--------------------------------------------------
/*
* main.h
*
*  Created on: 2023/09/06
*      Author: silva_lin
*/

#ifndef _MAIN_H__
#define _MAIN_H__

#include "Arduino.h"

//--------------------------------------------------
typedef enum
{
  LCM_TOP = 0,
    LCM_USERMODE,
    LCM_ENGMODE,
}_LcmDispPage;

_LcmDispPage g_UiPageStatus = 0;
//--------------------------------------------------

#endif