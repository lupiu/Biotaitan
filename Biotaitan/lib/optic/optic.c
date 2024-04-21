//--------------------------------------------------
/*
* optic.cpp
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#include "optic.h"

//--------------------------------------------------
gpio_num_t g_OptLed[4] = {OPT_LED1, OPT_LED2, OPT_LED3, OPT_LED4};
ads1115_mux_t g_OptAds1115Mode[4] = {ADS1115_MUX_0_GND, ADS1115_MUX_1_GND, ADS1115_MUX_2_GND, ADS1115_MUX_3_GND};
ads1115_t g_OptAds1115;

//--------------------------------------------------
void OPT_Led_OffAll(void)
{
    uint8_t i;

    for (i = 0; i < 4; i++)
    {
      gpio_set_level(g_OptLed[i], 0);
    }
}

//--------------------------------------------------
void OPT_Led_On(uint8_t ch)
{
    gpio_set_level(g_OptLed[ch], 1);
}

//--------------------------------------------------
void OPT_Pd_Measure(uint8_t ch, int *adc_value)
{
    ads1115_set_mux(&g_OptAds1115, g_OptAds1115Mode[ch]);
    *adc_value  = ads1115_get_raw(&g_OptAds1115);
}

//--------------------------------------------------
void OPT_Test(uint8_t op_mode)
{
  uint16_t i, j;
  uint8_t ch;
  double start_time;
  int adc_value[4];

  for (i = 0; i < 600; i++)
  {
    if (op_mode == 0)
    {
      ch = i % 4;
//      ch = 0;
      OPT_Led_OffAll();
      OPT_Led_On(ch);
// Gaspard      led_vr[0] = OPT_Led_On(0);
    }
    else
    {
      ch = i % 2;
      OPT_Led_OffAll();
      if (ch == 0)
      {
        OPT_Led_On(0);
        OPT_Led_On(2);
      }
      else
      {
        OPT_Led_On(1);
        OPT_Led_On(3);
      }
    }

    start_time = esp_timer_get_time();
    while ((esp_timer_get_time() - start_time) <= OPT_LIGHT_ON_TIME)
    {
      printf("T:%lld\tLED%d:\t", esp_timer_get_time(), ch);
      if (op_mode == 1)
      {
        printf("LED%d:\t", ch + 1);
      }

      for (j = 0; j < 4; j++)
      {
        OPT_Pd_Measure(j, &adc_value[j]);
        printf("PD%d:%d\t", j, adc_value[j]);
      }
      printf("\n");
      //delay(200);
    }
  }
  OPT_Led_OffAll();
}

//--------------------------------------------------
void OPT_Ctrl(void)
{
    uint16_t i;
    static uint8_t ch = 0;
    static double last_time = 0;
    int adc_value[4];

    if ((esp_timer_get_time() - last_time) >= OPT_LIGHT_ON_TIME)
    {
      ch = (ch + 1) % 4;
      OPT_Led_OffAll();
      OPT_Led_On(ch);
      last_time = esp_timer_get_time();
    }
    printf("LED%d:\t", ch);

    for (i = 0; i < 4; i++)
    {
      OPT_Pd_Measure(i, &adc_value[i]);
      printf(" PD%d:%d\t", i, adc_value[i]);
    }
    printf("\n");
    //delay(200);
}

//--------------------------------------------------
void OPT_ADS1115_Initial(void)
{
    i2c_device_config_t dev_cfg = {
      .dev_addr_length = I2C_ADDR_BIT_LEN_7,
      .device_address = ADS1115_I2C_ADDR_GND,
      .scl_speed_hz = I2C0_MASTER_FREQ_HZ,
    };
    i2c_master_dev_handle_t dev_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(I2C_GetHandle(), &dev_cfg, &dev_handle));

    ads1115_set_mode(&g_OptAds1115, ADS1115_MODE_SINGLE);
    ads1115_set_sps(&g_OptAds1115, ADS1115_SPS_250);
    ads1115_set_pga(&g_OptAds1115, OPT_ADS1115_PGA);
}

//--------------------------------------------------
void OPT_Initial(void)
{
  OPT_ADS1115_Initial();

  gpio_set_direction(OPT_LED1, GPIO_MODE_OUTPUT);
  gpio_set_level(OPT_LED1, 0);
  gpio_set_direction(OPT_LED2, GPIO_MODE_OUTPUT);
  gpio_set_level(OPT_LED2, 0);
  gpio_set_direction(OPT_LED3, GPIO_MODE_OUTPUT);
  gpio_set_level(OPT_LED3, 0);
  gpio_set_direction(OPT_LED4, GPIO_MODE_OUTPUT);
  gpio_set_level(OPT_LED4, 0);
}

//--------------------------------------------------
