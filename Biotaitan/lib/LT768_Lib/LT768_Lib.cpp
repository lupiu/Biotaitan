#include "Arduino.h"
#include "LT768_LCD.h"
#include "LT768.h"
#include "LT768_Lib.h"
#include <SPI.h>
#include <Wire.h>

unsigned char LT768_Lib_CCLK = 100;    // LT768 core clock frequency    
unsigned char LT768_Lib_MCLK = 100;    // SDRAM clock frequency
unsigned char LT768_Lib_SCLK = 60 ;    // LCD scan clock frequency

void LT768Lib::Set_PCLK(unsigned char val)
{
  if(val == 1)  LT768.PCLK_Falling();
  else      LT768.PCLK_Rising();
}

void LT768Lib::Set_HSYNC_Active(unsigned char val)
{
  if(val == 1)  LT768.HSYNC_High_Active();
  else      LT768.HSYNC_Low_Active();
}

void LT768Lib::Set_VSYNC_Active(unsigned char val)
{
  if(val == 1)  LT768.VSYNC_High_Active();
  else      LT768.VSYNC_Low_Active();
}

void LT768Lib::Set_DE_Active(unsigned char val)
{
  if(val == 1)  LT768.DE_High_Active();
  else      LT768.DE_Low_Active();
}

//Reset LT768
void LT768Lib::LT768_HW_Reset(void)
{
	pinMode(LCD_RSTN, OUTPUT);
  digitalWrite(LCD_RSTN, LOW);
  delay(500);
  digitalWrite(LCD_RSTN, HIGH);
  delay(500);
}
void LT768Lib::System_Check_Temp(void)
{
  unsigned char i=0,j=0;
  unsigned char temp=0;
  unsigned char system_ok=0;
  do
  {
    j = LT768_LCD.LCD_StatusRead();
    if((j&0x02)==0x00)    
    {
      delay(2);                  //MCU too fast, necessary
      LT768_LCD.LCD_CmdWrite(0x01);
      delay(2);                  //MCU too fast, necessary
      temp = LT768_LCD.LCD_DataRead();
      if((temp & 0x80) == 0x80)       //Check CCR register's PLL is ready or not
      {
        system_ok=1;
        i=0;
      }
      else
      {
        delay(2); //MCU too fast, necessary
        LT768_LCD.LCD_CmdWrite(0x01);
        delay(2); //MCU too fast, necessary
        LT768_LCD.LCD_DataWrite(0x80);
      }
    }
    else
    {
      system_ok=0;
      i++;
    }
    if(system_ok==0 && i==5)
    {
      LT768_Lib.LT768_HW_Reset(); //note1
      i=0;
    }
  }while(system_ok==0);
}

void LT768Lib::LT768_PLL_Initial(void) 
{
  unsigned short lpllOD_sclk, lpllOD_cclk, lpllOD_mclk;
  unsigned short lpllR_sclk, lpllR_cclk, lpllR_mclk;
  unsigned short lpllN_sclk, lpllN_cclk, lpllN_mclk;
  
	int temp = (LCD_HBPD + LCD_HFPD + LCD_HSPW + LCD_XSIZE_TFT) * (LCD_VBPD + LCD_VFPD + LCD_VSPW+LCD_YSIZE_TFT) * 60;   
	
	int temp1 = (temp % 1000000) / 100000;
	if(temp1 > 5)
		 temp = temp / 1000000 + 1;
	else temp = temp / 1000000;
	
	LT768_Lib_SCLK = temp;
	temp = temp * 3;
	LT768_Lib_MCLK = temp;
	LT768_Lib_CCLK = temp;
	
	if(LT768_Lib_CCLK > 100)	LT768_Lib_CCLK = 100;
	if(LT768_Lib_MCLK > 100)	LT768_Lib_MCLK = 100;
	if(LT768_Lib_SCLK > 65)	LT768_Lib_SCLK = 65;
  //Fout = Fin*(N/R)/OD
  //Fout = 10*N/(2*5) = N
    lpllOD_sclk = 2;
    lpllOD_cclk = 2;
    lpllOD_mclk = 2;
    lpllR_sclk  = 5;
    lpllR_cclk  = 5;
    lpllR_mclk  = 5;
    lpllN_sclk  = LT768_Lib_SCLK;   // TFT PCLK out put frequency:60
    lpllN_cclk  = LT768_Lib_CCLK;    // Core CLK:100
    lpllN_mclk  = LT768_Lib_MCLK;    // SRAM CLK:100
      
  LT768_LCD.LCD_CmdWrite(0x05);
  LT768_LCD.LCD_DataWrite((lpllOD_sclk<<6) | (lpllR_sclk<<1) | ((lpllN_sclk>>8)&0x1));
  LT768_LCD.LCD_CmdWrite(0x07);
  LT768_LCD.LCD_DataWrite((lpllOD_mclk<<6) | (lpllR_mclk<<1) | ((lpllN_mclk>>8)&0x1));
  LT768_LCD.LCD_CmdWrite(0x09);
  LT768_LCD.LCD_DataWrite((lpllOD_cclk<<6) | (lpllR_cclk<<1) | ((lpllN_cclk>>8)&0x1));

  LT768_LCD.LCD_CmdWrite(0x06);
  LT768_LCD.LCD_DataWrite(lpllN_sclk);
  LT768_LCD.LCD_CmdWrite(0x08);
  LT768_LCD.LCD_DataWrite(lpllN_mclk);
  LT768_LCD.LCD_CmdWrite(0x0a);
  LT768_LCD.LCD_DataWrite(lpllN_cclk);

    LT768_LCD.LCD_CmdWrite(0x00);
  delayMicroseconds(1);
  LT768_LCD.LCD_DataWrite(0x80);
  //LT768.Enable_PLL();

  delay(1);

    LT768_LCD.LCD_CmdWrite(0x85);//PWM0 out pwm
    LT768_LCD.LCD_DataWrite(0x03);
}


void LT768Lib::LT768_SDRAM_initail(unsigned char mclk)
{
  unsigned short sdram_itv;
  
  LT768_LCD.LCD_RegisterWrite(0xe0,0x29);      
  LT768_LCD.LCD_RegisterWrite(0xe1,0x03); //CAS:2=0x02�ACAS:3=0x03
  sdram_itv = (64000000 / 8192) / (1000/mclk) ;
  sdram_itv-=2;

  LT768_LCD.LCD_RegisterWrite(0xe2,sdram_itv);
  LT768_LCD.LCD_RegisterWrite(0xe3,sdram_itv >>8);
  LT768_LCD.LCD_RegisterWrite(0xe4,0x01);
  LT768.Check_SDRAM_Ready();
  delay(1);
}

void LT768Lib::Set_LCD_Panel(void)
{
//**[01h]**//
  //LT768.TFT_24bit();
    LT768.TFT_16bit();
  LT768.Host_Bus_16bit(); //Host bus 16bit
      
//**[02h]**//
  LT768.RGB_16b_16bpp();
      //LT768.RGB_16b_24bpp_mode1();
    //LT768.RGB_16b_24bpp_mode2();
  LT768.MemWrite_Left_Right_Top_Down(); 
  //LT768.MemWrite_Down_Top_Left_Right();
      
//**[03h]**//
  LT768.Graphic_Mode();
  LT768.Memory_Select_SDRAM();
   

//#ifdef TM097TDH01
    
    //LT768.PCLK_Falling();     //REG[12h]:falling edge   -----------------
  //LT768.PCLK_Rising();
  LT768.HSCAN_L_to_R();     //REG[12h]:from left to right
  LT768.VSCAN_T_to_B();       //REG[12h]:from top to bottom
  //LT768.VSCAN_B_to_T();   //from bottom to top
  LT768.PDATA_Set_RGB();        //REG[12h]:Select RGB output
	//LT768.PDATA_Set_RBG();  					// Select RBG output
	//LT768.PDATA_Set_GRB();   				// Select GRB output
	//LT768.PDATA_Set_GBR();   				// Select GBR output
	//LT768.PDATA_Set_BRG();    			// Select BRG output
	//LT768.PDATA_Set_BGR();   				// Select BGR output
	
  //LT768.HSYNC_Low_Active();     //REG[13h]:  --------------------------
  //LT768.HSYNC_High_Active();
  //LT768.VSYNC_Low_Active();     //REG[13h]:  --------------------------
  //LT768.VSYNC_High_Active();
  //LT768.DE_High_Active();       //REG[13h]:  --------------------------
  //LT768.DE_Low_Active();

  LT768_Lib.Set_PCLK(LCD_PCLK_Falling_Rising);   //LCD_PCLK_Falling_Rising
  LT768_Lib.Set_HSYNC_Active(LCD_HSYNC_Active_Polarity);
  LT768_Lib.Set_VSYNC_Active(LCD_VSYNC_Active_Polarity);
  LT768_Lib.Set_DE_Active(LCD_DE_Active_Polarity);
 
  LT768.LCD_HorizontalWidth_VerticalHeight(LCD_XSIZE_TFT ,LCD_YSIZE_TFT);
  LT768.LCD_Horizontal_Non_Display(LCD_HBPD);                          
  LT768.LCD_HSYNC_Start_Position(LCD_HFPD);                              
  LT768.LCD_HSYNC_Pulse_Width(LCD_HSPW);                              
  LT768.LCD_Vertical_Non_Display(LCD_VBPD);                               
  LT768.LCD_VSYNC_Start_Position(LCD_VFPD);                               
  LT768.LCD_VSYNC_Pulse_Width(LCD_VSPW);                              
      
  LT768.Select_Main_Window_16bpp();
  //LT768.Select_Main_Window_24bpp();

//  LT768.Main_Image_Start_Address(0);
//  LT768.Main_Image_Width(LCD_XSIZE_TFT);
//  LT768.Main_Window_Start_XY(0,0);
//  LT768.Canvas_Image_Start_address(0);
//  LT768.Canvas_image_width(LCD_XSIZE_TFT);//
//  LT768.Active_Window_XY(0,0);
//  LT768.Active_Window_WH(LCD_XSIZE_TFT,LCD_YSIZE_TFT);

  LT768.Memory_XY_Mode(); //Block mode (X-Y coordination addressing)
  LT768.Memory_16bpp_Mode();
  //LT768.Memory_24bpp_Mode();
  LT768.Select_Main_Window_16bpp();
  //LT768.Select_Main_Window_24bpp();
}
void LT768Lib::LT768_initial(void)
{

    LT768_Lib.LT768_PLL_Initial();
  
    LT768_Lib.LT768_SDRAM_initail(LT768_Lib_MCLK);

		LT768_Lib.Set_LCD_Panel();
}
void LT768Lib::LT768_Init(void)
{
  delay(100);
  LT768_Lib.LT768_HW_Reset();
  LT768_Lib.System_Check_Temp();
  delay(100);
  while(LT768_LCD.LCD_StatusRead()&0x02);
  LT768_Lib.LT768_initial();
}




//--------------------------------------------------------------------------------------------------------------------------------------------

void LT768Lib::MPU8_8bpp_Memory_Write
(
unsigned short x //x of coordinate 
,unsigned short y // y of coordinate 
,unsigned short w //width
,unsigned short h //height
,const unsigned char *data //8bit data
)
{                             
  unsigned short i,j;
  LT768.Graphic_Mode();
  LT768.Active_Window_XY(x,y);
  LT768.Active_Window_WH(w,h);          
  LT768.Goto_Pixel_XY(x,y);
  LT768_LCD.LCD_CmdWrite(0x04); 
for(i=0;i< h;i++)
{ 
  for(j=0;j< w;j++)
  {    
   LT768.Check_Mem_WR_FIFO_not_Full();
   LT768_LCD.LCD_DataWrite(*data);
   data++;
  }
}
  LT768.Check_Mem_WR_FIFO_Empty();
}  

      
void LT768Lib::MPU8_16bpp_Memory_Write
(
 unsigned short x // x of coordinate 
,unsigned short y // y of coordinate 
,unsigned short w // width
,unsigned short h // height
,const unsigned char *data // 8bit data
)
{
  unsigned short i,j;
  LT768.Graphic_Mode();
    LT768.Active_Window_XY(x,y);
  LT768.Active_Window_WH(w,h);          
  LT768.Goto_Pixel_XY(x,y);
  LT768_LCD.LCD_CmdWrite(0x04);
for(i=0;i< h;i++)
{ 
  for(j=0;j< w;j++)
  {
   LT768.Check_Mem_WR_FIFO_not_Full();
   LT768_LCD.LCD_DataWrite(*data);
   data++;
   LT768.Check_Mem_WR_FIFO_not_Full();
   LT768_LCD.LCD_DataWrite(*data);
   data++;
  }
}
  LT768.Check_Mem_WR_FIFO_Empty();
}    


void LT768Lib::MPU8_24bpp_Memory_Write 
(
unsigned short x // x of coordinate
,unsigned short y // y of coordinate 
,unsigned short w // width
,unsigned short h // height
,const unsigned char *data // 8bit data
)

{
  unsigned short i,j;
  LT768.Graphic_Mode();
    LT768.Active_Window_XY(x,y);
  LT768.Active_Window_WH(w,h);          
  LT768.Goto_Pixel_XY(x,y);
  LT768_LCD.LCD_CmdWrite(0x04);
for(i=0;i< h;i++)
{ 
  for(j=0;j< w;j++)
  {
   LT768.Check_Mem_WR_FIFO_not_Full();
   LT768_LCD.LCD_DataWrite(*data);
   data++;
   LT768.Check_Mem_WR_FIFO_not_Full();
   LT768_LCD.LCD_DataWrite(*data);
   data++;
   LT768.Check_Mem_WR_FIFO_not_Full();
   LT768_LCD.LCD_DataWrite(*data);
   data++;
  }
}
  LT768.Check_Mem_WR_FIFO_Empty();
}



void LT768Lib::MPU16_16bpp_Memory_Write 
(
 unsigned short x       //x of coordinate
,unsigned short y             //y of coordinate 
,unsigned short w             //width
,unsigned short h             //height
,const unsigned short *data   //16bit data
)     
{
  unsigned short i,j;
  LT768.Graphic_Mode();
    LT768.Active_Window_XY(x,y);
  LT768.Active_Window_WH(w,h);          
  LT768.Goto_Pixel_XY(x,y);
  LT768_LCD.LCD_CmdWrite(0x04);
for(i=0;i< h;i++)
{ 
  for(j=0;j< w;j++)
  {
   LT768.Check_Mem_WR_FIFO_not_Full();
   LT768_LCD.LCD_DataWrite(*data);
   data++;
  }
}
  LT768.Check_Mem_WR_FIFO_Empty();
}




void LT768Lib::MPU16_24bpp_Mode1_Memory_Write 
(
unsigned short x //x of coordinate
,unsigned short y //y of coordinate
,unsigned short w //width
,unsigned short h //height
,const unsigned short *data //16bit data
) 
{
  unsigned short i,j;
  LT768.Graphic_Mode();
    LT768.Active_Window_XY(x,y);
  LT768.Active_Window_WH(w,h);          
  LT768.Goto_Pixel_XY(x,y);
  LT768_LCD.LCD_CmdWrite(0x04);
for(i=0;i< h;i++)
{ 
  for(j=0;j< w/2;j++)
  {
   LT768_LCD.LCD_DataWrite(*data);
   LT768.Check_Mem_WR_FIFO_not_Full();
   data++;
   LT768_LCD.LCD_DataWrite(*data);
   LT768.Check_Mem_WR_FIFO_not_Full();
   data++;
   LT768_LCD.LCD_DataWrite(*data);
   LT768.Check_Mem_WR_FIFO_not_Full();
   data++;
  }
}
  LT768.Check_Mem_WR_FIFO_Empty();
}


void LT768Lib::MPU16_24bpp_Mode2_Memory_Write
(
unsigned short x //x of coordinate
,unsigned short y //y of coordinate
,unsigned short w //width
,unsigned short h //height
,const unsigned short *data //16bit data
) 
{
  unsigned short i,j;
  LT768.Graphic_Mode();
    LT768.Active_Window_XY(x,y);
  LT768.Active_Window_WH(w,h);          
  LT768.Goto_Pixel_XY(x,y);
  LT768_LCD.LCD_CmdWrite(0x04);
for(i=0;i< h;i++)
{ 
  for(j=0;j< w;j++)
  {
   LT768.Check_Mem_WR_FIFO_not_Full();
   LT768_LCD.LCD_DataWrite(*data);
   data++;
   LT768.Check_Mem_WR_FIFO_not_Full();
   LT768_LCD.LCD_DataWrite(*data);
   data++;
  }
}
  LT768.Check_Mem_WR_FIFO_Empty();
}

//------------------------------------- Line -----------------------------------------
void LT768Lib::LT768_DrawLine
(
 unsigned short X1        
,unsigned short Y1        
,unsigned short X2        
,unsigned short Y2        
,unsigned long  LineColor 
)
{
  LT768.Foreground_color_65k(LineColor);
  LT768.Line_Start_XY(X1,Y1);
  LT768.Line_End_XY(X2,Y2);
  LT768.Start_Line();
  LT768.Check_2D_Busy();
}

void LT768Lib::LT768_DrawLine_Width
(
 unsigned short X1        
,unsigned short Y1        
,unsigned short X2        
,unsigned short Y2        
,unsigned long  LineColor 
,unsigned short Width     
)
{
  unsigned short  i = 0;
  while(Width--)
  {
    LT768_Lib.LT768_DrawLine(X1,Y1+i,X2,Y2+i,LineColor);
    i++;
  }
}

//------------------------------------- Circle -----------------------------------------
void LT768Lib::LT768_DrawCircle
(
 unsigned short XCenter           
,unsigned short YCenter           
,unsigned short R                 
,unsigned long CircleColor        
)
{
  LT768.Foreground_color_65k(CircleColor);
  LT768.Circle_Center_XY(XCenter,YCenter);
  LT768.Circle_Radius_R(R);
  LT768.Start_Circle_or_Ellipse();
  LT768.Check_2D_Busy(); 
}

void LT768Lib::LT768_DrawCircle_Fill
(
 unsigned short XCenter           
,unsigned short YCenter           
,unsigned short R                 
,unsigned long ForegroundColor    
)
{
  LT768.Foreground_color_65k(ForegroundColor);
  LT768.Circle_Center_XY(XCenter,YCenter);
  LT768.Circle_Radius_R(R);
  LT768.Start_Circle_or_Ellipse_Fill();
  LT768.Check_2D_Busy(); 
}

void LT768Lib::LT768_DrawCircle_Width
(
 unsigned short XCenter          
,unsigned short YCenter          
,unsigned short R                
,unsigned long CircleColor       
,unsigned long ForegroundColor   
,unsigned short Width            
)
{
  LT768_Lib.LT768_DrawCircle_Fill(XCenter,YCenter,R+Width,CircleColor);
  LT768_Lib.LT768_DrawCircle_Fill(XCenter,YCenter,R,ForegroundColor);
}

//------------------------------------- Ellipse -----------------------------------------
void LT768Lib::LT768_DrawEllipse
(
 unsigned short XCenter          
,unsigned short YCenter          
,unsigned short X_R              
,unsigned short Y_R              
,unsigned long EllipseColor      
)
{
  LT768.Foreground_color_65k(EllipseColor);
  LT768.Ellipse_Center_XY(XCenter,YCenter);
  LT768.Ellipse_Radius_RxRy(X_R,Y_R);
  LT768.Start_Circle_or_Ellipse();
  LT768.Check_2D_Busy(); 
}

void LT768Lib::LT768_DrawEllipse_Fill
(
 unsigned short XCenter          
,unsigned short YCenter         
,unsigned short X_R              
,unsigned short Y_R             
,unsigned long ForegroundColor  
)
{
  LT768.Foreground_color_65k(ForegroundColor);
  LT768.Ellipse_Center_XY(XCenter,YCenter);
  LT768.Ellipse_Radius_RxRy(X_R,Y_R);
  LT768.Start_Circle_or_Ellipse_Fill();
  LT768.Check_2D_Busy(); 
}


void LT768Lib::LT768_DrawEllipse_Width
(
 unsigned short XCenter        
,unsigned short YCenter          
,unsigned short X_R          
,unsigned short Y_R          
,unsigned long EllipseColor  
,unsigned long ForegroundColor   
,unsigned short Width       
)
{
  LT768_Lib.LT768_DrawEllipse_Fill(XCenter,YCenter,X_R+Width,Y_R+Width,EllipseColor);
  LT768_Lib.LT768_DrawEllipse_Fill(XCenter,YCenter,X_R,Y_R,ForegroundColor);
}

//------------------------------------- Square -----------------------------------------
void LT768Lib::LT768_DrawSquare
(
 unsigned short X1                
,unsigned short Y1                
,unsigned short X2                
,unsigned short Y2             
,unsigned long SquareColor      
)
{
  LT768.Foreground_color_65k(SquareColor);
  LT768.Square_Start_XY(X1,Y1);
  LT768.Square_End_XY(X2,Y2);
  LT768.Start_Square();
  LT768.Check_2D_Busy(); 
}

void LT768Lib::LT768_DrawSquare_Fill
(
 unsigned short X1                
,unsigned short Y1              
,unsigned short X2                
,unsigned short Y2              
,unsigned long ForegroundColor   
)
{
  LT768.Foreground_color_65k(ForegroundColor);
  LT768.Square_Start_XY(X1,Y1);
  LT768.Square_End_XY(X2,Y2);
  LT768.Start_Square_Fill();
  LT768.Check_2D_Busy();
}

void LT768Lib::LT768_DrawSquare_Width
(
 unsigned short X1                
,unsigned short Y1                
,unsigned short X2               
,unsigned short Y2                
,unsigned long SquareColor       
,unsigned long ForegroundColor   
,unsigned short Width         
)
{
  LT768_Lib.LT768_DrawSquare_Fill(X1-Width,Y1-Width,X2+Width,Y2+Width,SquareColor);
  LT768_Lib.LT768_DrawSquare_Fill(X1,Y1,X2,Y2,ForegroundColor);
}

//------------------------------------- CircleSquare -----------------------------------------
void LT768Lib::LT768_DrawCircleSquare
(
 unsigned short X1                
,unsigned short Y1                
,unsigned short X2                
,unsigned short Y2                
,unsigned short X_R              
,unsigned short Y_R              
,unsigned long CircleSquareColor
)
{
  LT768.Foreground_color_65k(CircleSquareColor);
  LT768.Square_Start_XY(X1,Y1);
  LT768.Square_End_XY(X2,Y2); 
  LT768.Circle_Square_Radius_RxRy(X_R,Y_R);
  LT768.Start_Circle_Square();
  LT768.Check_2D_Busy();
}

void LT768Lib::LT768_DrawCircleSquare_Fill
(
 unsigned short X1              
,unsigned short Y1                
,unsigned short X2               
,unsigned short Y2                
,unsigned short X_R               
,unsigned short Y_R             
,unsigned long ForegroundColor  
)
{
  LT768.Foreground_color_65k(ForegroundColor);
  LT768.Square_Start_XY(X1,Y1);
  LT768.Square_End_XY(X2,Y2); 
  LT768.Circle_Square_Radius_RxRy(X_R,Y_R);
  LT768.Start_Circle_Square_Fill();
  LT768.Check_2D_Busy(); 
}

void LT768Lib::LT768_DrawCircleSquare_Width
(
 unsigned short X1              
,unsigned short Y1              
,unsigned short X2               
,unsigned short Y2               
,unsigned short X_R               
,unsigned short Y_R               
,unsigned long CircleSquareColor 
,unsigned long ForegroundColor   
,unsigned short Width           
)
{
  LT768_Lib.LT768_DrawCircleSquare_Fill(X1-Width,Y1-Width,X2+Width,Y2+Width,X_R,Y_R,CircleSquareColor);
  LT768_Lib.LT768_DrawCircleSquare_Fill(X1,Y1,X2,Y2,X_R,Y_R,ForegroundColor);
}


//------------------------------------- Triangle -----------------------------------------
void LT768Lib::LT768_DrawTriangle
(
 unsigned short X1             
,unsigned short Y1            
,unsigned short X2           
,unsigned short Y2              
,unsigned short X3              
,unsigned short Y3              
,unsigned long TriangleColor  
)
{
  LT768.Foreground_color_65k(TriangleColor);
  LT768.Triangle_Point1_XY(X1,Y1);
  LT768.Triangle_Point2_XY(X2,Y2);
  LT768.Triangle_Point3_XY(X3,Y3);
  LT768.Start_Triangle();
  LT768.Check_2D_Busy(); 
}



void LT768Lib::LT768_DrawTriangle_Fill
(
 unsigned short X1             
,unsigned short Y1             
,unsigned short X2            
,unsigned short Y2            
,unsigned short X3           
,unsigned short Y3             
,unsigned long ForegroundColor 
)
{
  LT768.Foreground_color_65k(ForegroundColor);
  LT768.Triangle_Point1_XY(X1,Y1);
  LT768.Triangle_Point2_XY(X2,Y2);
  LT768.Triangle_Point3_XY(X3,Y3);
  LT768.Start_Triangle_Fill();
  LT768.Check_2D_Busy();
}



//------------------------------------- 1/4 Curve -----------------------------------------
void LT768Lib::LT768_DrawLeftUpCurve
( 
 unsigned short XCenter          
,unsigned short YCenter          
,unsigned short X_R              
,unsigned short Y_R               
,unsigned long CurveColor        
)
{
  LT768.Foreground_color_65k(CurveColor);
  LT768.Ellipse_Center_XY(XCenter,YCenter);
  LT768.Ellipse_Radius_RxRy(X_R,Y_R);
  LT768.Start_Left_Up_Curve();
  LT768.Check_2D_Busy(); 
}


void LT768Lib::LT768_DrawLeftDownCurve
(
 unsigned short XCenter          
,unsigned short YCenter          
,unsigned short X_R              
,unsigned short Y_R             
,unsigned long CurveColor        
)
{
  LT768.Foreground_color_65k(CurveColor);
  LT768.Ellipse_Center_XY(XCenter,YCenter);
  LT768.Ellipse_Radius_RxRy(X_R,Y_R);
  LT768.Start_Left_Down_Curve();
  LT768.Check_2D_Busy(); 
}


void LT768Lib::LT768_DrawRightUpCurve
(
 unsigned short XCenter          
,unsigned short YCenter        
,unsigned short X_R            
,unsigned short Y_R              
,unsigned long CurveColor      
)
{
  LT768.Foreground_color_65k(CurveColor);
  LT768.Ellipse_Center_XY(XCenter,YCenter);
  LT768.Ellipse_Radius_RxRy(X_R,Y_R);
  LT768.Start_Right_Up_Curve();
  LT768.Check_2D_Busy(); 
}


void LT768Lib::LT768_DrawRightDownCurve
(
 unsigned short XCenter        
,unsigned short YCenter        
,unsigned short X_R            
,unsigned short Y_R              
,unsigned long CurveColor     
)
{
  LT768.Foreground_color_65k(CurveColor);
  LT768.Ellipse_Center_XY(XCenter,YCenter);
  LT768.Ellipse_Radius_RxRy(X_R,Y_R);
  LT768.Start_Right_Down_Curve();
  LT768.Check_2D_Busy(); 
}


void LT768Lib::LT768_SelectDrawCurve
(
 unsigned short XCenter       
,unsigned short YCenter          
,unsigned short X_R             
,unsigned short Y_R              
,unsigned long CurveColor        
,unsigned short  Dir             
)
{
  switch(Dir)
  {
    case 0:LT768_Lib.LT768_DrawLeftDownCurve(XCenter,YCenter,X_R,Y_R,CurveColor);   break;
    case 1:LT768_Lib.LT768_DrawLeftUpCurve(XCenter,YCenter,X_R,Y_R,CurveColor);     break;
    case 2:LT768_Lib.LT768_DrawRightUpCurve(XCenter,YCenter,X_R,Y_R,CurveColor);    break;
    case 3:LT768_Lib.LT768_DrawRightDownCurve(XCenter,YCenter,X_R,Y_R,CurveColor);  break;
    default:                                                              break;
  }
}


//------------------------------------- 1/4 Curve_Fill -----------------------------------------
void LT768Lib::LT768_DrawLeftUpCurve_Fill
(
 unsigned short XCenter           
,unsigned short YCenter           
,unsigned short X_R             
,unsigned short Y_R               
,unsigned long ForegroundColor   
)
{
  LT768.Foreground_color_65k(ForegroundColor);
  LT768.Ellipse_Center_XY(XCenter,YCenter);
  LT768.Ellipse_Radius_RxRy(X_R,Y_R);
  LT768.Start_Left_Up_Curve_Fill();
  LT768.Check_2D_Busy(); 
}


void LT768Lib::LT768_DrawLeftDownCurve_Fill
(
 unsigned short XCenter          
,unsigned short YCenter          
,unsigned short X_R              
,unsigned short Y_R          
,unsigned long ForegroundColor   
)
{
  LT768.Foreground_color_65k(ForegroundColor);
  LT768.Ellipse_Center_XY(XCenter,YCenter);
  LT768.Ellipse_Radius_RxRy(X_R,Y_R);
  LT768.Start_Left_Down_Curve_Fill();
  LT768.Check_2D_Busy(); 
}


void LT768Lib::LT768_DrawRightUpCurve_Fill
(
 unsigned short XCenter          
,unsigned short YCenter        
,unsigned short X_R            
,unsigned short Y_R            
,unsigned long ForegroundColor   
)
{
  LT768.Foreground_color_65k(ForegroundColor);
  LT768.Ellipse_Center_XY(XCenter,YCenter);
  LT768.Ellipse_Radius_RxRy(X_R,Y_R);
  LT768.Start_Right_Up_Curve_Fill();
  LT768.Check_2D_Busy(); 
}


void LT768Lib::LT768_DrawRightDownCurve_Fill
(
 unsigned short XCenter          
,unsigned short YCenter         
,unsigned short X_R               
,unsigned short Y_R             
,unsigned long ForegroundColor    
)
{
  LT768.Foreground_color_65k(ForegroundColor);
  LT768.Ellipse_Center_XY(XCenter,YCenter);
  LT768.Ellipse_Radius_RxRy(X_R,Y_R);
  LT768.Start_Right_Down_Curve_Fill();
  LT768.Check_2D_Busy(); 
}


void LT768Lib::LT768_SelectDrawCurve_Fill
(
 unsigned short XCenter           
,unsigned short YCenter           
,unsigned short X_R              
,unsigned short Y_R              
,unsigned long CurveColor         
,unsigned short  Dir            
)
{
  switch(Dir)
  {
    case 0:LT768_Lib.LT768_DrawLeftDownCurve_Fill(XCenter,YCenter,X_R,Y_R,CurveColor);    break;
    case 1:LT768_Lib.LT768_DrawLeftUpCurve_Fill(XCenter,YCenter,X_R,Y_R,CurveColor);      break;
    case 2:LT768_Lib.LT768_DrawRightUpCurve_Fill(XCenter,YCenter,X_R,Y_R,CurveColor);     break;
    case 3:LT768_Lib.LT768_DrawRightDownCurve_Fill(XCenter,YCenter,X_R,Y_R,CurveColor);   break;
    default:                                                                    break;
  }
}



//------------------------------------- Quadrilateral -----------------------------------------

void LT768Lib::LT768_DrawQuadrilateral
(
 unsigned short X1              
,unsigned short Y1             
,unsigned short X2            
,unsigned short Y2            
,unsigned short X3              
,unsigned short Y3            
,unsigned short X4          
,unsigned short Y4            
,unsigned long ForegroundColor  
)
{
  LT768.Foreground_color_65k(ForegroundColor);
  LT768.Triangle_Point1_XY(X1,Y1);
  LT768.Triangle_Point2_XY(X2,Y2);
  LT768.Triangle_Point3_XY(X3,Y3);
  LT768.Ellipse_Radius_RxRy(X4,Y4);
  
  LT768_LCD.LCD_CmdWrite(0x67);
  LT768_LCD.LCD_DataWrite(0x8d);
  LT768.Check_Busy_Draw();
  
  LT768.Check_2D_Busy(); 
}


void LT768Lib::LT768_DrawQuadrilateral_Fill
(
 unsigned short X1           
,unsigned short Y1            
,unsigned short X2            
,unsigned short Y2          
,unsigned short X3            
,unsigned short Y3             
,unsigned short X4              
,unsigned short Y4             
,unsigned long ForegroundColor  
)
{
  LT768.Foreground_color_65k(ForegroundColor);
  LT768.Triangle_Point1_XY(X1,Y1);
  LT768.Triangle_Point2_XY(X2,Y2);
  LT768.Triangle_Point3_XY(X3,Y3);
  LT768.Ellipse_Radius_RxRy(X4,Y4);
  
  LT768_LCD.LCD_CmdWrite(0x67);
  LT768_LCD.LCD_DataWrite(0xa7);
  LT768.Check_Busy_Draw();
  
  LT768.Check_2D_Busy(); 
}



//------------------------------------- Pentagon -----------------------------------------

void LT768Lib::LT768_DrawPentagon
(
 unsigned short X1              
,unsigned short Y1             
,unsigned short X2              
,unsigned short Y2              
,unsigned short X3              
,unsigned short Y3              
,unsigned short X4              
,unsigned short Y4             
,unsigned short X5              
,unsigned short Y5              
,unsigned long ForegroundColor 
)
{
  LT768.Foreground_color_65k(ForegroundColor);
  LT768.Triangle_Point1_XY(X1,Y1);
  LT768.Triangle_Point2_XY(X2,Y2);
  LT768.Triangle_Point3_XY(X3,Y3);
  LT768.Ellipse_Radius_RxRy(X4,Y4);
  LT768.Ellipse_Center_XY(X5,Y5);
  
  LT768_LCD.LCD_CmdWrite(0x67);
  LT768_LCD.LCD_DataWrite(0x8F);
  LT768.Check_Busy_Draw();
  
  LT768.Check_2D_Busy(); 
}


void LT768Lib::LT768_DrawPentagon_Fill
(
 unsigned short X1             
,unsigned short Y1             
,unsigned short X2            
,unsigned short Y2            
,unsigned short X3              
,unsigned short Y3           
,unsigned short X4           
,unsigned short Y4              
,unsigned short X5            
,unsigned short Y5          
,unsigned long ForegroundColor 
)
{
  LT768.Foreground_color_65k(ForegroundColor);
  LT768.Triangle_Point1_XY(X1,Y1);
  LT768.Triangle_Point2_XY(X2,Y2);
  LT768.Triangle_Point3_XY(X3,Y3);
  LT768.Ellipse_Radius_RxRy(X4,Y4);
  LT768.Ellipse_Center_XY(X5,Y5);
  
  LT768_LCD.LCD_CmdWrite(0x67);
  LT768_LCD.LCD_DataWrite(0xa9);
  LT768.Check_Busy_Draw();
  
  LT768.Check_2D_Busy(); 
}


//------------------------------------- Cylinder -----------------------------------------
unsigned char LT768Lib::LT768_DrawCylinder
(
 unsigned short XCenter           
,unsigned short YCenter         
,unsigned short X_R              
,unsigned short Y_R              
,unsigned short H                
,unsigned long CylinderColor     
,unsigned long ForegroundColor   
)
{
  if(YCenter < H) return 1;
  
  LT768_Lib.LT768_DrawEllipse_Fill(XCenter,YCenter,X_R,Y_R,ForegroundColor);
  LT768_Lib.LT768_DrawEllipse(XCenter,YCenter,X_R,Y_R,CylinderColor);

  LT768_Lib.LT768_DrawSquare_Fill(XCenter-X_R,YCenter-H,XCenter+X_R,YCenter,ForegroundColor);

  LT768_Lib.LT768_DrawEllipse_Fill(XCenter,YCenter-H,X_R,Y_R,ForegroundColor);
  LT768_Lib.LT768_DrawEllipse(XCenter,YCenter-H,X_R,Y_R,CylinderColor);
  
  LT768_Lib.LT768_DrawLine(XCenter-X_R,YCenter,XCenter-X_R,YCenter-H,CylinderColor);
  LT768_Lib.LT768_DrawLine(XCenter+X_R,YCenter,XCenter+X_R,YCenter-H,CylinderColor);
  
  return 0;
}


//------------------------------------- Quadrangular -----------------------------------------
void LT768Lib::LT768_DrawQuadrangular
(
 unsigned short X1
,unsigned short Y1
,unsigned short X2
,unsigned short Y2
,unsigned short X3
,unsigned short Y3
,unsigned short X4
,unsigned short Y4
,unsigned short X5
,unsigned short Y5
,unsigned short X6
,unsigned short Y6
,unsigned long QuadrangularColor   
,unsigned long ForegroundColor    
)
{
  LT768_Lib.LT768_DrawSquare_Fill(X1,Y1,X5,Y5,ForegroundColor);
  LT768_Lib.LT768_DrawSquare(X1,Y1,X5,Y5,QuadrangularColor);
  
  LT768_Lib.LT768_DrawQuadrilateral_Fill(X1,Y1,X2,Y2,X3,Y3,X4,Y4,ForegroundColor);
  LT768_Lib.LT768_DrawQuadrilateral(X1,Y1,X2,Y2,X3,Y3,X4,Y4,QuadrangularColor);
  
  LT768_Lib.LT768_DrawQuadrilateral_Fill(X3,Y3,X4,Y4,X5,Y5,X6,Y6,ForegroundColor);
  LT768_Lib.LT768_DrawQuadrilateral(X3,Y3,X4,Y4,X5,Y5,X6,Y6,QuadrangularColor);
}

//-------------------------------------------- Table -------------------------------------------------------------------
void LT768Lib::LT768_MakeTable
(
  unsigned short X1,                  
  unsigned short Y1,                 
  unsigned short W,                   
  unsigned short H,                   
  unsigned short Line,             
  unsigned short Row,                
  unsigned long  TableColor,        
  unsigned long  ItemColor,           
  unsigned long  ForegroundColor,   
  unsigned short width1,             
  unsigned short width2,             
  unsigned char  mode             
)
{
  unsigned short i = 0;
  unsigned short x2,y2;
  x2 = X1 + W * Row;
  y2 = Y1 + H * Line;
  
  LT768_Lib.LT768_DrawSquare_Width(X1,Y1,x2,y2,TableColor,ForegroundColor,width2);  
  
  if(mode == 0)       LT768_Lib.LT768_DrawSquare_Fill(X1,Y1,X1+W,y2,ItemColor);  
  else if(mode == 1)  LT768_Lib.LT768_DrawSquare_Fill(X1,Y1,x2,Y1+H,ItemColor); 
  
  for(i = 0 ; i < Line ; i++)
  {
    LT768_Lib.LT768_DrawLine_Width(X1,Y1+i*H,x2,Y1+i*H,TableColor,width1);
  }
  
  for(i = 0 ; i < Row ; i++)
  {
    LT768_Lib.LT768_DrawLine_Width(X1+i*W,Y1,X1+i*W,y2,TableColor,width1);
  }
}




//--------------------------------------------------------------------------------------------------------------------------------------------

void LT768Lib::LT768_Color_Bar_ON(void)
{
  LT768.Color_Bar_ON();
}

void LT768Lib::LT768_Color_Bar_OFF(void)
{
  LT768.Color_Bar_OFF();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void LT768Lib::LT768_DMA_24bit_Linear
(
 unsigned char SCS              // Select SPI : SCS��0       SCS��1
,unsigned char Clk              // SPI Clock = System Clock /{(Clk+1)*2}
,unsigned long flash_addr       // Data flash address  
,unsigned long memory_addr      // Data transfers to SDRAM address
,unsigned long data_num         // Data size
)
{
  
  LT768.Enable_SFlash_SPI();                               
  if(SCS == 0)    LT768.Select_SFI_0();                    // Select SPI0
  if(SCS == 1)    LT768.Select_SFI_1();                    // Select SPI1
  
  LT768.Memory_Linear_Mode();
  LT768.Select_SFI_DMA_Mode();                             // Select SPI DMA mode
  
  LT768.SPI_Clock_Period(Clk);                             // SPI speed
  LT768.SFI_DMA_Destination_Start_Address(memory_addr);    
  LT768.SFI_DMA_Transfer_Number(data_num);                 
  LT768.SFI_DMA_Source_Start_Address(flash_addr);          
  LT768.Check_Busy_SFI_DMA(); 
  LT768.Start_SFI_DMA();
  LT768.Check_Busy_SFI_DMA();
  LT768.Memory_XY_Mode();
}

void LT768Lib::LT768_DMA_32bit_Linear
(
 unsigned char SCS              // Select SPI : SCS��0       SCS��1
,unsigned char Clk              // SPI Clock = System Clock /{(Clk+1)*2}
,unsigned long flash_addr       // Data flash address   
,unsigned long memory_addr      // Data transfers to SDRAM address
,unsigned long data_num         // Data size
)
{
  LT768.Enable_SFlash_SPI();                               
  if(SCS == 0)    LT768.Select_SFI_0();                    // Select SPI0
  if(SCS == 1)    LT768.Select_SFI_1();                    // Select SPI1
  
  LT768.Memory_Linear_Mode();
  LT768.Select_SFI_DMA_Mode();                             // Select SPI DMA mode
  LT768.Select_SFI_32bit_Address();
  
  LT768.SPI_Clock_Period(Clk);                             // SPI speed
  LT768.SFI_DMA_Destination_Start_Address(memory_addr);    
  LT768.SFI_DMA_Transfer_Number(data_num);                
  LT768.SFI_DMA_Source_Start_Address(flash_addr);          
  LT768.Check_Busy_SFI_DMA(); 
  LT768.Start_SFI_DMA();
  LT768.Check_Busy_SFI_DMA();
  LT768.Memory_XY_Mode();
}



void LT768Lib::LT768_DMA_24bit_Block
(
 unsigned char SCS         // Select SPI : SCS��0       SCS��1
,unsigned char Clk         // SPI Clock = System Clock /{(Clk+1)*2}
,unsigned short X1         // Transfer to SDRAM address:X1
,unsigned short Y1         // Transfer to SDRAM address:Y1
,unsigned short X_W        // DMA data width
,unsigned short Y_H        // DMA data height
,unsigned short P_W        // Picture's width
,unsigned long Addr        // Flash address
)
{
  LT768.Enable_SFlash_SPI();                            
  if(SCS == 0)  LT768.Select_SFI_0();                   // Select SPI0
  if(SCS == 1)  LT768.Select_SFI_1();                   // Select SPI1
	
  LT768.Memory_XY_Mode();                     
  LT768.Select_SFI_DMA_Mode();                          // Select SPI DMA mode
  LT768.SPI_Clock_Period(Clk);                          // Select SPI clock

  LT768.Goto_Pixel_XY(X1,Y1);                           // Setting the location of memory in the graphic mode
  LT768.SFI_DMA_Destination_Upper_Left_Corner(X1,Y1);   // DMA destination(SDRAM address)
  LT768.SFI_DMA_Transfer_Width_Height(X_W,Y_H);         // Setting Block data: width&height
  LT768.SFI_DMA_Source_Width(P_W);                      // Setting the width of the source data
  LT768.SFI_DMA_Source_Start_Address(Addr);             // Setting the FLASH address of the source data

  LT768.Start_SFI_DMA();                                
  LT768.Check_Busy_SFI_DMA();                        
}



void LT768Lib::LT768_DMA_32bit_Block
(
 unsigned char SCS         // Select SPI : SCS��0       SCS��1
,unsigned char Clk         // SPI Clock = System Clock /{(Clk+1)*2}
,unsigned short X1         // Transfer to SDRAM address:X1
,unsigned short Y1         // Transfer to SDRAM address:Y1
,unsigned short X_W        // DMA data width
,unsigned short Y_H        // DMA data height
,unsigned short P_W        // Picture's width
,unsigned long Addr        // Flash address
)
{
  LT768.Enable_SFlash_SPI();                  
  if(SCS == 0)  LT768.Select_SFI_0();                          
  if(SCS == 1)  LT768.Select_SFI_1();                         
   
  LT768.Select_SFI_DMA_Mode();                  
  LT768.SPI_Clock_Period(Clk);

  LT768.Select_SFI_32bit_Address();               

  LT768.Goto_Pixel_XY(X1,Y1);                   
  LT768.SFI_DMA_Destination_Upper_Left_Corner(X1,Y1);     
  LT768.SFI_DMA_Transfer_Width_Height(X_W,Y_H);         
  LT768.SFI_DMA_Source_Width(P_W);                
  LT768.SFI_DMA_Source_Start_Address(Addr);              

  LT768.Start_SFI_DMA();                    
  LT768.Check_Busy_SFI_DMA();                
  LT768.Select_SFI_24bit_Address();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void LT768Lib::LT768_Select_Internal_Font_Init
(
 unsigned char Size         // Setting font size  16��16*16     24:24*24    32:32*32
,unsigned char XxN          // Width magnification:1~4
,unsigned char YxN          // Heigtht magnification:1~4
,unsigned char ChromaKey    // 0��not transparent   1��transparent
,unsigned char Alignment    // 0��font not align      1��font align
)
{
  if(Size==16)  LT768.Font_Select_8x16_16x16();
  if(Size==24)  LT768.Font_Select_12x24_24x24();
  if(Size==32)  LT768.Font_Select_16x32_32x32();

  //(*)
  if(XxN==1)  LT768.Font_Width_X1();
  if(XxN==2)  LT768.Font_Width_X2();
  if(XxN==3)  LT768.Font_Width_X3();
  if(XxN==4)  LT768.Font_Width_X4();

  //(*) 
  if(YxN==1)  LT768.Font_Height_X1();
  if(YxN==2)  LT768.Font_Height_X2();
  if(YxN==3)  LT768.Font_Height_X3();
  if(YxN==4)  LT768.Font_Height_X4();

  //(*)
  if(ChromaKey==0)  LT768.Font_Background_select_Color(); 
  if(ChromaKey==1)  LT768.Font_Background_select_Transparency();  

  //(*)
  if(Alignment==0)  LT768.Disable_Font_Alignment();
  if(Alignment==1)  LT768.Enable_Font_Alignment();
}

void LT768Lib::LT768_Print_Internal_Font_String
(
 unsigned short x               // Font X coordinates
,unsigned short y               // Font Y coordinates
,unsigned long FontColor        
,unsigned long BackGroundColor  
,char *c                        // The first address of the data buffer
)
{
  LT768.Text_Mode();
  LT768.CGROM_Select_Internal_CGROM();
  LT768.Foreground_color_65k(FontColor);
  LT768.Background_color_65k(BackGroundColor);
  LT768.Goto_Text_XY(x,y);
  LT768.Show_String(c);
}



void LT768Lib::LT768_Select_Outside_Font_Init
(
 unsigned char SCS           // Select SPI : SCS��0       SCS��1
,unsigned char Clk           // SPI Clock = System Clock /{(Clk+1)*2}
,unsigned long FlashAddr     
,unsigned long MemoryAddr    
,unsigned long Num           // Font data Num
,unsigned char Size          // Font size  16��16*16     24:24*24    32:32*32
,unsigned char XxN          // Width magnification:1~4
,unsigned char YxN          // Heigtht magnification:1~4
,unsigned char ChromaKey    // 0��not transparent   1��transparent
,unsigned char Alignment    // 0��font not align      1��font align
)
{
  if(Size==16)  LT768.Font_Select_8x16_16x16();
  if(Size==24)  LT768.Font_Select_12x24_24x24();
  if(Size==32)  LT768.Font_Select_16x32_32x32();

  //(*)
  if(XxN==1)  LT768.Font_Width_X1();
  if(XxN==2)  LT768.Font_Width_X2();
  if(XxN==3)  LT768.Font_Width_X3();
  if(XxN==4)  LT768.Font_Width_X4();

  //(*) 
  if(YxN==1)  LT768.Font_Height_X1();
  if(YxN==2)  LT768.Font_Height_X2();
  if(YxN==3)  LT768.Font_Height_X3();
  if(YxN==4)  LT768.Font_Height_X4();

  //(*)
  if(ChromaKey==0)  LT768.Font_Background_select_Color(); 
  if(ChromaKey==1)  LT768.Font_Background_select_Transparency();  

  //(*)
  if(Alignment==0)  LT768.Disable_Font_Alignment();
  if(Alignment==1)  LT768.Enable_Font_Alignment();  
  
  LT768_Lib.LT768_DMA_24bit_Linear(SCS,Clk,FlashAddr,MemoryAddr,Num);
  LT768.CGRAM_Start_address(MemoryAddr);        
}


void LT768Lib::LT768_Print_Outside_Font_String
(
 unsigned short x               // Font X coordinates
,unsigned short y               // Font Y coordinates
,unsigned long FontColor        
,unsigned long BackGroundColor  
,unsigned char *c                        // The first address of the data buffer
)
{
  unsigned short temp_H = 0;
  unsigned short temp_L = 0;
  unsigned short temp = 0;
  unsigned long i = 0;
  
  LT768.Text_Mode();
  LT768.Font_Select_UserDefine_Mode();
  LT768.Foreground_color_65k(FontColor);
  LT768.Background_color_65k(BackGroundColor);
  LT768.Goto_Text_XY(x,y);
  
  while(c[i] != '\0')
  { 
    if(c[i] < 0xa1)
    {
      LT768.CGROM_Select_Internal_CGROM();   //Internal CGROM :character source
      LT768_LCD.LCD_CmdWrite(0x04);
      LT768_LCD.LCD_DataWrite(c[i]);
      LT768.Check_Mem_WR_FIFO_not_Full();  
      i += 1;
    }
    else
    {
      LT768.Font_Select_UserDefine_Mode();  
      LT768_LCD.LCD_CmdWrite(0x04);
      temp_H = ((c[i] - 0xa1) & 0x00ff) * 94;
      temp_L = c[i+1] - 0xa1;
      temp = temp_H + temp_L + 0x8000;
      LT768_LCD.LCD_DataWrite((temp>>8)&0xff);
      LT768.Check_Mem_WR_FIFO_not_Full();
      LT768_LCD.LCD_DataWrite(temp&0xff);
      LT768.Check_Mem_WR_FIFO_not_Full();
      i += 2;   
    }
  }
  
  LT768.Check_2D_Busy();

  LT768.Graphic_Mode(); //back to graphic mode
}




void LT768Lib::LT768_Text_cursor_Init
(
 unsigned char On_Off_Blinking         
,unsigned short Blinking_Time         
,unsigned short X_W                    // Horizontal size
,unsigned short Y_W                    // Vertical size
)
{
  if(On_Off_Blinking == 0)  LT768.Disable_Text_Cursor_Blinking();
  if(On_Off_Blinking == 1)  LT768.Enable_Text_Cursor_Blinking();

  LT768.Blinking_Time_Frames(Blinking_Time); 
  
  //[3E][3Fh]
  LT768.Text_Cursor_H_V(X_W,Y_W);
  
  LT768.Enable_Text_Cursor();
}


void LT768Lib::LT768_Enable_Text_Cursor(void)
{
  LT768.Enable_Text_Cursor();
}


void LT768Lib::LT768_Disable_Text_Cursor(void)
{
  LT768.Disable_Text_Cursor();
}


void LT768Lib::LT768_Graphic_cursor_Init
(
 unsigned char Cursor_N                  // Select cursor : 1:cursor 1   2:cursor 2   3:cursor 3  4:cursor 4
,unsigned char Color1                    
,unsigned char Color2                    
,unsigned short X_Pos                    // X position
,unsigned short Y_Pos                    // Y position
,unsigned char *Cursor_Buf               
)
{
  unsigned int i ;
  
  LT768.Memory_Select_Graphic_Cursor_RAM(); 
  LT768.Graphic_Mode();
  
  switch(Cursor_N)
  {
    case 1: LT768.Select_Graphic_Cursor_1();  break;
    case 2: LT768.Select_Graphic_Cursor_2();  break;
    case 3: LT768.Select_Graphic_Cursor_3();  break;
    case 4: LT768.Select_Graphic_Cursor_4();  break;
    default:                            break;
  }
  
  LT768_LCD.LCD_CmdWrite(0x04);
  for(i=0;i<256;i++)
  {          
    LT768_LCD.LCD_DataWrite(Cursor_Buf[i]);
  }
  
  LT768.Memory_Select_SDRAM();//Back to SDRAM after finished
  LT768.Set_Graphic_Cursor_Color_1(Color1);
  LT768.Set_Graphic_Cursor_Color_2(Color2);
  LT768.Graphic_Cursor_XY(X_Pos,Y_Pos);
  
  LT768.Enable_Graphic_Cursor();
}


void LT768Lib::LT768_Set_Graphic_cursor_Pos
(
 unsigned char Cursor_N                  // Select cursor : 1:cursor 1   2:cursor 2   3:cursor 3  4:cursor 4
,unsigned short X_Pos                    // X position
,unsigned short Y_Pos                    // Y position
)
{
  LT768.Graphic_Cursor_XY(X_Pos,Y_Pos);
  switch(Cursor_N)
  {
    case 1: LT768.Select_Graphic_Cursor_1();  break;
    case 2: LT768.Select_Graphic_Cursor_2();  break;
    case 3: LT768.Select_Graphic_Cursor_3();  break;
    case 4: LT768.Select_Graphic_Cursor_4();  break;
    default:                            break;
  }
}


void LT768Lib::LT768_Enable_Graphic_Cursor(void)
{
  LT768.Enable_Graphic_Cursor();
}


void LT768Lib::LT768_Disable_Graphic_Cursor(void)
{
  LT768.Disable_Graphic_Cursor();
}


//-----------------------------------------------------------------------------------------------------------------------------

void LT768Lib::LT768_PIP_Init
(
 unsigned char On_Off         // 0 : forbid PIP    1 : enable PIP    2 : Keep the original state
,unsigned char Select_PIP     // 1 : use PIP1   2 : use PIP2
,unsigned long PAddr          // PIP begin address
,unsigned short XP            // PIP X position,must can be divided by 4
,unsigned short YP            // PIP Y position,must can be divided by 4
,unsigned long ImageWidth     
,unsigned short X_Dis         // Display window X position
,unsigned short Y_Dis         // Display window Y position
,unsigned short X_W           // Display window width,must can be divided by 4
,unsigned short Y_H           // Display window length��must can be divided by 4
)
{
  if(Select_PIP == 1 )  
  {
    LT768.Select_PIP1_Window_16bpp();
    LT768.Select_PIP1_Parameter();
  }
  if(Select_PIP == 2 )  
  {
    LT768.Select_PIP1_Window_16bpp();
    LT768.Select_PIP2_Parameter();
  }
  
  LT768.PIP_Display_Start_XY(X_Dis,Y_Dis);
  LT768.PIP_Image_Start_Address(PAddr);
  LT768.PIP_Image_Width(ImageWidth);
  LT768.PIP_Window_Image_Start_XY(XP,YP);
  LT768.PIP_Window_Width_Height(X_W,Y_H);

  if(On_Off == 0)
  {
    if(Select_PIP == 1 )  LT768.Disable_PIP1(); 
    if(Select_PIP == 2 )  LT768.Disable_PIP2();
  }

  if(On_Off == 1)
  {
    if(Select_PIP == 1 )  LT768.Enable_PIP1();  
    if(Select_PIP == 2 )  LT768.Enable_PIP2();
  }
}


void LT768Lib::LT768_Set_DisWindowPos
(
 unsigned char On_Off         // 0 : forbid PIP    1 : enable PIP    2 : Keep the original state
,unsigned char Select_PIP     // 1 : use PIP1   2 : use PIP2
,unsigned short X_Dis         // Display window X position
,unsigned short Y_Dis         // Display window Y position
)
{
  if(Select_PIP == 1 )  LT768.Select_PIP1_Parameter();
  if(Select_PIP == 2 )  LT768.Select_PIP2_Parameter();
  
  if(On_Off == 0)
  {
    if(Select_PIP == 1 )  LT768.Disable_PIP1(); 
    if(Select_PIP == 2 )  LT768.Disable_PIP2();
  }

  if(On_Off == 1)
  {
    if(Select_PIP == 1 )  LT768.Enable_PIP1();  
    if(Select_PIP == 2 )  LT768.Enable_PIP2();
  }
  
  LT768.PIP_Display_Start_XY(X_Dis,Y_Dis);
  
}

//-----------------------------------------------------------------------------------------------------------------------------


void LT768Lib::BTE_Solid_Fill
(
 unsigned long Des_Addr           // The destination address of the fill
,unsigned short Des_W             // Destination address picture width
,unsigned short XDes              // X position
,unsigned short YDes              // Y position
,unsigned short color             // Filled color
,unsigned short X_W               // Filled length
,unsigned short Y_H               // Filled width
)            
{
  
  LT768.BTE_Destination_Color_16bpp();
  
  LT768.BTE_Destination_Memory_Start_Address(Des_Addr);
    
  LT768.BTE_Destination_Image_Width(Des_W);
  LT768.BTE_Destination_Window_Start_XY(XDes,YDes);
  LT768.BTE_Window_Size(X_W,Y_H);
    
  LT768.Foreground_color_65k(color);
  LT768.BTE_Operation_Code(0x0c);
  LT768.BTE_Enable();
  LT768.Check_BTE_Busy();     
}


void LT768Lib::LT768_BTE_Memory_Copy
(
 unsigned long S0_Addr     // S0 image memory start address
,unsigned short S0_W       // S0 image width
,unsigned short XS0        // S0 image top left X coordinates
,unsigned short YS0        // S0 image top left Y coordinates
,unsigned long S1_Addr     // S1 image memory start address
,unsigned short S1_W       // S1 image width
,unsigned short XS1        // S1 image top left X coordinates
,unsigned short YS1        // S1 image top left Y coordinates
,unsigned long Des_Addr    // The memory starting address of the target image
,unsigned short Des_W      // The width of the target image
,unsigned short XDes       // The top left X coordinates
,unsigned short YDes       // The top left Y coordinates
,unsigned int ROP_Code     // Grating operation mode
/*ROP_Code :
   0000b    0(Blackness)
   0001b    ~S0!E~S1 or ~(S0+S1)
   0010b    ~S0!ES1
   0011b    ~S0
   0100b    S0!E~S1
   0101b    ~S1
   0110b    S0^S1
   0111b    ~S0 + ~S1 or ~(S0 + S1)
   1000b    S0!ES1
   1001b    ~(S0^S1)
   1010b    S1
   1011b    ~S0+S1
   1100b    S0
   1101b    S0+~S1
   1110b    S0+S1
   1111b    1(whiteness)*/
,unsigned short X_W       // active window's width
,unsigned short Y_H       // active window's heigth
)
{
  LT768.BTE_S0_Color_16bpp();
  LT768.BTE_S0_Memory_Start_Address(S0_Addr);
  LT768.BTE_S0_Image_Width(S0_W);
  LT768.BTE_S0_Window_Start_XY(XS0,YS0);

  LT768.BTE_S1_Color_16bpp();
  LT768.BTE_S1_Memory_Start_Address(S1_Addr);
  LT768.BTE_S1_Image_Width(S1_W); 
  LT768.BTE_S1_Window_Start_XY(XS1,YS1);

  LT768.BTE_Destination_Color_16bpp();
  LT768.BTE_Destination_Memory_Start_Address(Des_Addr);
  LT768.BTE_Destination_Image_Width(Des_W);
  LT768.BTE_Destination_Window_Start_XY(XDes,YDes); 
   
  LT768.BTE_ROP_Code(ROP_Code); 
  LT768.BTE_Operation_Code(0x02); //BTE Operation: Memory copy (move) with ROP.
  LT768.BTE_Window_Size(X_W,Y_H); 
  LT768.BTE_Enable();
  LT768.Check_BTE_Busy();
}


void LT768Lib::LT768_BTE_Memory_Copy_Chroma_key
(
 unsigned long S0_Addr     // S0 image memory start address
,unsigned short S0_W       // S0 image width
,unsigned short XS0        // S0 image top left X coordinates
,unsigned short YS0        // S0 image top left Y coordinates
,unsigned long Des_Addr    // The memory starting address of the target image
,unsigned short Des_W      // The width of the target image
,unsigned short XDes       // The top left X coordinates
,unsigned short YDes       // The top left Y coordinates
,unsigned long Background_color    
,unsigned short X_W       // active window's width
,unsigned short Y_H       // active window's heigth
)
{
  LT768.Background_color_65k(Background_color); 
  
  LT768.BTE_S0_Color_16bpp();
  LT768.BTE_S0_Memory_Start_Address(S0_Addr);
  LT768.BTE_S0_Image_Width(S0_W);
  LT768.BTE_S0_Window_Start_XY(XS0,YS0);  

  LT768.BTE_Destination_Color_16bpp();
  LT768.BTE_Destination_Memory_Start_Address(Des_Addr);
  LT768.BTE_Destination_Image_Width(Des_W);
  LT768.BTE_Destination_Window_Start_XY(XDes,YDes);
   
  LT768.BTE_Operation_Code(0x05); //BTE Operation: Memory copy (move) with chroma keying (w/o ROP)
  LT768.BTE_Window_Size(X_W,Y_H); 
  LT768.BTE_Enable();
  LT768.Check_BTE_Busy();
}


void LT768Lib::LT768_BTE_Pattern_Fill
(
 unsigned char P_8x8_or_16x16      // 0 : use 8x8 Icon , 1 : use 16x16 Icon.
,unsigned long S0_Addr             // S0 image memory start address
,unsigned short S0_W               // S0 image width
,unsigned short XS0                // S0 image top left X coordinates
,unsigned short YS0                // S0 image top left Y coordinates
,unsigned long Des_Addr            // The memory starting address of the target image
,unsigned short Des_W              // The width of the target image
,unsigned short XDes               // The top left X coordinates
,unsigned short YDes               // The top left Y coordinates
,unsigned int ROP_Code             // Grating operation mode
/*ROP_Code :
   0000b    0(Blackness)
   0001b    ~S0!E~S1 or ~(S0+S1)
   0010b    ~S0!ES1
   0011b    ~S0
   0100b    S0!E~S1
   0101b    ~S1
   0110b    S0^S1
   0111b    ~S0 + ~S1 or ~(S0 + S1)
   1000b    S0!ES1
   1001b    ~(S0^S1)
   1010b    S1
   1011b    ~S0+S1
   1100b    S0
   1101b    S0+~S1
   1110b    S0+S1
   1111b    1(whiteness)*/
,unsigned short X_W       // active window's width
,unsigned short Y_H       // active window's heigth
)
{
  if(P_8x8_or_16x16 == 0)
  {
    LT768.Pattern_Format_8X8();
  }
  if(P_8x8_or_16x16 == 1)
  {                               
    LT768.Pattern_Format_16X16();
  } 
  
  LT768.BTE_S0_Color_16bpp();
  LT768.BTE_S0_Memory_Start_Address(S0_Addr);
  LT768.BTE_S0_Image_Width(S0_W);
  LT768.BTE_S0_Window_Start_XY(XS0,YS0);

  LT768.BTE_Destination_Color_16bpp();
  LT768.BTE_Destination_Memory_Start_Address(Des_Addr);
  LT768.BTE_Destination_Image_Width(Des_W);
  LT768.BTE_Destination_Window_Start_XY(XDes,YDes); 
   
  LT768.BTE_ROP_Code(ROP_Code); 
  LT768.BTE_Operation_Code(0x06); //BTE Operation: Pattern Fill with ROP.
  LT768.BTE_Window_Size(X_W,Y_H); 
  LT768.BTE_Enable();
  LT768.Check_BTE_Busy();
}



void LT768Lib::LT768_BTE_Pattern_Fill_With_Chroma_key
(
 unsigned char P_8x8_or_16x16      // 0 : use 8x8 Icon , 1 : use 16x16 Icon.
,unsigned long S0_Addr             // S0 image memory start address
,unsigned short S0_W               // S0 image width
,unsigned short XS0                // S0 image top left X coordinates
,unsigned short YS0                // S0 image top left Y coordinates
,unsigned long Des_Addr            // The memory starting address of the target image
,unsigned short Des_W              // The width of the target image
,unsigned short XDes               // The top left X coordinates
,unsigned short YDes               // The top left Y coordinates
,unsigned int ROP_Code             // Grating operation mode
/*ROP_Code :
   0000b    0(Blackness)
   0001b    ~S0!E~S1 or ~(S0+S1)
   0010b    ~S0!ES1
   0011b    ~S0
   0100b    S0!E~S1
   0101b    ~S1
   0110b    S0^S1
   0111b    ~S0 + ~S1 or ~(S0 + S1)
   1000b    S0!ES1
   1001b    ~(S0^S1)
   1010b    S1
   1011b    ~S0+S1
   1100b    S0
   1101b    S0+~S1
   1110b    S0+S1
   1111b    1(whiteness)*/
,unsigned long Background_color   
,unsigned short X_W       // active window's width
,unsigned short Y_H       // active window's heigth
)
{
  LT768.Background_color_65k(Background_color);
  
  if(P_8x8_or_16x16 == 0)
  {
  LT768.Pattern_Format_8X8();
   }
  if(P_8x8_or_16x16 == 1)
  {                               
  LT768.Pattern_Format_16X16();
  }   
  
  LT768.BTE_S0_Color_16bpp();
  LT768.BTE_S0_Memory_Start_Address(S0_Addr);
  LT768.BTE_S0_Image_Width(S0_W);
  LT768.BTE_S0_Window_Start_XY(XS0,YS0);

  LT768.BTE_Destination_Color_16bpp();
  LT768.BTE_Destination_Memory_Start_Address(Des_Addr);
  LT768.BTE_Destination_Image_Width(Des_W);
  LT768.BTE_Destination_Window_Start_XY(XDes,YDes); 
   
  LT768.BTE_ROP_Code(ROP_Code); 
  LT768.BTE_Operation_Code(0x07); //BTE Operation: Pattern Fill with Chroma key.
  LT768.BTE_Window_Size(X_W,Y_H); 
  LT768.BTE_Enable();
  LT768.Check_BTE_Busy();
}



void LT768Lib::LT768_BTE_MCU_Write_MCU_16bit
(
 unsigned long S1_Addr     // S1 image memory start address
,unsigned short S1_W       // S1 image width
,unsigned short XS1        // S1 image top left X coordinates
,unsigned short YS1        // S1 image top left Y coordinates
,unsigned long Des_Addr    // The memory starting address of the target image
,unsigned short Des_W      // The width of the target image
,unsigned short XDes       // The top left X coordinates
,unsigned short YDes       // The top left Y coordinates
,unsigned int ROP_Code     // Grating operation mode
/*ROP_Code :
   0000b    0(Blackness)
   0001b    ~S0!E~S1 or ~(S0+S1)
   0010b    ~S0!ES1
   0011b    ~S0
   0100b    S0!E~S1
   0101b    ~S1
   0110b    S0^S1
   0111b    ~S0 + ~S1 or ~(S0 + S1)
   1000b    S0!ES1
   1001b    ~(S0^S1)
   1010b    S1
   1011b    ~S0+S1
   1100b    S0
   1101b    S0+~S1
   1110b    S0+S1
   1111b    1(whiteness)*/
,unsigned short X_W                 // active window's width
,unsigned short Y_H                 // active window's heigth
,const unsigned short *data         // S0 data buffer first address
)
{
  unsigned short i,j;

  LT768.BTE_S1_Color_16bpp();
  LT768.BTE_S1_Memory_Start_Address(S1_Addr);
  LT768.BTE_S1_Image_Width(S1_W); 
  LT768.BTE_S1_Window_Start_XY(XS1,YS1);

  LT768.BTE_Destination_Color_16bpp();
  LT768.BTE_Destination_Memory_Start_Address(Des_Addr);
  LT768.BTE_Destination_Image_Width(Des_W);
  LT768.BTE_Destination_Window_Start_XY(XDes,YDes);
  
  LT768.BTE_Window_Size(X_W,Y_H);
  LT768.BTE_ROP_Code(ROP_Code);
  LT768.BTE_Operation_Code(0x00);   //BTE Operation: MPU Write with ROP.
  LT768.BTE_Enable();
  
  LT768.BTE_S0_Color_16bpp();
  LT768_LCD.LCD_CmdWrite(0x04);           //Memory Data Read/Write Port
  
  //MCU_16bit_ColorDepth_16bpp
  for(i=0;i< Y_H;i++)
  { 
    for(j=0;j< (X_W);j++)
    {
      LT768.Check_Mem_WR_FIFO_not_Full();
      LT768_LCD.LCD_DataWrite_Pixel((*data));
      data++;
    }
  }
  LT768.Check_Mem_WR_FIFO_Empty();
  LT768.Check_BTE_Busy();
}



void LT768Lib::LT768_BTE_MCU_Write_Chroma_key_MCU_16bit
(
 unsigned long Des_Addr                 // The memory starting address of the target image
,unsigned short Des_W                   // The width of the target image
,unsigned short XDes                    // The top left X coordinates
,unsigned short YDes                    // The top left Y coordinates
,unsigned long Background_color         
,unsigned short X_W                     // active window's width
,unsigned short Y_H                     // active window's heigth
,const unsigned short *data             // S0 data recive address
)
{
  unsigned int i,j;
  
  LT768.Background_color_65k(Background_color);
  
  LT768.BTE_Destination_Color_16bpp();
  LT768.BTE_Destination_Memory_Start_Address(Des_Addr);
  LT768.BTE_Destination_Image_Width(Des_W);
  LT768.BTE_Destination_Window_Start_XY(XDes,YDes);
  
  LT768.BTE_Window_Size(X_W,Y_H);
  LT768.BTE_Operation_Code(0x04);   //BTE Operation: MPU Write with chroma keying (w/o ROP)
  LT768.BTE_Enable();
  
  LT768.BTE_S0_Color_16bpp();
  LT768_LCD.LCD_CmdWrite(0x04);     //Memory Data Read/Write Port
  
  //MCU_16bit_ColorDepth_16bpp
  for(i=0;i< Y_H;i++)
  { 
    for(j=0;j< (X_W);j++)
    {
      LT768.Check_Mem_WR_FIFO_not_Full();
      LT768_LCD.LCD_DataWrite_Pixel((*data));
      data++;
    }
  }
  LT768.Check_Mem_WR_FIFO_Empty();
  LT768.Check_BTE_Busy();
}



void LT768Lib::LT768_BTE_MCU_Write_ColorExpansion_MCU_16bit
(
 unsigned long Des_Addr               // The memory starting address of the target image
,unsigned short Des_W                 // The width of the target image
,unsigned short XDes                  // The top left X coordinates
,unsigned short YDes                  // The top left Y coordinates
,unsigned short X_W                   // active window's width
,unsigned short Y_H                   // active window's heigth
,unsigned long Foreground_color       
/*Foreground_color : The source (1bit map picture) map data 1 translate to Foreground color by color expansion*/
,unsigned long Background_color       
/*Background_color : The source (1bit map picture) map data 0 translate to Background color by color expansion*/
,const unsigned short *data           // Data buffer first address
)
{
  unsigned short i,j;
  
  LT768.RGB_16b_16bpp();
  LT768.Foreground_color_65k(Foreground_color);
  LT768.Background_color_65k(Background_color);
  LT768.BTE_ROP_Code(15);
  
  LT768.BTE_Destination_Color_16bpp();
  LT768.BTE_Destination_Memory_Start_Address(Des_Addr);
  LT768.BTE_Destination_Image_Width(Des_W);
  LT768.BTE_Destination_Window_Start_XY(XDes,YDes);
  

  LT768.BTE_Window_Size(X_W,Y_H);
  LT768.BTE_Operation_Code(0x8);    //BTE Operation: MPU Write with Color Expansion (w/o ROP)
  LT768.BTE_Enable();
  
  LT768_LCD.LCD_CmdWrite(0x04);           //Memory Data Read/Write Port  
  for(i=0;i< Y_H;i++)
  { 
    for(j=0;j< X_W/16;j++)
    {
      LT768.Check_Mem_WR_FIFO_not_Full();
      LT768_LCD.LCD_DataWrite_Pixel(*data);  
      data++;
    }
  }
  LT768.Check_Mem_WR_FIFO_Empty();
  LT768.Check_BTE_Busy();
}


void LT768Lib::LT768_BTE_MCU_Write_ColorExpansion_Chroma_key_MCU_16bit
(
 unsigned long Des_Addr               // The memory starting address of the target image
,unsigned short Des_W                 // The width of the target image
,unsigned short XDes                  // The top left X coordinates
,unsigned short YDes                  // The top left Y coordinates
,unsigned short X_W                   // active window's width
,unsigned short Y_H                   // active window's heigth
,unsigned long Foreground_color    
/*Foreground_color : The source (1bit map picture) map data 1 translate to Foreground color by color expansion*/
,const unsigned short *data           // Data buffer first address
)
{
  unsigned short i,j;
  
  LT768.RGB_16b_16bpp();
  LT768.Foreground_color_65k(Foreground_color);
  LT768.BTE_ROP_Code(15);
  
  LT768.BTE_Destination_Color_16bpp();
  LT768.BTE_Destination_Memory_Start_Address(Des_Addr);
  LT768.BTE_Destination_Image_Width(Des_W);
  LT768.BTE_Destination_Window_Start_XY(XDes,YDes);
  

  LT768.BTE_Window_Size(X_W,Y_H);
  LT768.BTE_Operation_Code(0x9);    //BTE Operation: MPU Write with Color Expansion and chroma keying (w/o ROP)
  LT768.BTE_Enable();
  
  LT768_LCD.LCD_CmdWrite(0x04);           //Memory Data Read/Write Port  
  for(i=0;i< Y_H;i++)
  { 
    for(j=0;j< X_W/16;j++)
    {
      LT768.Check_Mem_WR_FIFO_not_Full();
      LT768_LCD.LCD_DataWrite_Pixel(*data);  
      data++;
    }
  }
  LT768.Check_Mem_WR_FIFO_Empty();
  LT768.Check_BTE_Busy();
}

void LT768Lib::BTE_Alpha_Blending
(
 unsigned long S0_Addr     // S0 image memory start address
,unsigned short S0_W       // S0 image width
,unsigned short XS0        // S0 image top left X coordinates
,unsigned short YS0        // S0 image top left Y coordinates
,unsigned long S1_Addr     // S1 image memory start address
,unsigned short S1_W       // S1 image width
,unsigned short XS1        // S1 image top left X coordinates
,unsigned short YS1        // S1 image top left Y coordinates
,unsigned long Des_Addr    // The memory starting address of the target image
,unsigned short Des_W      // The width of the target image
,unsigned short XDes       // The top left X coordinates
,unsigned short YDes       // The top left Y coordinates
,unsigned short X_W        // active window's width
,unsigned short Y_H        // active window's heigth
,unsigned char alpha       // The level of transparency(0~31)
)
{ 
  LT768.BTE_S0_Color_16bpp();
  LT768.BTE_S0_Memory_Start_Address(S0_Addr);
  LT768.BTE_S0_Image_Width(S0_W);
  LT768.BTE_S0_Window_Start_XY(XS0,YS0);

  LT768.BTE_S1_Color_16bpp();
  LT768.BTE_S1_Memory_Start_Address(S1_Addr);
  LT768.BTE_S1_Image_Width(S1_W); 
  LT768.BTE_S1_Window_Start_XY(XS1,YS1);

  LT768.BTE_Destination_Color_16bpp();
  LT768.BTE_Destination_Memory_Start_Address(Des_Addr);
  LT768.BTE_Destination_Image_Width(Des_W);
  LT768.BTE_Destination_Window_Start_XY(XDes,YDes);

  LT768.BTE_Window_Size(X_W,Y_H);
  LT768.BTE_Operation_Code(0x0A);   //BTE Operation: Memory write with opacity (w/o ROP)
  LT768.BTE_Alpha_Blending_Effect(alpha);
  LT768.BTE_Enable();
  LT768.Check_BTE_Busy();
}


//----------------------------------------------------------------------------------------------------------------------------------

void LT768Lib::LT768_PWM0_Init
(
 unsigned char on_off                       // 0��disable PWM0    1��enbale PWM0
,unsigned char Clock_Divided                // PWM clock  0~3(1,1/2,1/4,1/8)
,unsigned char Prescalar                    // clock division  1~256
,unsigned short Count_Buffer                // Setting the output cycle of the PWM
,unsigned short Compare_Buffer              // Set up duty ratio
)
{
   LT768.Select_PWM0();
   LT768.Set_PWM_Prescaler_1_to_256(Prescalar);

  if(Clock_Divided ==0) LT768.Select_PWM0_Clock_Divided_By_1();
  if(Clock_Divided ==1) LT768.Select_PWM0_Clock_Divided_By_2();
  if(Clock_Divided ==2) LT768.Select_PWM0_Clock_Divided_By_4();
  if(Clock_Divided ==3) LT768.Select_PWM0_Clock_Divided_By_8();

  LT768.Set_Timer0_Count_Buffer(Count_Buffer);  
  LT768.Set_Timer0_Compare_Buffer(Compare_Buffer);  
    
  if (on_off == 1)  LT768.Start_PWM0(); 
  if (on_off == 0)  LT768.Stop_PWM0();
}


void LT768Lib::LT768_PWM0_Duty(unsigned short Compare_Buffer)
{
  LT768.Set_Timer0_Compare_Buffer(Compare_Buffer);
}



void LT768Lib::LT768_PWM1_Init
(
 unsigned char on_off                       // 0��disable PWM0    1��enbale PWM0
,unsigned char Clock_Divided                // PWM clock  0~3(1,1/2,1/4,1/8)
,unsigned char Prescalar                    // clock division  1~256
,unsigned short Count_Buffer                // Setting the output cycle of the PWM
,unsigned short Compare_Buffer              // Set up duty ratio
)
{
  LT768.Select_PWM1();
  LT768.Set_PWM_Prescaler_1_to_256(Prescalar);
 
  if(Clock_Divided ==0) LT768.Select_PWM1_Clock_Divided_By_1();
  if(Clock_Divided ==1) LT768.Select_PWM1_Clock_Divided_By_2();
  if(Clock_Divided ==2) LT768.Select_PWM1_Clock_Divided_By_4();
  if(Clock_Divided ==3) LT768.Select_PWM1_Clock_Divided_By_8();

  LT768.Set_Timer1_Count_Buffer(Count_Buffer); 
  LT768.Set_Timer1_Compare_Buffer(Compare_Buffer); 

  if (on_off == 1)  LT768.Start_PWM1(); 
  if (on_off == 0)  LT768.Stop_PWM1();
}


void LT768Lib::LT768_PWM1_Duty(unsigned short Compare_Buffer)
{
  LT768.Set_Timer1_Compare_Buffer(Compare_Buffer);
}

//----------------------------------------------------------------------------------------------------------------------------------

// LT768 Enter the standby mode
void LT768Lib::LT768_Standby(void)
{
  LT768.Power_Saving_Standby_Mode();
  LT768.Check_Power_is_Saving();
}
// Awaken from the standby mode
void LT768Lib::LT768_Wkup_Standby(void)
{
  LT768.Power_Normal_Mode();
  LT768.Check_Power_is_Normal();
}


// LT768 Enter the pause mode
void LT768Lib::LT768_Suspend(void)
{
  LT768_SDRAM_initail(LT768_Lib_MCLK);
  LT768.Power_Saving_Suspend_Mode();
  LT768.Check_Power_is_Saving();
}
// Awaken from the pause mode
void LT768Lib::LT768_Wkup_Suspend(void)
{
  LT768.Power_Normal_Mode();
  LT768.Check_Power_is_Normal();
  LT768_SDRAM_initail(LT768_Lib_MCLK);
}


// LT768 Enter the sleep mode
void LT768Lib::LT768_SleepMode(void)
{
  LT768.Power_Saving_Sleep_Mode();
  LT768.Check_Power_is_Saving();
}
// Awaken from the sleep mode
void LT768Lib::LT768_Wkup_Sleep(void)
{
  LT768.Power_Normal_Mode();
  LT768.Check_Power_is_Normal();
}
LT768Lib LT768_Lib=LT768Lib();