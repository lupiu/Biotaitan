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

_ButtonInfo g_func_btn[4] = 
{
  "F0", 2,  BLACK,  BLUE, (0 * (GRID_SPACING + FUNCTION_SPACING_X)), LCM_HEIGHT - (1 * (GRID_SPACING + FUNCTION_SPACING_Y)),
  "F1", 2,  BLACK,  BLUE, (1 * (GRID_SPACING + FUNCTION_SPACING_X)), LCM_HEIGHT - (1 * (GRID_SPACING + FUNCTION_SPACING_Y)),
  "F2", 2,  BLACK,  BLUE, (2 * (GRID_SPACING + FUNCTION_SPACING_X)), LCM_HEIGHT - (1 * (GRID_SPACING + FUNCTION_SPACING_Y)),
  "F3", 2,  BLACK,  BLUE, (3 * (GRID_SPACING + FUNCTION_SPACING_X)), LCM_HEIGHT - (1 * (GRID_SPACING + FUNCTION_SPACING_Y)),
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
    g_LcmDisplay.Draw_Fast_VLine((i * (GRID_SPACING + BUTTON_SPACING_X)) - 1, LCM_HEIGHT - 1 - (3 * (GRID_SPACING + BUTTON_SPACING_Y)), LCM_HEIGHT - 1 - (3 * (GRID_SPACING + BUTTON_SPACING_Y)));
  }
}

//--------------------------------------------------
void LCM_DrawFuncKeyGrid(void)
{
  uint16_t i;

  g_LcmDisplay.Set_Draw_color(BLACK);

  g_LcmDisplay.Draw_Fast_HLine(0, LCM_HEIGHT - 1 - (1 * (GRID_SPACING + FUNCTION_SPACING_Y)), LCM_WIDTH);
  g_LcmDisplay.Draw_Fast_HLine(0, LCM_CHMSG_Y, LCM_WIDTH);

  g_LcmDisplay.Draw_Fast_VLine(LCM_WIDTH / 2, LCM_HEIGHT - 1 - (3 * (GRID_SPACING + FUNCTION_SPACING_Y)), LCM_HEIGHT);
  for (i = 1; i <= 3; i++)
  {
    g_LcmDisplay.Draw_Fast_VLine(( i * (GRID_SPACING + FUNCTION_SPACING_X)), LCM_HEIGHT - 1 - (1 * (GRID_SPACING + BUTTON_SPACING_Y)), LCM_HEIGHT - 1 - (1 * (GRID_SPACING + BUTTON_SPACING_Y)));
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

  LCM_DrawFuncKeyGrid();
}

//--------------------------------------------------
void LCM_DisplayFuncKey(_LcmMenuType *menu)
{
  uint16_t i;

  g_LcmDisplay.Set_Text_Mode(0);

  g_LcmDisplay.Set_Text_Size(LCM_CHMSG_SIZE);
  g_LcmDisplay.Set_Text_colour(BLACK);
  g_LcmDisplay.Set_Text_Back_colour(BLUE);
  g_LcmDisplay.Print_String("P_ID:", GRID_SPACING + 2, LCM_CHMSG_Y + 2);
  g_LcmDisplay.Print_String("R_ID:", GRID_SPACING + 2, LCM_CHMSG_Y + 2 + (2 * LCM_ChMSG_HEIGHT));
  g_LcmDisplay.Print_String("P_ID:", GRID_SPACING + 2 + (LCM_WIDTH / 2), LCM_CHMSG_Y + 2);
  g_LcmDisplay.Print_String("R_ID:", GRID_SPACING + 2 + (LCM_WIDTH / 2), LCM_CHMSG_Y + 2 + (2 * LCM_ChMSG_HEIGHT));
  
  for(i = 0; i < sizeof(g_func_btn) / sizeof(_ButtonInfo); i++)
  {
    g_LcmDisplay.Set_Text_Size(g_func_btn[i].NameSize);
    g_LcmDisplay.Set_Text_colour(g_func_btn[i].NameColor);
    g_LcmDisplay.Set_Text_Back_colour(g_func_btn[i].BgColor);
    g_LcmDisplay.Print_String(menu[i].Name, g_func_btn[i].Px + 3, g_func_btn[i].Py - g_func_btn[i].NameSize * 8 / 2 + g_func_btn[i].NameSize / 2 + 1 + FUNCTION_SPACING_Y /  2);
  }
}

//--------------------------------------------------
void LCM_UpdateFuncKey(int num, uint8_t *str)
{
  g_LcmDisplay.Set_Text_Mode(0);

  g_LcmDisplay.Set_Text_Size(g_func_btn[num].NameSize);
  g_LcmDisplay.Set_Text_colour(g_func_btn[num].NameColor);
  g_LcmDisplay.Set_Text_Back_colour(g_func_btn[num].BgColor);
  g_LcmDisplay.Print_String(str, g_func_btn[num].Px + 3, g_func_btn[num].Py - g_func_btn[num].NameSize * 8 / 2 + g_func_btn[num].NameSize / 2 + 1 + FUNCTION_SPACING_Y /  2);
}
//--------------------------------------------------
void LCM_SetMsgText(void)
{
  g_LcmDisplay.Set_Text_Mode(0);
  g_LcmDisplay.Set_Text_Size(LCM_MSG_SIZE);
  g_LcmDisplay.Set_Text_colour(BLACK);
  g_LcmDisplay.Set_Text_Back_colour(BLUE);
}

//--------------------------------------------------
void LCM_ShowTitleString(uint8_t *str)
{
  uint8_t i;
  uint8_t  msg[27] = "                           ";

  for (i = 0; i < strlen(str); i++)
    msg[i] = str[i];

  LCM_SetMsgText();
  g_LcmDisplay.Print_String(msg, GRID_SPACING + 2, 1);
}

//--------------------------------------------------
void LCM_ShowParamString(uint8_t *str)
{
  uint8_t i;
  uint8_t  msg[11] = "           ";

  for (i = 0; i < strlen(str); i++)
    msg[i] = str[i];

  LCM_SetMsgText();
  g_LcmDisplay.Print_String(msg, GRID_SPACING + 2, LCM_MSG_HEIGHT + 2);
}

//--------------------------------------------------
void LCM_ShowValue(double value)
{
  LCM_SetMsgText();
  g_LcmDisplay.Print_Number_Float(value, 2, (GRID_SPACING + (4 * (GRID_SPACING + BUTTON_SPACING_X))), LCM_MSG_HEIGHT + 2, '.', 4, ' ');
}

//--------------------------------------------------
void LCM_ShowInfoString(uint8_t *str, uint8_t contd)
{
  static uint8_t msg_line = 0;
  uint8_t i;
  uint8_t  msg[27] = "                           ";

  for (i = 0; i < strlen(str); i++)
    msg[i] = str[i];

  LCM_SetMsgText();
  if (contd == 0)
  {
    msg_line = 0;
  }

  if (msg_line == 0)
    g_LcmDisplay.Fill_Rect(0, (1 * LCM_MSG_HEIGHT) + 2, LCM_WIDTH, (5 * LCM_MSG_HEIGHT), BLUE);

  g_LcmDisplay.Print_String(msg, GRID_SPACING + 2,  ((msg_line + 1) * LCM_MSG_HEIGHT) + 2);
  msg_line++;
  if (msg_line >= 5)
    msg_line = 0;
}

//--------------------------------------------------
void LCM_ScreenShowMsg(uint8_t *str, uint8_t contd)
{
  static uint8_t msg_line = 0;
  uint8_t i;
  uint8_t  msg[27] = "                           ";

  for (i = 0; i < strlen(str); i++)
    msg[i] = str[i];

  LCM_SetMsgText();
  if (contd == 0)
  {
    msg_line = 0;
  }

  if (msg_line == 0)
    g_LcmDisplay.Fill_Screen(BLUE);

  g_LcmDisplay.Print_String(msg, GRID_SPACING + 2, (msg_line * LCM_MSG_HEIGHT) + 2);
  msg_line++;
  if (msg_line >= 11)
    msg_line = 0;
}

//--------------------------------------------------
void LCM_ShowChannelID(int ch, int type, uint8_t *str)
{
  uint8_t i;
  uint8_t  msg[18] = "                  ";

  g_LcmDisplay.Set_Text_Mode(0);
  g_LcmDisplay.Set_Text_Size(LCM_CHMSG_SIZE);
  g_LcmDisplay.Set_Text_colour(BLACK);
  g_LcmDisplay.Set_Text_Back_colour(BLUE);
  for (i = 0; i < strlen(msg); i++)
    msg[i] = str[i];

  g_LcmDisplay.Print(msg, GRID_SPACING + 2 + (ch * (LCM_WIDTH / 2)), LCM_CHMSG_Y + 2 + (((type * 2) + 1) * LCM_ChMSG_HEIGHT));
}

//--------------------------------------------------
void LCM_ShowCycleCnt(int ch, uint8_t *str)
{
  uint8_t i;
  uint8_t  msg[18] = "                  ";

  g_LcmDisplay.Set_Text_Mode(0);
  g_LcmDisplay.Set_Text_Size(LCM_CHMSG_SIZE);
  g_LcmDisplay.Set_Text_colour(BLACK);
  g_LcmDisplay.Set_Text_Back_colour(BLUE);
  for (i = 0; i < strlen(msg); i++)
    msg[i] = str[i];

  g_LcmDisplay.Print(msg, GRID_SPACING + 2 + (ch * (LCM_WIDTH / 2)), LCM_CHMSG_Y + 2 + ((4 * LCM_ChMSG_HEIGHT)));
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
void LCM_TouchScan(uint8_t *type, uint8_t *num)
{  
  uint16_t i;
  uint16_t px = 0;
  uint16_t py = 0;

  
  *type = 0;
  *num = 255;
  g_LcmTouch.TP_Scan(0);
  if (g_LcmTouch.TP_Get_State() & TP_PRES_DOWN) 
  {
    px = g_LcmTouch.x;
    py = g_LcmTouch.y;

    for (i = 0; i < sizeof(g_func_btn) / sizeof(_ButtonInfo); i++)
    {
      if (LCM_IsPressed(px, py, g_func_btn[i].Px, g_func_btn[i].Py, (g_func_btn[i].Px + BUTTON_SPACING_X), (g_func_btn[i].Py + BUTTON_SPACING_Y)))
      {
        *type = BUTTON_FUNC;
        *num = i;
        break;
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

  //g_LcmDisplay.Fill_Screen(BLUE);
  //LCM_ScreenShowMsg("System Initial...", 0);
}

//--------------------------------------------------
