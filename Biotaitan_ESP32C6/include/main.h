//--------------------------------------------------
/*
* main.h
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/

#ifndef _MAIN_H__
#define _MAIN_H__

//--------------------------------------------------
#define BTN_START  9

#define I2C0_MASTER_SCL_IO  GPIO_NUM_7
#define I2C0_MASTER_SDA_IO  GPIO_NUM_6
#define I2C0_MASTER_NUM     I2C_0
#define I2C0_MASTER_FREQ_HZ 400000

#define I2C_MASTER_TX_BUF_DISABLE   0
#define I2C_MASTER_RX_BUF_DISABLE   0
#define I2C_MASTER_TIMEOUT_MS       1000

//--------------------------------------------------
typedef enum
{
  SYS_LED_SINGLE = 0,
  SYS_LED_DUAL,
  SYS_TEMP_MANUAL_H,
  SYS_TEMP_MANUAL_L,
  SYS_TEMP_AUTO,
}_SysOpmode;

//--------------------------------------------------

#endif
