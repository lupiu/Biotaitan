//--------------------------------------------------
/*
* optic.cpp
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/
#include "Arduino.h"
#include "ADS1115-Driver.h"
#include "optic.h"

//--------------------------------------------------
uint8_t g_OptLedIntens[4] = {OPT_LED1_INTENS, OPT_LED2_INTENS, OPT_LED3_INTENS, OPT_LED4_INTENS};
uint8_t g_OptLed[4] = {OPT_LED1, OPT_LED2, OPT_LED3, OPT_LED4};
uint8_t g_OptAds1115Mode[4] = {ADS1115_MUX_AIN0_GND, ADS1115_MUX_AIN1_GND, ADS1115_MUX_AIN2_GND, ADS1115_MUX_AIN3_GND};
ADS1115 g_OptAds1115 = ADS1115(ADS1115_I2C_ADDR_GND);

//--------------------------------------------------
void OPT_Led_OffAll(void)
{  
  uint8_t i;

  for (i = 0; i < 4; i++)
  {
    analogWrite(g_OptLed[i], 0);
  }
}

//--------------------------------------------------
int OPT_Led_On(uint8_t ch)
{
  int led_pwm;

  led_pwm = analogRead(g_OptLedIntens[ch]);  
  led_pwm = map(led_pwm, 0, 1023, 0, 255); 
  analogWrite(g_OptLed[ch], (led_pwm));
  return led_pwm;
}

//--------------------------------------------------
void OPT_Pd_Measure(uint8_t ch, int *adc_value)
{	
  g_OptAds1115.setMultiplexer(g_OptAds1115Mode[ch]);
	g_OptAds1115.startSingleConvertion();

	delayMicroseconds(25); // The ADS1115 needs to wake up from sleep mode and usually it takes 25 uS to do that

	while (g_OptAds1115.getOperationalStatus() == 0);

 *adc_value  = g_OptAds1115.readRawValue();
}

//--------------------------------------------------
void OPT_Test(uint8_t op_mode)
{
  uint16_t i, j;
  uint8_t ch;
  double start_time;
  int led_vr[2];
  int adc_value[4];

  for (i = 0; i < 60; i++)
  {
    if (op_mode == 0)
    {
      //ch = i % 4;
      ch = 0;
      OPT_Led_OffAll();
      led_vr[0] = OPT_Led_On(ch);
    }
    else
    {
      ch = i % 2;
      OPT_Led_OffAll();
      if (ch == 0)
      {
        led_vr[0] = OPT_Led_On(0);
        led_vr[1] = OPT_Led_On(2);
      }
      else
      {
        led_vr[0] = OPT_Led_On(1);
        led_vr[1] = OPT_Led_On(3);
      }
    }

    start_time = millis();
    while ((millis() - start_time) <= OPT_LIGHT_ON_TIME)
    {
      Serial.print(F(" T:")); Serial.print(millis()); Serial.print("\t");  
      Serial.print(F(" LED")); Serial.print(ch); Serial.print(F(": ")); Serial.print(led_vr[0]); Serial.print("\t");  
      if (op_mode == 1)
      {
        Serial.print(F(" LED")); Serial.print(ch + 1); Serial.print(F(": ")); Serial.print(led_vr[1]); Serial.print("\t");  
      }

      for (j = 0; j < 4; j++)
      {
        OPT_Pd_Measure(j, &adc_value[j]);
        Serial.print(F(" PD")); Serial.print(j); Serial.print(F(": ")); Serial.print(adc_value[j]); Serial.print("\t");
        Serial.flush();
      }
      Serial.println();
      Serial.flush();
      //delay(200);
    }
  }
  OPT_Led_OffAll();
}

//--------------------------------------------------
void OPT_Ctrl(void)
{
  uint16_t i, j;
  static uint8_t ch = 0;
  static double last_time = 0;
  static int led_vr[2];
  int adc_value[4];

    if ((millis() - last_time) >= OPT_LIGHT_ON_TIME)
    {
      ch = (ch + 1) % 4;
      OPT_Led_OffAll();
      led_vr[0] = OPT_Led_On(ch);
      last_time = millis();
    }
    Serial.print(F(" LED")); Serial.print(ch); Serial.print(F(": ")); Serial.print(led_vr[0]); Serial.print("\t");  

    for (j = 0; j < 4; j++)
    {
      OPT_Pd_Measure(j, &adc_value[j]);
      Serial.print(F(" PD")); Serial.print(j); Serial.print(F(": ")); Serial.print(adc_value[j]); Serial.print("\t");
      Serial.flush();
    }
    Serial.println();
    Serial.flush();
    //delay(200);
}

//--------------------------------------------------
void OPT_ADS1115_Initial(void)
{
  g_OptAds1115.reset();
  g_OptAds1115.setDeviceMode(ADS1115_MODE_SINGLE);
  g_OptAds1115.setDataRate(ADS1115_DR_250_SPS);
  g_OptAds1115.setPga(OPT_ADS1115_PGA);
}

//--------------------------------------------------
void OPT_Initial(void)
{
  OPT_ADS1115_Initial();

  pinMode(OPT_LED1, OUTPUT);
  analogWrite(OPT_LED1, 0);
  pinMode(OPT_LED2, OUTPUT);
  analogWrite(OPT_LED2, 0);
  pinMode(OPT_LED3, OUTPUT);
  analogWrite(OPT_LED3, 0);
  pinMode(OPT_LED4, OUTPUT);
  analogWrite(OPT_LED4, 0);

  pinMode(OPT_LED1_INTENS, INPUT);
  pinMode(OPT_LED2_INTENS, INPUT);
  pinMode(OPT_LED3_INTENS, INPUT);
  pinMode(OPT_LED4_INTENS, INPUT);
}

//--------------------------------------------------
