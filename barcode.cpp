//--------------------------------------------------
/*
* barcode.cpp
*
*  Created on: 2023/09/14
*      Author: silva_lin
*/

#include "Arduino.h"
#include "barcode.h"

//--------------------------------------------------
uint8_t g_Barcoede_status = 0;
//--------------------------------------------------
void BAR_AutoOn(void)
{
  static double time = 0;
  static bool blink = 0;

  if (g_Barcoede_status == 0)
  {
     digitalWrite(BAR_TRIG, BAR_OFF);
  }
  else
  {
    if (blink == true)
    {
      if (millis() - time > 400)
      {
        digitalWrite(BAR_TRIG, BAR_OFF);
        blink = false;
        time = millis();
      }
    }
    else
    {
      if (millis() - time > 400)
      {
        digitalWrite(BAR_TRIG, BAR_ON);
        blink = true;
        time = millis();
      }
    }
  }

}

//--------------------------------------------------
void BAR_TrigOn(void)
{
  g_Barcoede_status = 1;
}

//--------------------------------------------------
void BAR_TrigOff(void)
{
  g_Barcoede_status = 0;
}

//--------------------------------------------------
int BAR_Read(char *id)
{  
  char tmp = '0';
  char barcode[BAR_LENGTH] = {0};
  int i = 0;
  double start_time;
  int j = 0;
  
  if (Serial3.available() > 0)
  {
    start_time =  millis();
    while(1)
    {
      if (Serial3.available() > 0)
      {
        barcode[i] = Serial3.read();
        if (i > 0)
        {
          if (barcode[i] == 0x0A && barcode[i - 1] == 0x0D)
          {
            memcpy(id, barcode, i - 1);
            for (j = 0; j < (i - 1); j++)
            {
              Serial.print(barcode[j]);
            }
            Serial.println("");
            return BAR_OK;
          }
        }
        i++;
      }

      if (millis() - start_time > BAR_TIMEOUT)
      {
        Serial.println("Barcode reader timeout!!");
        return BAR_ERROR;
      }
    }
  }
  return BAR_ERROR;
}

//--------------------------------------------------
void BAR_Initial(void)
{
  pinMode(BAR_TRIG, OUTPUT);
  digitalWrite(BAR_TRIG, BAR_OFF);

  Serial3.begin(9600);
}
//--------------------------------------------------