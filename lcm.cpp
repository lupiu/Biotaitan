//--------------------------------------------------
/*
* lcm.cpp
*
*  Created on: 2023/09/14
*      Author: silva_lin
*/

#include <Arduino_FreeRTOS.h>
#include "Arduino.h" 
#include "lcm.h" 

//--------------------------------------------------
LCDWIKI_SPI g_LcmDisplay(LCM_MODEL, LCM_CS, LCM_CD, LCM_RST, LCM_LED);
LCDWIKI_TOUCH g_LcmTouch(LCM_TCS, LCM_TCLK, LCM_TDOUT, LCM_TDIN, LCM_TIRQ);
_LcmDispPage g_UiPageStatus = LCM_TOP;
//--------------------------------------------------
void LCM_DisplayGrid(void) 
{
  uint16_t i;

  g_LcmDisplay.Set_Draw_color(BLACK);
  for (i = 0; i < 5; i++)
  {
    g_LcmDisplay.Draw_Fast_HLine(0, (i * (BUTTON_SPACING_Y + GRID_SPACING)), GRID_SPACING);
  }

  g_LcmDisplay.Draw_Fast_VLine(0, 0, GRID_SPACING);
  g_LcmDisplay.Draw_Fast_VLine(LCM_WIDTH, 0, GRID_SPACING);

  for (i = 1; i < 7; i++)
  {
    g_LcmDisplay.Draw_Fast_VLine((i * (BUTTON_SPACING_X + GRID_SPACING)), (2 * (BUTTON_SPACING_Y + GRID_SPACING)), GRID_SPACING);
  }
}

//--------------------------------------------------
void LCM_DisplayKeyBoard(void) 
{
  uint16_t i;

  for(i = 0; i < sizeof(g_kb_btn) / sizeof(_ButtonInfo); i++)
  {
    g_LcmDisplay.Set_Text_Mode(1);
    g_LcmDisplay.Set_Text_Size(g_kb_btn[i].NameSize);
    g_LcmDisplay.Set_Text_colour(g_kb_btn[i].NameColor);
    g_LcmDisplay.Set_Text_Back_colour(BLACK);
    g_LcmDisplay.Print_String(g_kb_btn[i].Name, g_kb_btn[i].Px, g_kb_btn[i].Py);
  }
}

//--------------------------------------------------
void LCM_DisplayFuncKey(_UiMenuType *menu) 
{
  uint16_t i;

  for(i = 0; i < sizeof(g_func_btn) / sizeof(_ButtonInfo); i++)
  {
    g_LcmDisplay.Set_Text_Mode(1);
    g_LcmDisplay.Set_Text_Size(g_func_btn[i].NameSize);
    g_LcmDisplay.Set_Text_colour(g_func_btn[i].NameColor);
    g_LcmDisplay.Set_Text_Back_colour(BLACK);
    g_LcmDisplay.Print_String(menu[i].Name, g_func_btn[i].Px, g_func_btn[i].Py);
  }
}

//--------------------------------------------------
void LCN_ShowString(uint8_t *str, uint8_t msg_type)
{
    g_LcmDisplay.Set_Text_Mode(1);
    g_LcmDisplay.Set_Text_Size(4);
    g_LcmDisplay.Set_Text_colour(BLACK);
    g_LcmDisplay.Set_Text_Back_colour(BLACK);
    if (msg_type = LCM_MSG_TITLE)
      g_LcmDisplay.Print_String(str, GRID_SPACING, (GRID_SPACING + (0 * (GRID_SPACING + BUTTON_SPACING_Y))));
    else
      g_LcmDisplay.Print_String(str, GRID_SPACING, (GRID_SPACING + (1 * (GRID_SPACING + BUTTON_SPACING_Y))));
}

//--------------------------------------------------
boolean LCM_IsPressed(uint16_t px, uint16_t py, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if((px > x1 && px < x2) && (py > y1 && py < y2))
    {
        return true;  
    } 
    else
    {
        return false;  
    }
 }
//--------------------------------------------------
void TaskLcmCtrl(void *pvParameters) 
{  
  uint16_t i;
  uint16_t px = 0;
  uint16_t py = 0;

  g_LcmTouch.TP_Scan(0);
  if (g_LcmTouch.TP_Get_State() & TP_PRES_DOWN) 
  {
    px = g_LcmTouch.x;
    py = g_LcmTouch.y;

    for (i = 0; i < sizeof(g_func_btn) / sizeof(_ButtonInfo); i++)
    {
      if (LCM_IsPressed(px, py, g_func_btn[i].Px, g_func_btn[i].Py, (g_func_btn[i].Px + BUTTON_SPACING_X), (g_func_btn[i].Py + BUTTON_SPACING_Y)))
      {
        if (g_UiPageStatus)
        break;
      }
    }

    for (i = 0; i < sizeof(g_kb_btn) / sizeof(_ButtonInfo); i++)
    {
      if (LCM_IsPressed(px, py, g_kb_btn[i].Px, g_kb_btn[i].Py, (g_kb_btn[i].Px + BUTTON_SPACING_X), (g_kb_btn[i].Py + BUTTON_SPACING_Y)))
      {
        ///
        break;
      }
    }
  }
}

//--------------------------------------------------
void LCM_Initial(void)
{
  g_LcmDisplay.Init_LCD();
  g_LcmDisplay.Set_Rotation(0);
  g_LcmTouch.TP_Set_Rotation(1);
  g_LcmTouch.TP_Init(0, LCM_WIDTH, LCM_HEIGHT);

  g_LcmDisplay.Fill_Screen(BLUE);
  LCM_DisplayGrid();
  LCM_DisplayKeyBoard();

  LCM_DisplayFuncKey(g_TopMenu);  
  LCN_ShowString("System Initial...", LCM_MSG_TITLE);

  xTaskCreate(TaskLcmCtrl,"LCM Control",128,NULL,2,NULL);
}

//--------------------------------------------------
