#ifndef LT768_Lib_h
#define LT768_Lib_h

#include "Arduino.h"
#include "LT768_LCD.h"
#include "LT768.h"
#include <SPI.h>
#include <Wire.h>

#define LCD_XSIZE_TFT    800
#define LCD_YSIZE_TFT    480

#define LCD_VBPD         23
#define LCD_VFPD         22
#define LCD_VSPW         1
#define LCD_HBPD         46
#define LCD_HFPD         210
#define LCD_HSPW         1

#define LCD_RSTN  47

#define LCD_PCLK_Falling_Rising     1      // 1:FallingEdge    0:RisingEdge
#define LCD_HSYNC_Active_Polarity   0      // 1:HighLevel    0:LowLevel
#define LCD_VSYNC_Active_Polarity   0      // 1:HighLevel    0:LowLevel
#define LCD_DE_Active_Polarity      1      // 1:HighLevel    0:LowLevel


class LT768Lib
{
	private:
		
  public:
		void Set_PCLK(unsigned char val);
		void Set_HSYNC_Active(unsigned char val);
		void Set_VSYNC_Active(unsigned char val);
		void Set_DE_Active(unsigned char val);
		void LT768_HW_Reset(void);
		void System_Check_Temp(void);
		void LT768_PLL_Initial(void);
		void LT768_SDRAM_initail(unsigned char mclk);
		void Set_LCD_Panel(void);
		void LT768_initial(void);
		void LT768_Init(void);
		void MPU8_8bpp_Memory_Write
(
unsigned short x //x of coordinate 
,unsigned short y // y of coordinate 
,unsigned short w //width
,unsigned short h //height
,const unsigned char *data //8bit data
);
		void MPU8_16bpp_Memory_Write
(
 unsigned short x // x of coordinate 
,unsigned short y // y of coordinate 
,unsigned short w // width
,unsigned short h // height
,const unsigned char *data // 8bit data
);
		void MPU8_24bpp_Memory_Write 
(
unsigned short x // x of coordinate
,unsigned short y // y of coordinate 
,unsigned short w // width
,unsigned short h // height
,const unsigned char *data // 8bit data
);
		void MPU16_16bpp_Memory_Write 
(
 unsigned short x       //x of coordinate
,unsigned short y             //y of coordinate 
,unsigned short w             //width
,unsigned short h             //height
,const unsigned short *data   //16bit data
);
		void MPU16_24bpp_Mode1_Memory_Write 
(
unsigned short x //x of coordinate
,unsigned short y //y of coordinate
,unsigned short w //width
,unsigned short h //height
,const unsigned short *data //16bit data
);
		void MPU16_24bpp_Mode2_Memory_Write
(
unsigned short x //x of coordinate
,unsigned short y //y of coordinate
,unsigned short w //width
,unsigned short h //height
,const unsigned short *data //16bit data
);
		void LT768_DrawLine
(
 unsigned short X1        
,unsigned short Y1        
,unsigned short X2        
,unsigned short Y2        
,unsigned long  LineColor 
);
		void LT768_DrawLine_Width
(
 unsigned short X1        
,unsigned short Y1        
,unsigned short X2        
,unsigned short Y2        
,unsigned long  LineColor 
,unsigned short Width     
);
		void LT768_DrawCircle
(
 unsigned short XCenter           
,unsigned short YCenter           
,unsigned short R                 
,unsigned long CircleColor        
);
		void LT768_DrawCircle_Fill
(
 unsigned short XCenter           
,unsigned short YCenter           
,unsigned short R                 
,unsigned long ForegroundColor    
);
		void LT768_DrawCircle_Width
(
 unsigned short XCenter          
,unsigned short YCenter          
,unsigned short R                
,unsigned long CircleColor       
,unsigned long ForegroundColor   
,unsigned short Width            
);
		void LT768_DrawEllipse
(
 unsigned short XCenter          
,unsigned short YCenter          
,unsigned short X_R              
,unsigned short Y_R              
,unsigned long EllipseColor      
);
		void LT768_DrawEllipse_Fill
(
 unsigned short XCenter          
,unsigned short YCenter         
,unsigned short X_R              
,unsigned short Y_R             
,unsigned long ForegroundColor  
);
		void LT768_DrawEllipse_Width
(
 unsigned short XCenter        
,unsigned short YCenter          
,unsigned short X_R          
,unsigned short Y_R          
,unsigned long EllipseColor  
,unsigned long ForegroundColor   
,unsigned short Width       
);
		void LT768_DrawSquare
(
 unsigned short X1                
,unsigned short Y1                
,unsigned short X2                
,unsigned short Y2             
,unsigned long SquareColor      
);
		void LT768_DrawSquare_Fill
(
 unsigned short X1                
,unsigned short Y1              
,unsigned short X2                
,unsigned short Y2              
,unsigned long ForegroundColor   
);
		void LT768_DrawSquare_Width
(
 unsigned short X1                
,unsigned short Y1                
,unsigned short X2               
,unsigned short Y2                
,unsigned long SquareColor       
,unsigned long ForegroundColor   
,unsigned short Width         
);
		void LT768_DrawCircleSquare
(
 unsigned short X1                
,unsigned short Y1                
,unsigned short X2                
,unsigned short Y2                
,unsigned short X_R              
,unsigned short Y_R              
,unsigned long CircleSquareColor
);
		void LT768_DrawCircleSquare_Fill
(
 unsigned short X1              
,unsigned short Y1                
,unsigned short X2               
,unsigned short Y2                
,unsigned short X_R               
,unsigned short Y_R             
,unsigned long ForegroundColor  
);
		void LT768_DrawCircleSquare_Width
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
);
		void LT768_DrawTriangle
(
 unsigned short X1             
,unsigned short Y1            
,unsigned short X2           
,unsigned short Y2              
,unsigned short X3              
,unsigned short Y3              
,unsigned long TriangleColor  
);
		void LT768_DrawTriangle_Fill
(
 unsigned short X1             
,unsigned short Y1             
,unsigned short X2            
,unsigned short Y2            
,unsigned short X3           
,unsigned short Y3             
,unsigned long ForegroundColor 
);
		void LT768_DrawLeftUpCurve
( 
 unsigned short XCenter          
,unsigned short YCenter          
,unsigned short X_R              
,unsigned short Y_R               
,unsigned long CurveColor        
);
		void LT768_DrawLeftDownCurve
(
 unsigned short XCenter          
,unsigned short YCenter          
,unsigned short X_R              
,unsigned short Y_R             
,unsigned long CurveColor        
);
		void LT768_DrawRightUpCurve
(
 unsigned short XCenter          
,unsigned short YCenter        
,unsigned short X_R            
,unsigned short Y_R              
,unsigned long CurveColor      
);
		void LT768_DrawRightDownCurve
(
 unsigned short XCenter        
,unsigned short YCenter        
,unsigned short X_R            
,unsigned short Y_R              
,unsigned long CurveColor     
);
		void LT768_SelectDrawCurve
(
 unsigned short XCenter       
,unsigned short YCenter          
,unsigned short X_R             
,unsigned short Y_R              
,unsigned long CurveColor        
,unsigned short  Dir             
);
		void LT768_DrawLeftUpCurve_Fill
(
 unsigned short XCenter           
,unsigned short YCenter           
,unsigned short X_R             
,unsigned short Y_R               
,unsigned long ForegroundColor   
);
		void LT768_DrawLeftDownCurve_Fill
(
 unsigned short XCenter          
,unsigned short YCenter          
,unsigned short X_R              
,unsigned short Y_R          
,unsigned long ForegroundColor   
);
		void LT768_DrawRightUpCurve_Fill
(
 unsigned short XCenter          
,unsigned short YCenter        
,unsigned short X_R            
,unsigned short Y_R            
,unsigned long ForegroundColor   
);
		void LT768_DrawRightDownCurve_Fill
(
 unsigned short XCenter          
,unsigned short YCenter         
,unsigned short X_R               
,unsigned short Y_R             
,unsigned long ForegroundColor    
);
		void LT768_SelectDrawCurve_Fill
(
 unsigned short XCenter           
,unsigned short YCenter           
,unsigned short X_R              
,unsigned short Y_R              
,unsigned long CurveColor         
,unsigned short  Dir            
);
		void LT768_DrawQuadrilateral
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
);
		void LT768_DrawQuadrilateral_Fill
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
);
		void LT768_DrawPentagon
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
);
		void LT768_DrawPentagon_Fill
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
);
		unsigned char LT768_DrawCylinder
(
 unsigned short XCenter           
,unsigned short YCenter         
,unsigned short X_R              
,unsigned short Y_R              
,unsigned short H                
,unsigned long CylinderColor     
,unsigned long ForegroundColor   
);
		void LT768_DrawQuadrangular
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
);
		void LT768_MakeTable
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
);
		void LT768_Color_Bar_ON(void);
		void LT768_Color_Bar_OFF(void);
		void LT768_DMA_24bit_Linear
(
 unsigned char SCS              // Select SPI : SCS��0       SCS��1
,unsigned char Clk              // SPI Clock = System Clock /{(Clk+1);*2}
,unsigned long flash_addr       // Data flash address  
,unsigned long memory_addr      // Data transfers to SDRAM address
,unsigned long data_num         // Data size
);
		void LT768_DMA_32bit_Linear
(
 unsigned char SCS              // Select SPI : SCS��0       SCS��1
,unsigned char Clk              // SPI Clock = System Clock /{(Clk+1);*2}
,unsigned long flash_addr       // Data flash address   
,unsigned long memory_addr      // Data transfers to SDRAM address
,unsigned long data_num         // Data size
);
		void LT768_DMA_24bit_Block
(
 unsigned char SCS         // Select SPI : SCS��0       SCS��1
,unsigned char Clk         // SPI Clock = System Clock /{(Clk+1);*2}
,unsigned short X1         // Transfer to SDRAM address:X1
,unsigned short Y1         // Transfer to SDRAM address:Y1
,unsigned short X_W        // DMA data width
,unsigned short Y_H        // DMA data height
,unsigned short P_W        // Picture's width
,unsigned long Addr        // Flash address
);
		void LT768_DMA_32bit_Block
(
 unsigned char SCS         // Select SPI : SCS��0       SCS��1
,unsigned char Clk         // SPI Clock = System Clock /{(Clk+1);*2}
,unsigned short X1         // Transfer to SDRAM address:X1
,unsigned short Y1         // Transfer to SDRAM address:Y1
,unsigned short X_W        // DMA data width
,unsigned short Y_H        // DMA data height
,unsigned short P_W        // Picture's width
,unsigned long Addr        // Flash address
);
		void LT768_Select_Internal_Font_Init
(
 unsigned char Size         // Setting font size  16��16*16     24:24*24    32:32*32
,unsigned char XxN          // Width magnification:1~4
,unsigned char YxN          // Heigtht magnification:1~4
,unsigned char ChromaKey    // 0��not transparent   1��transparent
,unsigned char Alignment    // 0��font not align      1��font align
);
		void LT768_Print_Internal_Font_String
(
 unsigned short x               // Font X coordinates
,unsigned short y               // Font Y coordinates
,unsigned long FontColor        
,unsigned long BackGroundColor  
,char *c                        // The first address of the data buffer
);
		void LT768_Select_Outside_Font_Init
(
 unsigned char SCS           // Select SPI : SCS��0       SCS��1
,unsigned char Clk           // SPI Clock = System Clock /{(Clk+1);*2}
,unsigned long FlashAddr     
,unsigned long MemoryAddr    
,unsigned long Num           // Font data Num
,unsigned char Size          // Font size  16��16*16     24:24*24    32:32*32
,unsigned char XxN          // Width magnification:1~4
,unsigned char YxN          // Heigtht magnification:1~4
,unsigned char ChromaKey    // 0��not transparent   1��transparent
,unsigned char Alignment    // 0��font not align      1��font align
);
		void LT768_Print_Outside_Font_String
(
 unsigned short x               // Font X coordinates
,unsigned short y               // Font Y coordinates
,unsigned long FontColor        
,unsigned long BackGroundColor  
,unsigned char *c                        // The first address of the data buffer
);
		void LT768_Text_cursor_Init
(
 unsigned char On_Off_Blinking         
,unsigned short Blinking_Time         
,unsigned short X_W                    // Horizontal size
,unsigned short Y_W                    // Vertical size
);
		void LT768_Enable_Text_Cursor(void);
		void LT768_Disable_Text_Cursor(void);
		void LT768_Graphic_cursor_Init
(
 unsigned char Cursor_N                  // Select cursor : 1:cursor 1   2:cursor 2   3:cursor 3  4:cursor 4
,unsigned char Color1                    
,unsigned char Color2                    
,unsigned short X_Pos                    // X position
,unsigned short Y_Pos                    // Y position
,unsigned char *Cursor_Buf               
);
		void LT768_Set_Graphic_cursor_Pos
(
 unsigned char Cursor_N                  // Select cursor : 1:cursor 1   2:cursor 2   3:cursor 3  4:cursor 4
,unsigned short X_Pos                    // X position
,unsigned short Y_Pos                    // Y position
);
		void LT768_Enable_Graphic_Cursor(void);
		void LT768_Disable_Graphic_Cursor(void);
		void LT768_PIP_Init
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
);
		void LT768_Set_DisWindowPos
(
 unsigned char On_Off         // 0 : forbid PIP    1 : enable PIP    2 : Keep the original state
,unsigned char Select_PIP     // 1 : use PIP1   2 : use PIP2
,unsigned short X_Dis         // Display window X position
,unsigned short Y_Dis         // Display window Y position
);
		void BTE_Solid_Fill
(
 unsigned long Des_Addr           // The destination address of the fill
,unsigned short Des_W             // Destination address picture width
,unsigned short XDes              // X position
,unsigned short YDes              // Y position
,unsigned short color             // Filled color
,unsigned short X_W               // Filled length
,unsigned short Y_H               // Filled width
);
		void LT768_BTE_Memory_Copy
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
   0000b    0(Blackness);
   0001b    ~S0!E~S1 or ~(S0+S1);
   0010b    ~S0!ES1
   0011b    ~S0
   0100b    S0!E~S1
   0101b    ~S1
   0110b    S0^S1
   0111b    ~S0 + ~S1 or ~(S0 + S1);
   1000b    S0!ES1
   1001b    ~(S0^S1);
   1010b    S1
   1011b    ~S0+S1
   1100b    S0
   1101b    S0+~S1
   1110b    S0+S1
   1111b    1(whiteness);*/
,unsigned short X_W       // active window's width
,unsigned short Y_H       // active window's heigth
);
		void LT768_BTE_Memory_Copy_Chroma_key
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
);
		void LT768_BTE_Pattern_Fill
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
   0000b    0(Blackness);
   0001b    ~S0!E~S1 or ~(S0+S1);
   0010b    ~S0!ES1
   0011b    ~S0
   0100b    S0!E~S1
   0101b    ~S1
   0110b    S0^S1
   0111b    ~S0 + ~S1 or ~(S0 + S1);
   1000b    S0!ES1
   1001b    ~(S0^S1);
   1010b    S1
   1011b    ~S0+S1
   1100b    S0
   1101b    S0+~S1
   1110b    S0+S1
   1111b    1(whiteness);*/
,unsigned short X_W       // active window's width
,unsigned short Y_H       // active window's heigth
);
		void LT768_BTE_Pattern_Fill_With_Chroma_key
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
   0000b    0(Blackness);
   0001b    ~S0!E~S1 or ~(S0+S1);
   0010b    ~S0!ES1
   0011b    ~S0
   0100b    S0!E~S1
   0101b    ~S1
   0110b    S0^S1
   0111b    ~S0 + ~S1 or ~(S0 + S1);
   1000b    S0!ES1
   1001b    ~(S0^S1);
   1010b    S1
   1011b    ~S0+S1
   1100b    S0
   1101b    S0+~S1
   1110b    S0+S1
   1111b    1(whiteness);*/
,unsigned long Background_color   
,unsigned short X_W       // active window's width
,unsigned short Y_H       // active window's heigth
);
		void LT768_BTE_MCU_Write_MCU_16bit
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
   0000b    0(Blackness);
   0001b    ~S0!E~S1 or ~(S0+S1);
   0010b    ~S0!ES1
   0011b    ~S0
   0100b    S0!E~S1
   0101b    ~S1
   0110b    S0^S1
   0111b    ~S0 + ~S1 or ~(S0 + S1);
   1000b    S0!ES1
   1001b    ~(S0^S1);
   1010b    S1
   1011b    ~S0+S1
   1100b    S0
   1101b    S0+~S1
   1110b    S0+S1
   1111b    1(whiteness);*/
,unsigned short X_W                 // active window's width
,unsigned short Y_H                 // active window's heigth
,const unsigned short *data         // S0 data buffer first address
);
		void LT768_BTE_MCU_Write_Chroma_key_MCU_16bit
(
 unsigned long Des_Addr                 // The memory starting address of the target image
,unsigned short Des_W                   // The width of the target image
,unsigned short XDes                    // The top left X coordinates
,unsigned short YDes                    // The top left Y coordinates
,unsigned long Background_color         
,unsigned short X_W                     // active window's width
,unsigned short Y_H                     // active window's heigth
,const unsigned short *data             // S0 data recive address
);
		void LT768_BTE_MCU_Write_ColorExpansion_MCU_16bit
(
 unsigned long Des_Addr               // The memory starting address of the target image
,unsigned short Des_W                 // The width of the target image
,unsigned short XDes                  // The top left X coordinates
,unsigned short YDes                  // The top left Y coordinates
,unsigned short X_W                   // active window's width
,unsigned short Y_H                   // active window's heigth
,unsigned long Foreground_color       
/*Foreground_color : The source (1bit map picture); map data 1 translate to Foreground color by color expansion*/
,unsigned long Background_color       
/*Background_color : The source (1bit map picture); map data 0 translate to Background color by color expansion*/
,const unsigned short *data           // Data buffer first address
);
		void LT768_BTE_MCU_Write_ColorExpansion_Chroma_key_MCU_16bit
(
 unsigned long Des_Addr               // The memory starting address of the target image
,unsigned short Des_W                 // The width of the target image
,unsigned short XDes                  // The top left X coordinates
,unsigned short YDes                  // The top left Y coordinates
,unsigned short X_W                   // active window's width
,unsigned short Y_H                   // active window's heigth
,unsigned long Foreground_color    
/*Foreground_color : The source (1bit map picture); map data 1 translate to Foreground color by color expansion*/
,const unsigned short *data           // Data buffer first address
);
		void BTE_Alpha_Blending
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
,unsigned char alpha       // The level of transparency(0~31);
);
		void LT768_PWM0_Init
(
 unsigned char on_off                       // 0��disable PWM0    1��enbale PWM0
,unsigned char Clock_Divided                // PWM clock  0~3(1,1/2,1/4,1/8);
,unsigned char Prescalar                    // clock division  1~256
,unsigned short Count_Buffer                // Setting the output cycle of the PWM
,unsigned short Compare_Buffer              // Set up duty ratio
);
		void LT768_PWM0_Duty(unsigned short Compare_Buffer);
		void LT768_PWM1_Init
(
 unsigned char on_off                       // 0��disable PWM0    1��enbale PWM0
,unsigned char Clock_Divided                // PWM clock  0~3(1,1/2,1/4,1/8);
,unsigned char Prescalar                    // clock division  1~256
,unsigned short Count_Buffer                // Setting the output cycle of the PWM
,unsigned short Compare_Buffer              // Set up duty ratio
);
		void LT768_PWM1_Duty(unsigned short Compare_Buffer);
		void LT768_Standby(void);
		void LT768_Wkup_Standby(void);
		void LT768_Suspend(void);
		void LT768_Wkup_Suspend(void);
		void LT768_SleepMode(void);
		void LT768_Wkup_Sleep(void);
};
extern LT768Lib LT768_Lib;
#endif
