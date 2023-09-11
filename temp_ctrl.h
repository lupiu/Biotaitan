//--------------------------------------------------
 /*
 * temp_ctrl.h
 *
 *  Created on: 2023/09/06
 *      Author: silva_lin
 */

 #ifndef _TEMP_CTRL_H__
 #define _TEMP_CTRL_H__

 #include "Arduino.h"

//--------------------------------------------------
#define NTC_0 0
#define HEATER_CTRL 1
#define PELTIER_CTRL 2
#define FAN_CTRL 3

#define PID_KP 30  // Proportional constant
#define PID_KI 0.7 // Integral Constant
#define PID_KD 200 // Derivative Constant

//--------------------------------------------------
typedef enum
{
    IDLE  = 0,
    MEASURE,
    PIDING,
}_TempCtrlStatus;

typedef struct
{
  _TempCtrlStatus status;
  double Temp_C;
}_TempData;

typedef struct
{
  double SetPoint;
  double Input;
  double Output;
}_PidData;

typedef struct
{
  _TempCtrlStatus status;
  double Temp_C;
}_TempCtrl;

//--------------------------------------------------
void TEMP_Initial(void);
//--------------------------------------------------

#endif