#include <Arduino_FreeRTOS.h>
#include "main.h"

void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );
void Taskprint( void *pvParameters );

//--------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  xTaskCreate(TaskSPI1,"Task1",128,NULL,1,NULL);
  xTaskCreate(TaskADC1,"Task2 ",128,NULL,1,NULL);
  vTaskStartScheduler();
}

//--------------------------------------------------
void TaskSPI1(void *pvParameters) 
{
pinMode(8, OUTPUT);
while(1)
{
digitalWrite(8, HIGH);   
vTaskDelay( 200 / portTICK_PERIOD_MS ); 
digitalWrite(8, LOW);    
vTaskDelay( 200 / portTICK_PERIOD_MS ); 
}
}

//--------------------------------------------------
void TaskADC1(void *pvParameters) 
{
pinMode(8, OUTPUT);
while(1)
{
digitalWrite(8, HIGH);   
vTaskDelay( 200 / portTICK_PERIOD_MS ); 
digitalWrite(8, LOW);    
vTaskDelay( 200 / portTICK_PERIOD_MS ); 
}
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
