//--------------------------------------------------
/*
* lcm.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _LCM_H__
#define _LCM_H__

#include "Arduino.h"
#include "esp_err.h"
#include "spi_api.h"
#include "LT768.h"
#include "LT768_LCD.h"
#include "LT768_Lib.h"
#include "TAMC_GT911.h"

//--------------------------------------------------
//paramters define
#define TOUCH_SDA GPIO_NUM_6
#define TOUCH_SCL GPIO_NUM_7
#define TOUCH_INT GPIO_NUM_42
#define TOUCH_RST GPIO_NUM_48
#define TOUCH_WIDTH  800
#define TOUCH_HEIGHT 480

//--------------------------------------------------
#define BUTTON_X 150
#define BUTTON_Y 80
#define BUTTON_SPACING_X 40
#define BUTTON_SPACING_Y 20

#define LCM_MSG_START_X 30
#define LCM_MSG_START_Y 60
#define LCM_MSG_SIZE 32 //32

//--------------------------------------------------
typedef struct
{
    uint8_t Name[10];
    uint8_t NameSize;
    uint16_t NameColor;
    uint16_t BgColor;
    uint16_t Px;
    uint16_t Py;
}_ButtonInfo;

typedef void (*_Btn_cb)(void);

typedef struct
{
    char Name[10];
    _Btn_cb CallBack;
}_LcmMenuType;

//--------------------------------------------------
/*color65k*/
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0
#define color65k_black   0x0000
#define color65k_white   0xffff
#define color65k_red     0xf800
#define color65k_green   0x07e0
#define color65k_blue    0x001f
#define color65k_yellow  color65k_red|color65k_green
#define color65k_cyan    color65k_green|color65k_blue
#define color65k_purple  color65k_red|color65k_blue

#define color65k_grayscale1    2113
#define color65k_grayscale2    2113*2
#define color65k_grayscale3    2113*3
#define color65k_grayscale4    2113*4
#define color65k_grayscale5    2113*5
#define color65k_grayscale6    2113*6
#define color65k_grayscale7    2113*7
#define color65k_grayscale8    2113*8
#define color65k_grayscale9    2113*9
#define color65k_grayscale10   2113*10
#define color65k_grayscale11   2113*11
#define color65k_grayscale12   2113*12
#define color65k_grayscale13   2113*13
#define color65k_grayscale14   2113*14
#define color65k_grayscale15   2113*15
#define color65k_grayscale16   2113*16
#define color65k_grayscale17   2113*17
#define color65k_grayscale18   2113*18
#define color65k_grayscale19   2113*19
#define color65k_grayscale20   2113*20
#define color65k_grayscale21   2113*21
#define color65k_grayscale22   2113*22
#define color65k_grayscale23   2113*23
#define color65k_grayscale24   2113*24
#define color65k_grayscale25   2113*25
#define color65k_grayscale26   2113*26
#define color65k_grayscale27   2113*27
#define color65k_grayscale28   2113*28
#define color65k_grayscale29   2113*29
#define color65k_grayscale30   2113*30

/*color16M*/
#define color16M_black   0x00000000
#define color16M_white   0x00ffffff
#define color16M_red     0x00ff0000
#define color16M_green   0x0000ff00
#define color16M_blue    0x000000ff
#define color16M_gray    0x00808080
#define color16M_yellow  color16M_red|color16M_green
#define color16M_cyan    color16M_green|color16M_blue
#define color16M_purple  color16M_red|color16M_blue

//--------------------------------------------------
void LCM_Initial(void);
void LCM_Task(void * pvParametersoid);
void LCM_ShowInfoString(char *str, uint8_t contd);
void LCM_ShowInfoString_red(char *str, uint8_t contd);
void LCM_DisplayFuncKey(_LcmMenuType *menu);
void LCM_TouchScan(uint8_t *num);
//--------------------------------------------------

#endif
