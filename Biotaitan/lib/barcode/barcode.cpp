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
static const int BCR_RX_BUF = 1024;
uint8_t g_bcr_enable = 0;

//--------------------------------------------------
void BCR_Enable(void)
{
    g_bcr_enable = 1;
}

//--------------------------------------------------
void BCR_Disable(void)
{
    g_bcr_enable = 0;
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
        .baud_rate = 115200,
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
void BCR_Task(void * pvParametersoid)
{
    static const char *RX_TASK_TAG = "RX_TASK";

    while(1)
    {
        //if (g_bcr_enable)
        if (1)
        {
            digitalWrite(BCR_TRIG, 0);
            delay(20);
            digitalWrite(BCR_TRIG, 1);

            uint8_t* data = (uint8_t*) malloc(BCR_RX_BUF + 1);
            const int rxBytes = uart_read_bytes(UART_NUM_1, data, BCR_RX_BUF, 200 / portTICK_PERIOD_MS);
            Serial.println(rxBytes);
            if (rxBytes > 0) {
                data[rxBytes] = 0;
                Serial.print(F("Read"));Serial.print(rxBytes);Serial.print(F("bytes:"));Serial.println(data[0]);
                ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
            }
            free(data);
        }
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
