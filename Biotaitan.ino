#include "Arduino.h"
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
  //TEMP_Initial();
  //OPT_Initial();
  //SYS_Initial();
  SYS_SetOpmode(LCM_TOP);
  LCM_DisplayGrid();
  LCM_DisplayKeyBoard();
  LCM_DisplayTop();
}

//--------------------------------------------------
void loop() {
  // put your main code here, to run repeatedly:
  LCM_TouchScan();
}
