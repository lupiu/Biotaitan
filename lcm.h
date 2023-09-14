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
typedef enum
{
    IDLE  = 0,
    MEASURE,
    PID_EN,
}_TempCtrlStatus;

typedef struct
{
     uint8_t ButtonName[10];
     uint8_t ButtonNameSize;
     uint16_t ButtonNameColor;
     uint16_t ButtonColor;
     uint16_t ButtonX;
     uint16_t ButtonY;     
}_ButtonInfo;

_ButtonInfo kb_button[12] = 
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

//--------------------------------------------------

//--------------------------------------------------

#endif