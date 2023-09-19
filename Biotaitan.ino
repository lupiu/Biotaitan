#include "Arduino.h"
#include <Arduino_FreeRTOS.h>
#include "main.h"
#include "temp_ctrl.h"
#include "lcm.h"

//--------------------------------------------------


//--------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);

  LCM_Initial();
  TEMP_Initial();

  vTaskStartScheduler();
}

//--------------------------------------------------
void loop() {
  // put your main code here, to run repeatedly:

}
