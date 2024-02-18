#include "Arduino.h"
#include "main.h"
#include "temp_ctrl.h"
#include "lcm.h"
#include "optic.h"
#include "system.h"
#include "barcode.h"

//--------------------------------------------------


//--------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);

  LCM_Initial();
  TEMP_Initial();
  //OPT_Initial();
  BAR_Initial();
  SYS_Initial();
}

//--------------------------------------------------
void loop() {
  // put your main code here, to run repeatedly:
  SYS_SystemRun();
}
