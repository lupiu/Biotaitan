//--------------------------------------------------
/*
* lcm.cpp
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "lcm.h"

//--------------------------------------------------
spi_device_handle_t g_lcm_handle;
TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, -1, TOUCH_RST, TOUCH_WIDTH, TOUCH_HEIGHT);

//--------------------------------------------------
void display_gray16_H(void)
{
    uint16_t y,i;
    uint8_t R8,G8,B8;
    uint32_t color=0;
    R8=0;
    G8=0;
    B8=0;
    for(y=0;y<16;y++)
    {
        {
        for(i=0;i<30;i++)
                {
                LT768_Lib.LT768_DrawSquare_Fill(0,y*30+i,800,y*30+i+1,color);
                }
        }
        R8=R8+16;
        G8=G8+16;
        B8=B8+16;
        color=(R8<<16|G8<<8|B8);
        }
}

//--------------------------------------------------
//清空屏幕并在右上角显示"RST"
void Load_Drow_Dialog(void)
{
    LT768_Lib.LT768_DrawSquare_Fill(0,0,800,480,color16M_white);//清屏  
    LT768_Lib.LT768_Select_Internal_Font_Init(24,1,1,0,1);
    LT768_Lib.LT768_Print_Internal_Font_String(800-40,0,color16M_red,color16M_white,"RST");
    //LT768_DrawSquare_Fill(1024-40,0,1024,600,color16M_white);
    //LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域 
}

//--------------------------------------------------
void WaitKey(void)
{
    static u8_t key_rls = 1;

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
                return;
            }
        }
        else{
            key_rls = 1;
        }
    }
}

//--------------------------------------------------
void LCM_Task(void * pvParametersoid)
{
    while(1)
    {
        LT768_Lib.LT768_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,Red);
        delay(500);
        WaitKey();
        LT768_Lib.LT768_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,Green);
        delay(500);
        WaitKey();
        LT768_Lib.LT768_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,Blue);
        delay(500);
        WaitKey();
        LT768_Lib.LT768_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,Grey);
        delay(500);
        WaitKey();
        display_gray16_H();
        delay(500);
        WaitKey();
        LT768_Lib.LT768_DrawSquare(0,0,LCD_XSIZE_TFT-1,LCD_YSIZE_TFT-1,Magenta);
        delay(500);
        WaitKey();
        LT768_Lib.LT768_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,Black);
        delay(500);
        WaitKey();
        LT768_Lib.LT768_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,color16M_white);
        LT768_Lib.LT768_Select_Internal_Font_Init(32,1,1,0,1);
        LT768_Lib.LT768_Print_Internal_Font_String(30,90,color16M_red,color16M_white,"Waiken-Smart STM32F407 TEST BOARD");
        LT768_Lib.LT768_Print_Internal_Font_String(30,130,color16M_red,color16M_white,"LCD:5.0inch_800X480");
        LT768_Lib.LT768_Print_Internal_Font_String(30,170,color16M_red,color16M_white,"TOUCH IC:GT911");
        LT768_Lib.LT768_Print_Internal_Font_String(30,210,color16M_red,color16M_white,"FW:V10 @2023-6-29");
        delay(1500);
        delay(1500);
        WaitKey();
        Load_Drow_Dialog();
        delay(500);
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
void LT768_Initial(void)
{
    LT768_LCD.Parallel_Init();
    LT768_Lib.LT768_HW_Reset();
    LT768_Lib.System_Check_Temp();
    delay(100);
    while(LT768_LCD.LCD_StatusRead() & 0x02);
    LT768_Lib.LT768_initial();
    LT768_Lib.LT768_PWM1_Init(1,0,200,100,100);
    LT768_Lib.LT768_PWM0_Init(1,0,200,100,100);
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
    xTaskCreatePinnedToCore(LCM_Task, "LCM_Task", 4096, NULL, 0, NULL, tskNO_AFFINITY);
}

//--------------------------------------------------
