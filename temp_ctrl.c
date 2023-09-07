//--------------------------------------------------
 /*
 * temp_ctrl.c
 *
 *  Created on: 2023/09/06
 *      Author: silva_lin
 */

 #include "Arduino.h" 
 #include "main.h"

//--------------------------------------------------
u16 TEMP_ReadTemperature(u8 pin)
{
  u16 temp;
  
  temp = analogRead(pin);
}


//--------------------------------------------------
