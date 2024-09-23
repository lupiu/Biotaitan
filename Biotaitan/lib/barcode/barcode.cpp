//--------------------------------------------------
/*
* barcode.cpp
*
*  Created on: 2023/10/27
*      Author: silva_lin
*/
#include "Arduino.h"
#include "barcode.h"

//--------------------------------------------------
_BcrHandle g_BcrHDL = {0};

//--------------------------------------------------
void BCR_Enable(void)
{
    g_BcrHDL.enable = 1;
    g_BcrHDL.datavalid = 0;
    g_BcrHDL.rxbytes = 0;
    memset(g_BcrHDL.data, 0, BCR_RX_BUF);
}

//--------------------------------------------------
void BCR_Disable(void)
{
    g_BcrHDL.enable = 0;
}

//--------------------------------------------------
void BCR_GetStatus(uint8_t *enalbe, uint8_t *valid, uint16_t *rxbytes)
{
    *enalbe = g_BcrHDL.enable;
    *valid = g_BcrHDL.datavalid;
    *rxbytes = g_BcrHDL.rxbytes;
}

//--------------------------------------------------
void BCR_GetData(char *data, uint16_t size)
{
    uint16_t i;

    for (i = 0; i < size; i++)
    {
        data[i] = g_BcrHDL.data[i];
    }
    g_BcrHDL.datavalid = 0;
    g_BcrHDL.rxbytes = 0;
    memset(g_BcrHDL.data, 0, BCR_RX_BUF);
}

//--------------------------------------------------
void BCR_Beep(int ms)
{
    analogWrite(BCR_OUT, 192);
    delay(ms);
    analogWrite(BCR_OUT, 0);
}

//--------------------------------------------------
void UART_Initial(void)
{
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, BCR_RX_BUF * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, BCR_TX, BCR_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

//--------------------------------------------------
void BCR_Setting(void)
{
    while(1)
    {
        digitalWrite(BCR_TRIG, 0);
        delay(50);
        digitalWrite(BCR_TRIG, 1);

        g_BcrHDL.rxbytes = uart_read_bytes(UART_NUM_1, g_BcrHDL.data, BCR_RX_BUF, 50 / portTICK_PERIOD_MS);
        if (g_BcrHDL.rxbytes > 0)
        {
            BCR_Beep(20);
        }
        delay(500);
    }
}

//--------------------------------------------------
void BCR_Task(void * pvParametersoid)
{
    uint8_t i;

    while(1)
    {
        if (g_BcrHDL.enable)
        {
            digitalWrite(BCR_TRIG, 0);
            delay(50);
            digitalWrite(BCR_TRIG, 1);

            g_BcrHDL.rxbytes = uart_read_bytes(UART_NUM_1, g_BcrHDL.data, BCR_RX_BUF, 50 / portTICK_PERIOD_MS);
            if (g_BcrHDL.rxbytes > 0)
            {
                BCR_Disable();
                g_BcrHDL.datavalid = 1;
                BCR_Beep(20);
                g_BcrHDL.data[g_BcrHDL.rxbytes] = 0;
                /*
                Serial.print(F("Read "));Serial.print(g_BcrHDL.rxbytes);Serial.println(F(" bytes:"));
                for (i = 0; i < g_BcrHDL.rxbytes; i++)
                {
                    Serial.println(g_BcrHDL.data[i]);
                }
                */
            }
        }
        delay(100);
    }
}

//--------------------------------------------------
void BCR_Initial(void)
{
  UART_Initial();

  pinMode(BCR_BEEP, INPUT);
  pinMode(BCR_TRIG, OUTPUT);
  digitalWrite(BCR_TRIG, 1);
  pinMode(BCR_OUT, OUTPUT);
  analogWrite(BCR_OUT, 0);

  xTaskCreatePinnedToCore((TaskFunction_t)BCR_Task, "BCR_Task", 4096, NULL, 0, NULL, tskNO_AFFINITY);
}

//--------------------------------------------------
