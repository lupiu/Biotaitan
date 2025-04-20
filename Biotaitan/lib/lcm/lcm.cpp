//--------------------------------------------------
/*
* lcm.cpp
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "lcm.h"

//--------------------------------------------------
//spi_device_handle_t g_lcm_handle;
TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, -1, TOUCH_RST, TOUCH_WIDTH, TOUCH_HEIGHT);
uint8_t msg_line = 0;
//--------------------------------------------------
_ButtonInfo g_func_btn[4] = 
{
    "F0", 2,  Black,  Blue2, ((0 * (BUTTON_X + BUTTON_SPACING_X)) + BUTTON_SPACING_X + (BUTTON_X / 2)), (TOUCH_HEIGHT - (1 * (BUTTON_Y / 2)) - BUTTON_SPACING_Y),
    "F1", 2,  Black,  Blue2, ((1 * (BUTTON_X + BUTTON_SPACING_X)) + BUTTON_SPACING_X + (BUTTON_X / 2)), (TOUCH_HEIGHT - (1 * (BUTTON_Y / 2)) - BUTTON_SPACING_Y),
    "F2", 2,  Black,  Blue2, ((2 * (BUTTON_X + BUTTON_SPACING_X)) + BUTTON_SPACING_X + (BUTTON_X / 2)), (TOUCH_HEIGHT - (1 * (BUTTON_Y / 2)) - BUTTON_SPACING_Y),
    "F3", 2,  Black,  Blue2, ((3 * (BUTTON_X + BUTTON_SPACING_X)) + BUTTON_SPACING_X + (BUTTON_X / 2)), (TOUCH_HEIGHT - (1 * (BUTTON_Y / 2)) - BUTTON_SPACING_Y),
};
//--------------------------------------------------
void LCM_Task(void * pvParametersoid)
{
    static u8_t cnt = 0;
    static u8_t key_rls = 1;

    LT768_Lib.LT768_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,color16M_white);
    while(1)
    {
        tp.read();
        if (tp.isTouched){
            if (key_rls == 1){
                for (int i=0; i<tp.touches; i++){
                    Serial.print("Touch ");Serial.print(i+1);Serial.print(": ");;
                    Serial.print("  x: ");Serial.print(tp.points[i].x);
                    Serial.print("  y: ");Serial.print(tp.points[i].y);
                    Serial.print("  size: ");Serial.println(tp.points[i].size);
                    Serial.println(' ');
                }
                key_rls = 0;
                switch (cnt)
                {
                case 0:
                    LT768_Lib.LT768_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,Red);
                    LT768_Lib.LT768_DrawLine_Width(0,0,400,120,Green,10);
                    break;
                case 1:
                    LT768_Lib.LT768_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,Green);
                    break;
                case 2:
                    LT768_Lib.LT768_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,Blue);
                    break;
                case 3:
                    LT768_Lib.LT768_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,color16M_white);
                    LT768_Lib.LT768_Select_Internal_Font_Init(32,1,1,0,1);
                    LT768_Lib.LT768_Print_Internal_Font_String(30,90,color16M_red,color16M_white,"Waiken-Smart STM32F407 TEST BOARD");
                    LT768_Lib.LT768_Print_Internal_Font_String(30,130,color16M_red,color16M_white,"LCD:5.0inch_800X480");
                    LT768_Lib.LT768_Print_Internal_Font_String(30,170,color16M_red,color16M_white,"TOUCH IC:GT911");
                    LT768_Lib.LT768_Print_Internal_Font_String(30,210,color16M_red,color16M_white,"FW:V10 @2023-6-29");
                    break;                
                default:
                    break;
                }
                cnt++;
                if (cnt >= 4){
                    cnt = 0;
                }
            }
        }
        else{
            key_rls = 1;
        }
        delay(200);

        /*
        LT768_Lib.LT768_DrawSquare_Fill(0,0,800,480,Red);
        delay(500);
        LT768_Lib.LT768_DrawSquare_Fill(0,0,800,480,Green);
        delay(500);
        LT768_Lib.LT768_DrawSquare_Fill(0,0,800,480,Blue);
        delay(500);
        */
        //vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

//--------------------------------------------------
void LCM_DisplayFuncKey(_LcmMenuType *menu)
{
    uint16_t i;

    //for(i = 0; i < sizeof(g_func_btn) / sizeof(_ButtonInfo); i++)
    for(i = 0; i < 1; i++)
    {
        LT768_Lib.LT768_DrawCircleSquare_Fill(g_func_btn[i].Px - (BUTTON_X / 2), g_func_btn[i].Py - (BUTTON_Y / 2), g_func_btn[i].Px + (BUTTON_X / 2), g_func_btn[i].Py + (BUTTON_Y / 2), 20 , 20, Blue2);
        LT768_Lib.LT768_Select_Internal_Font_Init(32,1,1,0,1);
        LT768_Lib.LT768_Print_Internal_Font_String(g_func_btn[i].Px - (strlen(menu[i].Name) * LCM_MSG_SIZE / 2 / 2), g_func_btn[i].Py - (LCM_MSG_SIZE / 2), Black, Blue2, menu[i].Name);
    }
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
void LCM_TouchScan(uint8_t *num)
{
    uint16_t i;
    uint16_t px = 0;
    uint16_t py = 0;
    static u8_t key_rls = 1;

    num[0] = 255;
    tp.read();
    if (tp.isTouched)
    {
        if (key_rls == 1)
        {
            key_rls = 0;
            px = tp.points[0].x;
            py = tp.points[0].y;

            for (i = 0; i < sizeof(g_func_btn) / sizeof(_ButtonInfo); i++)
            {
                if (LCM_IsPressed(px, py, (g_func_btn[i].Px - (BUTTON_X / 2)), (g_func_btn[i].Py - (BUTTON_Y / 2)), (g_func_btn[i].Px + (BUTTON_X / 2)), (g_func_btn[i].Py + (BUTTON_X / 2))))
                {
                    num[0] = i;
                    break;
                }
            }
        }
        delay(20);
    }
    else
    {
        key_rls = 1;
    }
}

//--------------------------------------------------
void LCM_ShowInfoString(char *str, uint8_t contd)
{
    static uint8_t msg_line_num = 9; //9
    //static uint8_t msg_line = 0;
    uint8_t i;

    if (contd == 0)
    {
        msg_line = 0;
    }

    if (msg_line == 0)
        LT768_Lib.LT768_DrawSquare_Fill(0, LCM_MSG_START_Y, TOUCH_WIDTH, (LCM_MSG_START_Y + ((msg_line_num + 1) * LCM_MSG_SIZE+28)), White);

    LT768_Lib.LT768_Print_Internal_Font_String(LCM_MSG_START_X, (LCM_MSG_START_Y + (msg_line * LCM_MSG_SIZE+28)) , Black, White, str);
    msg_line++;
    if (msg_line >= msg_line_num)
        msg_line = 0;
}

//--------------------------------------------------
void LCM_ShowInfoString_red(char *str, uint8_t contd)
{
    static uint8_t msg_line_num = 6; //9
    //static uint8_t msg_line = 0;
    uint8_t i;

    if (contd == 0)
    {
        msg_line = 0;
    }

    if (msg_line == 0)
        LT768_Lib.LT768_DrawSquare_Fill(0, LCM_MSG_START_Y, TOUCH_WIDTH, (LCM_MSG_START_Y + ((msg_line_num + 1) * (LCM_MSG_SIZE+15))), White);

    LT768_Lib.LT768_Select_Internal_Font_Init(24,2,2,0,1);
    LT768_Lib.LT768_Print_Internal_Font_String(LCM_MSG_START_X, (LCM_MSG_START_Y + (msg_line * (LCM_MSG_SIZE+15))) , Red, White, str);
    msg_line++;
    if (msg_line >= msg_line_num)
        msg_line = 0;
}

//--------------------------------------------------
void LT768_Initial(void)
{
    LT768_LCD.Parallel_Init();
    LT768_Lib.LT768_HW_Reset();
    LT768_Lib.System_Check_Temp();
    delay(100);
    while(LT768_LCD.LCD_StatusRead() & 0x02);
    LT768_Lib.LT768_initial();
    LT768_Lib.LT768_PWM1_Init(1,3,200,500,30);
    LT768_Lib.LT768_PWM0_Init(1,3,200,500,30);
    LT768.Display_ON();
    LT768.Select_Main_Window_16bpp();
    LT768.Main_Image_Start_Address(0);
    LT768.Main_Image_Width(800);
    LT768.Main_Window_Start_XY(0,0);
    LT768.Canvas_Image_Start_address(0);
    LT768.Canvas_image_width(800);
    LT768.Active_Window_XY(0,0);
    LT768.Active_Window_WH(800,480);
}

//--------------------------------------------------
void LCM_Initial(void)
{
    LT768_Initial();
    tp.begin();
    tp.setRotation(ROTATION_INVERTED);

    //xTaskCreatePinnedToCore((TaskFunction_t)LCM_Task, "LCM_Task", 4096, NULL, 0, NULL, tskNO_AFFINITY);
}

//--------------------------------------------------
