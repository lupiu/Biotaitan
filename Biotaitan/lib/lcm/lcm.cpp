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

//--------------------------------------------------
void LCM_Task(void * pvParametersoid)
{
    static u8_t cnt = 0;
    static u8_t key_rls = 1;

    for (;;) {
        
        if (xSemaphoreTake(g_SPI_Semaphore, portMAX_DELAY) == pdTRUE)
        {
            LT768_LCD.Parallel_Init();
            Serial.println("LCM_Task");
            xSemaphoreGive(g_SPI_Semaphore);
        }
        delay(2000);
    }
/*
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
*/
        /*
        LT768_Lib.LT768_DrawSquare_Fill(0,0,800,480,Red);
        delay(500);
        LT768_Lib.LT768_DrawSquare_Fill(0,0,800,480,Green);
        delay(500);
        LT768_Lib.LT768_DrawSquare_Fill(0,0,800,480,Blue);
        delay(500);
        */
        //vTaskDelay(500 / portTICK_PERIOD_MS);
    //}
}

//--------------------------------------------------
void LT768_Initial(void)
{
    //LT768_LCD.Parallel_Init();
    //LT768_Lib.LT768_HW_Reset();
    //LT768_Lib.System_Check_Temp();
    //delay(100);
    //while(LT768_LCD.LCD_StatusRead() & 0x02);
    //LT768_Lib.LT768_initial();
    //LT768_Lib.LT768_PWM1_Init(1,0,200,100,100);
    //LT768_Lib.LT768_PWM0_Init(1,0,200,100,100);
    //LT768.Display_ON();
    //LT768.Select_Main_Window_16bpp();
    //LT768.Main_Image_Start_Address(0);
    //LT768.Main_Image_Width(800);
    //LT768.Main_Window_Start_XY(0,0);
    //LT768.Canvas_Image_Start_address(0);
    //LT768.Canvas_image_width(800);
    //LT768.Active_Window_XY(0,0);
    //LT768.Active_Window_WH(800,480);
}

//--------------------------------------------------
void LCM_Initial(void)
{
    LT768_Initial();
    tp.begin();
    tp.setRotation(ROTATION_INVERTED);

    xTaskCreatePinnedToCore((TaskFunction_t)LCM_Task, "LCM_Task", 4096, NULL, 0, NULL, tskNO_AFFINITY);
}

//--------------------------------------------------
