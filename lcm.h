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
#define LCM_CS   43    
#define LCM_CD   44
#define LCM_RST  42
#define LCM_LED  -1   //if you don't need to control the LED pin,you should set it to -1 and set it to 3.3V

//touch screen paramters define
#define LCM_TCS   48
#define LCM_TCLK  46
#define LCM_TDOUT 47
#define LCM_TDIN  45
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
#define BUTTON_SPACING_X 49
#define BUTTON_SPACING_Y 49
#define GRID_SPACING 1

#define LCM_WIDTH 480
#define LCM_HEIGHT 320

//--------------------------------------------------
#define LCM_MSG_TITLE 0
#define LCM_MSG_PARAM 1
#define LCM_MSG_VALUE 2

#define LCM_MSG_SIZE 3
#define LCM_MSG_HEIGHT (LCM_MSG_SIZE * 9)

//--------------------------------------------------
typedef void (*_Btn_cb)(void);

typedef struct
{
  uint8_t Name[10];
  _Btn_cb CallBack;
}_LcmMenuType;

typedef struct
{
  uint8_t Name[10];
  uint8_t NameSize;
  uint16_t NameColor;
  uint16_t BgColor;
  uint16_t Px;
  uint16_t Py;
}_ButtonInfo;

//--------------------------------------------------
void LCM_DisplayTop(void);
void LCM_DisplayEngMode(void);
void LCM_DisplayPtMode(void);
void LCM_DisplayGrid(void);
void LCM_DisplayKeyBoard(void);
void LCM_Initial(void);
void LCM_ShowMsg(uint8_t *str, uint8_t contd);
void LCM_TouchScan(void);
//--------------------------------------------------

#endif