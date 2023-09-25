#include "Arduino.h"
#include <Arduino_FreeRTOS.h>
#include "main.h"
#include "temp_ctrl.h"
#include "lcm.h"
#include "optic.h"
#include "system.h"

//--------------------------------------------------


//--------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);

  SYS_SetOpmode(SYSTEM_INIT);

  LCM_Initial();
  TEMP_Initial();
  OPT_Initial();

  vTaskStartScheduler();
  SYS_Initial();
}

//--------------------------------------------------
void loop() {
  // put your main code here, to run repeatedly:

}
