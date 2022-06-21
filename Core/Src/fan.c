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

        if(run_t.gPlasma==0){ //0 -ON
				PLASMA_SetHigh();
				HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
		}
		else{ //1 -OFF
			     run_t.gAi_Led =1;
				 run_t.gAi_fun =1; //tunr off AI function 
				PLASMA_SetLow();
				HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic off
				
		}

           //FAN and PTC 
		if(run_t.gFan==0){ //0 -ON
		      
				FAN_CCW_RUN();
		}
		else{ //1-OFF
				
				run_t.gAi_Led =1;
				run_t.gAi_fun =1;
			  
			  PTC_SetLow(); //PTC turn off 
			  FAN_Stop();

		}
				
         if(run_t.gDry==0){ //dry is works this fan must be works
			  
                FAN_CCW_RUN();
				PTC_SetHigh();
		}
		 else{

		        run_t.gAi_Led =1;
				run_t.gAi_fun =1;
				PTC_SetLow();
		}


    //adjust AI function On or Off
    if(run_t.gAi_fun ==0){


	       if(run_t.gAi_Led ==1 && run_t.gDry ==1 && run_t.gPlasma==1 &&run_t.gFan==1){

		      run_t.gAi=0; //Turn On AI


		   }
       

	   if(run_t.gAi==0){ //On 
	    FAN_CCW_RUN();
		PLASMA_SetHigh(); //
	    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	    PTC_SetHigh();

	   }
	   else{ //off 
	   
		PLASMA_SetLow(); //
	    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	    PTC_SetLow();
		FAN_Stop();
	    }
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

void ShutDown_AllFunction(void)
{
	
	PLASMA_SetLow(); //
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	PTC_SetLow();
	FAN_Stop();



}

