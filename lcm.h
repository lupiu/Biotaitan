//--------------------------------------------------
/*
* lcm.h
*
*  Created on: 2023/09/14
*      Author: silva_lin
*/

#ifndef _LCM_H__
#define _LCM_H__

#include "Arduino.h"
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library
#include <LCDWIKI_TOUCH.h> //touch screen library

//--------------------------------------------------
//paramters define
#define LCM_MODEL ILI9488_18
#define LCM_CS   A5    
#define LCM_CD   A3
#define LCM_RST  A4
#define LCM_LED  A0   //if you don't need to control the LED pin,you should set it to -1 and set it to 3.3V

//touch screen paramters define
#define LCM_TCS   45
#define LCM_TCLK  46
#define LCM_TDOUT 47
#define LCM_TDIN  48
#define LCM_TIRQ  49

//--------------------------------------------------
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//--------------------------------------------------
#define BUTTON_SPACING_X 74
#define BUTTON_SPACING_Y 58
#define GRID_SPACING 5

#define LCM_WIDTH 480
#define LCM_HEIGHT 320

//--------------------------------------------------
#define LCM_MSG_TITLE 0
#define LCM_MSG_DATA 1

//--------------------------------------------------

typedef enum
{
  TOP_ENMODE = 0,
  TOP_USERMODE,
}_LcmTopGroupType;

typedef enum
{
  EM_TEMP = 0,
  EM_OPTIC,
}_LcmEMGroupType;

typedef struct
{
  uint8_t Name[10];
}_UiMenuType;

typedef struct
{
  uint8_t Name[10];
  uint8_t NameSize;
  uint16_t NameColor;
  uint16_t BgColor;
  uint16_t Px;
  uint16_t Py;
}_ButtonInfo;

_UiMenuType g_TopMenu[9] = 
{
  " ",
  " ",
  "USER MODE",
  " ",
  " ",
  "ENG MODE",
  " ",
  " ",
  "BACK"
};

_UiMenuType g_EMMenu[9] = 
{
  " ",
  " ",
  "F1",
  " ",
  " ",
  "F2",
  " ",
  " ",
  "BACK"
};

_ButtonInfo g_kb_btn[12] = 
{
  "1",    4,  BLACK,  BLUE, (GRID_SPACING + (0 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "2",    4,  BLACK,  BLUE, (GRID_SPACING + (1 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "3",    4,  BLACK,  BLUE, (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "DEL",  4,  BLACK,  BLUE, (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "4",    4,  BLACK,  BLUE, (GRID_SPACING + (0 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "5",    4,  BLACK,  BLUE, (GRID_SPACING + (1 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "6",    4,  BLACK,  BLUE, (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "ENT",  4,  BLACK,  BLUE, (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "7",    4,  BLACK,  BLUE, (GRID_SPACING + (0 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "8",    4,  BLACK,  BLUE, (GRID_SPACING + (1 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "9",    4,  BLACK,  BLUE, (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "0",    4,  BLACK,  BLUE, (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
};

_ButtonInfo g_func_btn[9] = 
{
  "F0", 4,  BLACK,  BLUE, (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F1", 4,  BLACK,  BLUE, (GRID_SPACING + (5 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F2", 4,  BLACK,  BLUE, (GRID_SPACING + (6 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F3", 4,  BLACK,  BLUE, (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F4", 4,  BLACK,  BLUE, (GRID_SPACING + (5 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F5", 4,  BLACK,  BLUE, (GRID_SPACING + (6 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F6", 4,  BLACK,  BLUE, (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (5 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F7", 4,  BLACK,  BLUE, (GRID_SPACING + (5 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (5 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F8", 4,  BLACK,  BLUE, (GRID_SPACING + (6 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (5 * (GRID_SPACING + BUTTON_SPACING_Y))),
};


//--------------------------------------------------

//--------------------------------------------------

#endif