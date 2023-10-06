#include "Arduino.h"
#include "ADS1115-Driver.h"

//===================//
uint8_t g_op_mode;
uint8_t g_LedIntensityPin[4] = {A0, A1, A2, A3};
uint8_t g_LedPinDefine[4] = {5, 6, 10, 11};
uint8_t g_Ads1115_Pin[4] = {ADS1115_MUX_AIN0_GND, ADS1115_MUX_AIN1_GND, ADS1115_MUX_AIN2_GND, ADS1115_MUX_AIN3_GND};

#define LIGHT_ON_TIME  1000

ADS1115 ads1115 = ADS1115(ADS1115_I2C_ADDR_GND);

float readValue(uint8_t input) {
	ads1115.setMultiplexer(input);
	ads1115.startSingleConvertion();

	delayMicroseconds(25); // The ADS1115 needs to wake up from sleep mode and usually it takes 25 uS to do that

	while (ads1115.getOperationalStatus() == 0);

  int temp = ads1115.readRawValue();
  
	return (0 - temp);
}

void setup() {
	Serial.begin(115200);
 
	ads1115.reset();
	ads1115.setDeviceMode(ADS1115_MODE_SINGLE);
	ads1115.setDataRate(ADS1115_DR_250_SPS);
	ads1115.setPga(ADS1115_PGA_0_512);

  pinMode(5, OUTPUT);
  analogWrite(5, 255);
  pinMode(6, OUTPUT);
  analogWrite(6, 255);
  pinMode(10, OUTPUT);
  analogWrite(10, 255);
  pinMode(11, OUTPUT);
  analogWrite(11, 255);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(3, INPUT_PULLUP);
  g_op_mode = digitalRead(3);
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

  //while(1);
}

void Led_OffAll(void)
{  
  uint8_t i;

  for (i = 0; i < 4; i++)
  {
    analogWrite(g_LedPinDefine[i], 255);
  }
}

int Led_On(uint8_t ch)
{
  int led_vr;

  led_vr = analogRead(g_LedIntensityPin[ch]);  
  led_vr = map(led_vr, 0, 1023, 0, 255); 
  analogWrite(g_LedPinDefine[ch], (255 - led_vr));
  return led_vr;
}

void Pd_Measure(uint8_t ch, int *adc_value)
{
  *adc_value = readValue(g_Ads1115_Pin[ch]);
}
