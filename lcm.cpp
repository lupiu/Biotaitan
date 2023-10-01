//--------------------------------------------------
/*
* lcm.cpp
*
*  Created on: 2023/09/14
*      Author: silva_lin
*/

#include "Arduino.h"
#include "lcm.h"
#include "system.h"

//--------------------------------------------------
LCDWIKI_SPI g_LcmDisplay(LCM_MODEL, LCM_CS, LCM_CD, LCM_RST, LCM_LED);
LCDWIKI_TOUCH g_LcmTouch(LCM_TCS, LCM_TCLK, LCM_TDOUT, LCM_TDIN, LCM_TIRQ);

_LcmMenuType g_TopMenu[6] = 
{
  {" ",         NULL},
  {"USER MODE", NULL},
  {" ",         NULL},
  {"ENG MODE",  LCM_DisplayEngMode},
  {" ",         NULL},
  {"BACK",      LCM_DisplayTop}
};

_LcmMenuType g_EMMenu[6] = 
{
  {" ",     NULL},
  {"START", NULL},
  {" ",     NULL},
  {"STOP",  NULL},
  {" ",     NULL},
  {"BACK",  LCM_DisplayTop}
};

_ButtonInfo g_kb_btn[15] = 
{
  "1",    4,  BLACK,  BLUE, (GRID_SPACING + (0 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "2",    4,  BLACK,  BLUE, (GRID_SPACING + (1 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "3",    4,  BLACK,  BLUE, (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "DEL",  3,  BLACK,  BLUE, (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "UP",   4,  BLACK,  BLUE, (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "4",    4,  BLACK,  BLUE, (GRID_SPACING + (0 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "5",    4,  BLACK,  BLUE, (GRID_SPACING + (1 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "6",    4,  BLACK,  BLUE, (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "0",    4,  BLACK,  BLUE, (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "DN",   4,  BLACK,  BLUE, (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "7",    4,  BLACK,  BLUE, (GRID_SPACING + (0 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "8",    4,  BLACK,  BLUE, (GRID_SPACING + (1 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "9",    4,  BLACK,  BLUE, (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
  ".",    4,  BLACK,  BLUE, (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "ENT",  3,  BLACK,  BLUE, (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
};

_ButtonInfo g_func_btn[6] = 
{
  "F0", 2,  BLACK,  BLUE, (GRID_SPACING + (5 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F1", 2,  BLACK,  BLUE, (GRID_SPACING + (6 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F2", 2,  BLACK,  BLUE, (GRID_SPACING + (5 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F3", 2,  BLACK,  BLUE, (GRID_SPACING + (6 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F4", 2,  BLACK,  BLUE, (GRID_SPACING + (5 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (5 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F5", 2,  BLACK,  BLUE, (GRID_SPACING + (6 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (5 * (GRID_SPACING + BUTTON_SPACING_Y))),
};

//--------------------------------------------------
void LCM_DisplayGrid(void)
{
  uint16_t i;

  g_LcmDisplay.Fill_Screen(BLUE);
  g_LcmDisplay.Set_Draw_color(BLACK);
  for (i = 0; i < 5; i++)
  {
    g_LcmDisplay.Draw_Fast_HLine(0, (i * (BUTTON_SPACING_Y + GRID_SPACING)), LCM_WIDTH);
  }

  g_LcmDisplay.Draw_Fast_VLine(0, 0, LCM_HEIGHT);
  g_LcmDisplay.Draw_Fast_VLine(LCM_WIDTH, 0, LCM_HEIGHT);

  for (i = 1; i < 7; i++)
  {
    g_LcmDisplay.Draw_Fast_VLine((i * (BUTTON_SPACING_X + GRID_SPACING)), (2 * (BUTTON_SPACING_Y + GRID_SPACING)), LCM_HEIGHT);
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
    //g_LcmDisplay.Print_String(g_kb_btn[i].Name, g_kb_btn[i].Px, g_kb_btn[i].Py);
    g_LcmDisplay.Print_String(g_kb_btn[i].Name, g_kb_btn[i].Px - strlen(g_kb_btn[i].Name) * g_kb_btn[i].NameSize * 6 / 2 + g_kb_btn[i].NameSize / 2 + 1 + BUTTON_SPACING_X /  2, g_kb_btn[i].Py - g_kb_btn[i].NameSize * 8 / 2 + g_kb_btn[i].NameSize / 2 + 1 + BUTTON_SPACING_Y /  2);
  }
}

//--------------------------------------------------
void LCM_DisplayFuncKey(_LcmMenuType *menu)
{
  uint16_t i;

  for(i = 0; i < sizeof(g_func_btn) / sizeof(_ButtonInfo); i++)
  {
    g_LcmDisplay.Set_Text_Mode(1);
    g_LcmDisplay.Set_Text_Size(g_func_btn[i].NameSize);
    g_LcmDisplay.Set_Text_colour(g_func_btn[i].NameColor);
    g_LcmDisplay.Set_Text_Back_colour(BLACK);
    g_LcmDisplay.Print_String(menu[i].Name, g_func_btn[i].Px - strlen(g_func_btn[i].Name) * g_func_btn[i].NameSize * 6 / 2 + g_func_btn[i].NameSize / 2 + 1 + BUTTON_SPACING_X /  2, g_func_btn[i].Py - g_func_btn[i].NameSize * 8 / 2 + g_func_btn[i].NameSize / 2 + 1 + BUTTON_SPACING_Y /  2);
  }
}

//--------------------------------------------------
void LCM_ShowString(uint8_t *str, uint8_t msg_type)
{
    g_LcmDisplay.Set_Text_Mode(1);
    g_LcmDisplay.Set_Text_Size(4);
    g_LcmDisplay.Set_Text_colour(BLACK);
    g_LcmDisplay.Set_Text_Back_colour(BLACK);
    if (msg_type == LCM_MSG_TITLE)
      g_LcmDisplay.Print_String(str, GRID_SPACING + 2, 1 + (GRID_SPACING + (0 * (GRID_SPACING + BUTTON_SPACING_Y))));
    else if (msg_type == LCM_MSG_PARAM)
      g_LcmDisplay.Print_String(str, GRID_SPACING + 2, 1 + (GRID_SPACING + (1 * (GRID_SPACING + BUTTON_SPACING_Y))));
    else if (msg_type == LCM_MSG_VALUE)
      g_LcmDisplay.Print_String(str, (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_X))), 1 + (GRID_SPACING + (1 * (GRID_SPACING + BUTTON_SPACING_Y))));
}

//--------------------------------------------------
void LCM_ShowMsg(uint8_t *str, uint8_t contd)
{
  static uint8_t msg_line = 0;

  g_LcmDisplay.Set_Text_Mode(1);
  g_LcmDisplay.Set_Text_Size(3);
  g_LcmDisplay.Set_Text_colour(BLACK);
  g_LcmDisplay.Set_Text_Back_colour(BLACK);

  if (contd == 0)
  {
    msg_line = 0;
  }

  if (msg_line == 0)
    g_LcmDisplay.Fill_Screen(BLUE);

  g_LcmDisplay.Print_String(str, GRID_SPACING, (GRID_SPACING + (msg_line * (GRID_SPACING + BUTTON_SPACING_Y))));
  msg_line++;
  if (msg_line >= 5)
    msg_line = 0;

}

//--------------------------------------------------
void LCM_DisplayEngMode(void) 
{
  LCM_DisplayFuncKey(g_EMMenu);
  LCM_ShowString("Engineering Mode", LCM_MSG_TITLE);
}

//--------------------------------------------------
void LCM_DisplayTop(void) 
{
  LCM_DisplayFuncKey(g_TopMenu);
  LCM_ShowString("Biotaitan System", LCM_MSG_TITLE);
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

  while(1)
  {
    if (SYS_GetOpmode() == SYSTEM_INIT)
    {
      continue;
    }

    g_LcmTouch.TP_Scan(0);
    if (g_LcmTouch.TP_Get_State() & TP_PRES_DOWN) 
    {
      px = g_LcmTouch.x;
      py = g_LcmTouch.y;

      for (i = 0; i < sizeof(g_func_btn) / sizeof(_ButtonInfo); i++)
      {
        if (LCM_IsPressed(px, py, g_func_btn[i].Px, g_func_btn[i].Py, (g_func_btn[i].Px + BUTTON_SPACING_X), (g_func_btn[i].Py + BUTTON_SPACING_Y)))
        {
          switch (SYS_GetOpmode())
          {
            case LCM_TOP :
              if (g_TopMenu[i].CallBack != NULL)
              {
                g_TopMenu[i].CallBack();
              }
            break;

            case LCM_ENGMODE :
              if (g_EMMenu[i].CallBack != NULL)
              {
                g_EMMenu[i].CallBack();
              }
            break;   
  
            default : ;  
          }
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
}

//--------------------------------------------------
void LCM_Initial(void)
{
  g_LcmDisplay.Init_LCD();
  g_LcmDisplay.Set_Rotation(1);
  g_LcmTouch.TP_Set_Rotation(1);
  g_LcmTouch.TP_Init(0, LCM_WIDTH, LCM_HEIGHT);

  g_LcmDisplay.Fill_Screen(BLUE);
  LCM_ShowMsg("System Initial...", 0);
}

//--------------------------------------------------
