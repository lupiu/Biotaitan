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

//--------------------------------------------------
void LCM_DrawGrid(void) 
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
void LCM_DrawKeyBoard(void) 
{
  uint16_t i;
   for(i = 0; i < sizeof(kb_button) / sizeof(_ButtonInfo); i++)
  {
    g_LcmDisplay.Set_Text_Mode(1);
    g_LcmDisplay.Set_Text_Size(kb_button[i].ButtonNameSize);
    g_LcmDisplay.Set_Text_colour(kb_button[i].ButtonNameColor);
    g_LcmDisplay.Set_Text_Back_colour(BLACK);
    g_LcmDisplay.Print_String(kb_button[i].ButtonName, kb_button[i].ButtonX, kb_button[i].ButtonY);
  }
}

//--------------------------------------------------
void TaskLcmCtrl(void *pvParameters) 
{  
  uint16_t i;
  px = 0;
  py = 0;
  g_LcmTouch.TP_Scan(0);
  if (g_LcmTouch.TP_Get_State() & TP_PRES_DOWN) 
  {
    px = g_LcmTouch.x;
    py = g_LcmTouch.y;
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
  LCM_DrawGrid();
  LCM_DrawKeyBoard();

  xTaskCreate(TaskLcmCtrl,"LCM Control",128,NULL,2,NULL);
}

//--------------------------------------------------
