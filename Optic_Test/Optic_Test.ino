#include "Arduino.h"
#include "ADS1115-Driver.h"

//===================//
uint8_t g_op_mode;
uint8_t g_LedIntensityPin[4] = {A0, A1, A2, A3};
uint8_t g_LedPinDefine[4] = {5, 4, 3, 2};
uint8_t g_Ads1115_Pin[4] = {ADS1115_MUX_AIN0_GND, ADS1115_MUX_AIN1_GND, ADS1115_MUX_AIN2_GND, ADS1115_MUX_AIN3_GND};

#define ADS1115_PGA ADS1115_PGA_1_024
//ADS1115_PGA_6_144
//ADS1115_PGA_4_096
//ADS1115_PGA_2_048
//ADS1115_PGA_1_024
//ADS1115_PGA_0_512
//ADS1115_PGA_0_256

#define LIGHT_ON_TIME  1000

ADS1115 ads1115 = ADS1115(ADS1115_I2C_ADDR_GND);

float readValue(uint8_t input) {
	ads1115.setMultiplexer(input);
	ads1115.startSingleConvertion();

	delayMicroseconds(25); // The ADS1115 needs to wake up from sleep mode and usually it takes 25 uS to do that

	while (ads1115.getOperationalStatus() == 0);

  int temp = ads1115.readRawValue();
  
	return (temp);
}

void setup() {
	Serial.begin(115200);
 
	ads1115.reset();
	ads1115.setDeviceMode(ADS1115_MODE_SINGLE);
	ads1115.setDataRate(ADS1115_DR_250_SPS);
	ads1115.setPga(ADS1115_PGA);

  pinMode(g_LedPinDefine[0], OUTPUT);
  analogWrite(g_LedPinDefine[0], 0);
  pinMode(g_LedPinDefine[1], OUTPUT);
  analogWrite(g_LedPinDefine[1], 0);
  pinMode(g_LedPinDefine[2], OUTPUT);
  analogWrite(g_LedPinDefine[2], 0);
  pinMode(g_LedPinDefine[3], OUTPUT);
  analogWrite(g_LedPinDefine[3], 0);

  //pinMode(2, OUTPUT);
  //digitalWrite(2, LOW);
  //pinMode(3, INPUT_PULLUP);
  //g_op_mode = digitalRead(3);
  g_op_mode = 0;
}


void loop()
{
  uint16_t i, j;
  uint8_t ch;
  double start_time;
  int led_vr[2];
  int adc_value[4];

  for (i = 0; i < 60; i++)
  {
    if (g_op_mode == 0)
    {
      ch = i % 4;
      Led_OffAll();
      led_vr[0] = Led_On(ch);
    }
    else
    {
      ch = i % 2;
      Led_OffAll();
      if (ch == 0)
      {
        led_vr[0] = Led_On(0);
        led_vr[1] = Led_On(2);
      }
      else
      {
        led_vr[0] = Led_On(1);
        led_vr[1] = Led_On(3);
      }
    }

    start_time = millis();
    while ((millis() - start_time) <= LIGHT_ON_TIME)
    {
      Serial.print(F(" VR")); Serial.print(ch); Serial.print(F(": ")); Serial.print(led_vr[0]); Serial.print("\t");  
      if (g_op_mode == 1)
      {
        Serial.print(F(" VR")); Serial.print(ch + 1); Serial.print(F(": ")); Serial.print(led_vr[1]); Serial.print("\t");  
      }

      for (j = 0; j < 4; j++)
      {
        Pd_Measure(j, &adc_value[j]);
        Serial.print(F(" IN")); Serial.print(j); Serial.print(F(": ")); Serial.print(adc_value[j]); Serial.print("\t");
        Serial.flush();
      }
      Serial.println();
      Serial.flush();
      delay(200);
    }
  }
  
  Led_OffAll();
  while(1);
}

void Led_OffAll(void)
{  
  uint8_t i;

  for (i = 0; i < 4; i++)
  {
    analogWrite(g_LedPinDefine[i], 0);
  }
}

int Led_On(uint8_t ch)
{
  int led_vr;

  //led_vr = analogRead(g_LedIntensityPin[ch]);  
  //led_vr = map(led_vr, 0, 1023, 0, 255); 
  led_vr = 128;
  analogWrite(g_LedPinDefine[ch], led_vr);
  return led_vr;
}

void Pd_Measure(uint8_t ch, int *adc_value)
{
  *adc_value = readValue(g_Ads1115_Pin[ch]);
}
