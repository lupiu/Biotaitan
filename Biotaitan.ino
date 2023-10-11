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
  
  Serial.begin(115200);

  SYS_SetOpmode(SYS_INIT);

  //LCM_Initial();
  //TEMP_Initial();
  //OPT_Initial();
  //SYS_Initial();
}

//--------------------------------------------------
void loop() {
  // put your main code here, to run repeatedly:
  //SYS_SystemRun();
  TEMP_TempCtrl(50, NTC_TS1);
  delay(1000);
}
