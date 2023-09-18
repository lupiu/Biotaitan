#include "Arduino.h"
#include <Arduino_FreeRTOS.h>
#include "main.h"
#include "temp_ctrl.h"

//--------------------------------------------------


//--------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);

  TEMP_Initial();

  vTaskStartScheduler();
}

//--------------------------------------------------
void Taskprint(void *pvParameters)  
{
  int counter = 0;
  while(1)
  {
    counter++;
    Serial.println(counter);
    vTaskDelay( 500 / portTICK_PERIOD_MS ); 
  }
}

//--------------------------------------------------
void loop() {
  // put your main code here, to run repeatedly:

}
