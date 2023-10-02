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
  {" ",     NULL},
  {"USR",  NULL},
  {" ",     NULL},
  {"ENG",   LCM_DisplayEngMode},
  {"PT ",   LCM_DisplayPtMode},
  {"BACK",  LCM_DisplayTop}
};

_LcmMenuType g_EMMenu[6] = 
{
  {" ",     NULL},
  {" ",     NULL},
  {" ",     NULL},
  {" ",     NULL},
  {" ",     NULL},
  {"BACK",  LCM_DisplayTop}
};

_LcmMenuType g_PTMenu[6] = 
{
  {"NTC",   NULL},
  {"OPT",   NULL},
  {"HET",   NULL},
  {"COL",   NULL},
  {" ",     NULL},
  {"BACK",  LCM_DisplayTop}
};

_ButtonInfo g_kb_btn[15] = 
{
  "1",    3,  BLACK,  BLUE, (0 * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (3 * (GRID_SPACING + BUTTON_SPACING_Y)),
  "2",    3,  BLACK,  BLUE, (1 * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (3 * (GRID_SPACING + BUTTON_SPACING_Y)),
  "3",    3,  BLACK,  BLUE, (2 * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (3 * (GRID_SPACING + BUTTON_SPACING_Y)),
  "DEL",  2,  BLACK,  BLUE, (3 * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (3 * (GRID_SPACING + BUTTON_SPACING_Y)),
  "UP",   2,  BLACK,  BLUE, (4 * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (3 * (GRID_SPACING + BUTTON_SPACING_Y)),
  "4",    3,  BLACK,  BLUE, (0 * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (2 * (GRID_SPACING + BUTTON_SPACING_Y)),
  "5",    3,  BLACK,  BLUE, (1 * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (2 * (GRID_SPACING + BUTTON_SPACING_Y)),
  "6",    3,  BLACK,  BLUE, (2 * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (2 * (GRID_SPACING + BUTTON_SPACING_Y)),
  "0",    3,  BLACK,  BLUE, (3 * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (2 * (GRID_SPACING + BUTTON_SPACING_Y)),
  "DN",   2,  BLACK,  BLUE, (4 * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (2 * (GRID_SPACING + BUTTON_SPACING_Y)),
  "7",    3,  BLACK,  BLUE, (0 * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (1 * (GRID_SPACING + BUTTON_SPACING_Y)),
  "8",    3,  BLACK,  BLUE, (1 * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (1 * (GRID_SPACING + BUTTON_SPACING_Y)),
  "9",    3,  BLACK,  BLUE, (2 * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (1 * (GRID_SPACING + BUTTON_SPACING_Y)),
  ".",    3,  BLACK,  BLUE, (3 * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (1 * (GRID_SPACING + BUTTON_SPACING_Y)),
  "ENT",  2,  BLACK,  BLUE, (4 * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (1 * (GRID_SPACING + BUTTON_SPACING_Y)),
};

_ButtonInfo g_func_btn[6] = 
{
  "F0", 2,  BLACK,  BLUE, (GRID_SPACING + (5 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F1", 2,  BLACK,  BLUE, (GRID_SPACING + (6 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (2 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F2", 2,  BLACK,  BLUE, (GRID_SPACING + (5 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F3", 2,  BLACK,  BLUE, (GRID_SPACING + (6 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (3 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F4", 2,  BLACK,  BLUE, (GRID_SPACING + (5 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
  "F5", 2,  BLACK,  BLUE, (GRID_SPACING + (6 * (GRID_SPACING + BUTTON_SPACING_X))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_Y))),
};

//--------------------------------------------------
void LCM_DrawKeyBoardGrid(void)
{
  uint16_t i;

  g_LcmDisplay.Set_Draw_color(BLACK);

  for (i = 1; i <= 3; i++)
  {
    g_LcmDisplay.Draw_Fast_HLine(0, LCM_HEIGHT - 1 - (i * (GRID_SPACING + BUTTON_SPACING_Y)), (5 * (GRID_SPACING + BUTTON_SPACING_X)));
  }

  for (i = 1; i <= 5; i++)
  {
    g_LcmDisplay.Draw_Fast_VLine((i * (GRID_SPACING + BUTTON_SPACING_X)), LCM_HEIGHT - 1 - (3 * (GRID_SPACING + BUTTON_SPACING_Y)), LCM_HEIGHT - 1 - (3 * (GRID_SPACING + BUTTON_SPACING_Y)));
  }
}

//--------------------------------------------------
void LCM_DisplayGrid(void)
{
  uint16_t i;

  g_LcmDisplay.Fill_Screen(BLUE);
  g_LcmDisplay.Set_Draw_color(BLACK);

  //draw Massage Line
  g_LcmDisplay.Draw_Fast_HLine(0, LCM_MSG_HEIGHT, LCM_WIDTH);
  g_LcmDisplay.Draw_Fast_HLine(0, LCM_MSG_HEIGHT * 2, LCM_WIDTH);

  for (i = 0; i < 5; i++)
  {
    g_LcmDisplay.Draw_Fast_HLine(0, (i * (BUTTON_SPACING_Y + GRID_SPACING)), LCM_WIDTH);
  }

  LCM_DrawKeyBoardGrid();
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
    g_LcmDisplay.Print_String(g_kb_btn[i].Name, g_kb_btn[i].Px - strlen(g_kb_btn[i].Name) * g_kb_btn[i].NameSize * 6 / 2 + g_kb_btn[i].NameSize / 2 + 1 + BUTTON_SPACING_X /  2, g_kb_btn[i].Py - g_kb_btn[i].NameSize * 8 / 2 + g_kb_btn[i].NameSize / 2 + 1 + BUTTON_SPACING_Y /  2);
  }
}

//--------------------------------------------------
void LCM_DisplayFuncKey(_LcmMenuType *menu)
{
  uint16_t i;

  g_LcmDisplay.Set_Text_Mode(0);
  g_LcmDisplay.Set_Text_Back_colour(BLACK);

  for(i = 0; i < sizeof(g_func_btn) / sizeof(_ButtonInfo); i++)
  {
    g_LcmDisplay.Set_Text_Size(g_func_btn[i].NameSize);
    g_LcmDisplay.Set_Text_colour(g_func_btn[i].NameColor);
    g_LcmDisplay.Fill_Rect(g_func_btn[i].Px, g_func_btn[i].Py, BUTTON_SPACING_X - GRID_SPACING, BUTTON_SPACING_Y - GRID_SPACING, BLUE);
    g_LcmDisplay.Print_String(menu[i].Name, g_func_btn[i].Px - strlen(g_func_btn[i].Name) * g_func_btn[i].NameSize * 6 / 2 + g_func_btn[i].NameSize / 2 + 1 + BUTTON_SPACING_X /  2, g_func_btn[i].Py - g_func_btn[i].NameSize * 8 / 2 + g_func_btn[i].NameSize / 2 + 1 + BUTTON_SPACING_Y /  2);
  }
}

//--------------------------------------------------
void LCM_ShowString(uint8_t *str, uint8_t msg_type)
{
    g_LcmDisplay.Set_Text_Mode(1);
    g_LcmDisplay.Set_Text_Size(LCM_MSG_SIZE);
    g_LcmDisplay.Set_Text_colour(BLACK);
    g_LcmDisplay.Set_Text_Back_colour(BLACK);

    if (msg_type == LCM_MSG_TITLE)
    {
      g_LcmDisplay.Fill_Rect(GRID_SPACING + 2, 1 + ((0 * (GRID_SPACING + BUTTON_SPACING_Y))), LCM_WIDTH, LCM_MSG_HEIGHT - GRID_SPACING, BLUE);
      g_LcmDisplay.Print_String(str, GRID_SPACING + 2, 1 + (GRID_SPACING + (0 * (GRID_SPACING + BUTTON_SPACING_Y))));
    }
    else if (msg_type == LCM_MSG_PARAM)
    {
      g_LcmDisplay.Fill_Rect(GRID_SPACING + 2, 1 + (GRID_SPACING + (1 * (GRID_SPACING + BUTTON_SPACING_Y))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_X))), BUTTON_SPACING_Y - GRID_SPACING, BLUE);
      g_LcmDisplay.Print_String(str, GRID_SPACING + 2, 1 + (GRID_SPACING + (1 * (GRID_SPACING + BUTTON_SPACING_Y))));
    }
    else if (msg_type == LCM_MSG_VALUE)
    {
      g_LcmDisplay.Fill_Rect((GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_X))), 1 + (GRID_SPACING + (0 * (GRID_SPACING + BUTTON_SPACING_Y))), (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_X))), BUTTON_SPACING_Y - GRID_SPACING, BLUE);
      g_LcmDisplay.Print_String(str, (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_X))), 1 + (GRID_SPACING + (1 * (GRID_SPACING + BUTTON_SPACING_Y))));
    }
}

//--------------------------------------------------
void LCM_ShowMsg(uint8_t *str, uint8_t contd)
{
  static uint8_t msg_line = 0;

  g_LcmDisplay.Set_Text_Mode(0);
  g_LcmDisplay.Set_Text_Size(LCM_MSG_SIZE);
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
void LCM_DisplayPtMode(void)
{
  LCM_DisplayFuncKey(g_PTMenu);
  LCM_ShowString("Part Test Mode", LCM_MSG_TITLE);
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
void LCM_TouchScan(void)
{  
  uint16_t i;
  uint16_t px = 0;
  uint16_t py = 0;

  while(1)
  {
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

            case LCM_PTMODE :
              if (g_PTMenu[i].CallBack != NULL)
              {
                g_PTMenu[i].CallBack();
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
  g_LcmTouch.TP_Init(1, LCM_WIDTH, LCM_HEIGHT);

  g_LcmDisplay.Fill_Screen(BLUE);
  LCM_ShowMsg("System Initial...", 0);
}

//--------------------------------------------------
