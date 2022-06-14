#include "fan.h"
#include "main.h"
#include "tim.h"

void FAN_CCW_RUN(void)
{
    FAN_CW_SetLow();
    FAN_CCW_SetHigh();
  
}

void FAN_Stop(void)
{
    FAN_CCW_SetLow(); //brake
    FAN_CW_SetLow(); //brake
}


void AI_Function_OnOff(uint8_t sel)
{
   if(sel==0){ //On 
    FAN_CCW_RUN();
    PLASMA_SetHigh(); //
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
    PTC_SetHigh();

   }
   else{ //off 
    FAN_Stop();
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
    PTC_SetLow();
    }
}

