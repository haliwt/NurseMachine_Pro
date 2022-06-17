#include "fan.h"
#include "main.h"
#include "tim.h"
#include "run.h"

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


void AI_Function_OnOff(void)
{
   if(run_t.gAi==0){ //On 
    FAN_CCW_RUN();
    PLASMA_SetHigh(); //
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
    PTC_SetHigh();

   }
   else{ //off 
    FAN_Stop();
	PLASMA_SetLow(); //
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
    PTC_SetLow();
    }
}

void Buzzer_On(void)
{
  uint8_t i;
  for(i=0;i<30;i++){//
        BUZZER_SetHigh();
        HAL_Delay(1);
        BUZZER_SetLow() ;
        HAL_Delay(1);
  }

}

