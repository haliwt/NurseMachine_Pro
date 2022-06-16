#include "run.h"
#include "key.h"
#include "smg.h"
#include "dht11.h"
#include "fan.h"
#include "tim.h"

 



DHT11_Data_TypeDef DHT11;
uint8_t ReceiveBuffer[1];

static CProcess1 cprocess;
RUN_T run_t; 
#if 0
 n = 0;
   CParser1Init(&cparser); 
   while ((ch = fgetc(f)) != EOF) {
       int sig;
       switch (ch) {
       case '/': sig = SLASH_SIG; break;
       case '*': sig = STAR_SIG;  break;
       default:  sig = CHAR_SIG;  break;
       }
       CParser1Dispatch(&cparser, sig); 
       ++n; 
   }
#endif  

void CProcessRun_Init(void)
{
   run_t.gSig = KEY_SIG ;
  
   CProcessInit(&cprocess);

}


 

    

/**********************************************************************
*
*Functin Name: void RunCommand_Mode(unit8_t sig)
*Function : be check key of value 
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void RunCommand_Mode(uint8_t sig)
{
     static uint8_t powerflag ,beepflag=0xff,keyMode;
	 static uint8_t fanflag,plasmaflag,dryflag,aiflag;

    if(sig!=0){
		 if(beepflag !=run_t.gKeyValue){
		 	beepflag = run_t.gKeyValue ;
            Buzzer_On();
			
		 }
     }
       switch(sig){

	    case 0:

	         if(run_t.gPower_On !=1){
                  
                  run_t.gRun_flag= IDEL_SIG ;


			 }

	    break;
        case 0x80: //CIN0 -> POWER KEY 
             powerflag = powerflag ^ 0x01;
             if(powerflag == 1){
			 	run_t.gRun_flag= RUN_SIG ;
			 	run_t.gPower_On =1;
             }
             else{
                 Smg_AllOff();
				 run_t.gRun_flag= IDEL_SIG ;
			 	run_t.gPower_On =0;
				run_t.gPower_On =0;
            
             }
         break;
         
         case 0x40: //CIN1 -> MODE KEY
             if(run_t.gPower_On ==1){
			 	 
              if(keyMode ==0){
			  	  keyMode ++;
			      run_t.gTimer_key_2s=0;
              }
           
	           
           if(run_t.gTimer_key_2s >1){  //Mode key be pressed long 
               run_t.gKeyLong =1;
              // run_t.gTimer_5s_start =1; //timer is 5s start be pressed key 
                 run_t.gTimer_key_5s=0;
                run_t.gKeyValue++ ;
			   keyMode=0;
			   run_t.gTimer_key_2s=0;
			   run_t.gRun_flag= RUN_SIG ;
           } 
           else if(run_t.gKeyLong ==0 && sig != 0x40){ //shot be pressed 
                

			   run_t.gKey_long_flag=1;
		      
			   keyMode=0;
               run_t.gKeyValue++ ;
			   run_t.gRun_flag= RUN_SIG ;
           }

		break;
         
         case 0x20: //CIN2 ->ADD KEY
             if(run_t.gPower_On ==1){

					 keyMode=0;
				 if( run_t.gKeyLong ==1){

					 run_t.gTimer_key_5s=0;// run_t.gTimer_5s_start =0; //timer is 5s start be pressed key 
					
					run_t.gTimes_hours++;
				    if(run_t.gTimes_hours >24){
						run_t.gTimes_hours=0;
					}
                    run_t.gTimer_key_5s=0;//run_t.gTimer_5s_start =1; //timer is 5s start be pressed key 
				 }
				 else{


				     if(run_t.gSig==0){
						run_t.gSig ++;
						run_t.gTimes_hours_temp =12;

					}

					  run_t.gTimes_hours_temp++;
					  if(run_t.gTimes_hours_temp >24)
					  	 run_t.gTimes_hours_temp=0;
				 }
			 
			 	 run_t.gRun_flag= RUN_SIG ;
             
             	}
            
             
         break;
         
         case 0x10: //CIN3 -> DEC KEY
             if(run_t.gPower_On ==1){
			 	 keyMode=0;
			 	  if( run_t.gKeyLong ==1){
                      run_t.gTimer_key_5s=0;//run_t.gTimer_5s_start =0; //timer is 5s start be pressed key 
              
					run_t.gTimes_hours--;
				    if(run_t.gTimes_hours <0){
						run_t.gTimes_hours=24;
					}
					 run_t.gTimer_key_5s=0;//run_t.gTimer_5s_start =1; //timer is 5s start be pressed key 

				 }
				 else{

                     if(run_t.gSig==0){
						run_t.gSig ++;
						run_t.gTimes_hours_temp =12;

					}

					  run_t.gTimes_hours_temp--;
					  if(run_t.gTimes_hours_temp <0)
					  	 run_t.gTimes_hours_temp=24;
				 }
			 
			 	 run_t.gRun_flag= RUN_SIG ;
             
             }
           
             
         break;
         
         case 0x08: //CIN4 -> FAN KEY 
               if(run_t.gPower_On ==1){
                  keyMode=0;
			     fanflag = fanflag ^ 0x01;
				 if(fanflag == 1){
                     run_t.gFan =1; //turn off fan 
				 }
				 else
				 	run_t.gFan =0; // turn on fan
			   	 run_t.gRun_flag= RUN_SIG ;
                
               
             
             }
            
         break;
         
         case 0x04: //CIN5  -> STERILIZATION KEY 
             if(run_t.gPower_On ==1){
				keyMode=0;

			    plasmaflag = plasmaflag ^ 0x01;
				if(plasmaflag ==1){
                    run_t.gPlasma =1;
				}
				else
					run_t.gPlasma =0;
                 
                run_t.gRun_flag= RUN_SIG ;
             
             }
            
            
         break;
         
         case 0x02: //CIN6  ->DRY KEY 
               if(run_t.gPower_On ==1){
				   keyMode=0;

			       dryflag = dryflag ^ 0x01;
				   if(dryflag == 1){
						run_t.gDry =1; //turn off 
				   }
				   else
				   	   run_t.gDry =0; //turn on
                 
                    run_t.gRun_flag= RUN_SIG ;
             }
           
             
         break;
         
         case 0x01: //CIN7 -> AI KEY
             if(run_t.gPower_On ==1){

			      keyMode=0;
				  aiflag = aiflag ^ 0x01;
				   if(aiflag ==1){
 						run_t.gAi=1;
				   }
                   else
                      run_t.gAi=0;
                   run_t.gRun_flag= RUN_SIG ;
             
             }
          
         break;
             
         default:
             
         break;
       
       } //switch(ReceiveBuffer) --end 
      
    }
	
  
}

/**********************************************************************
*
*Functin Name: void RunCommand_Mode(unit8_t sig)
*Function : be check key of value 
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void RunCommand_Order(void)
{
   static uint8_t po,m,n;
	switch(run_t.gRun_flag){

      case IDEL_SIG :
	  	 if(po==0 ){
			 	po++;
			 	Smg_AllOff();
	  	 	}
         
           Breath_Led();
		   run_t.gKeyLong = 0;
          run_t.gKeyValue++;


	  break;

	  case  RUN_SIG: //1
         run_t.gKeyLongPressed=0;
		 run_t.gKeyValue++;
		
		if(run_t.gTimer_500ms ==1){
		
			run_t.gTimer_500ms = 0;
			KeyLed_Power_On();
			Display_Function_OnOff();

		if(run_t.gFan==0){ //0 -ON
			FAN_CCW_RUN();
		
		}
		else{ //1-OFF
			
			PTC_SetLow(); //PTC turn off 
			FAN_Stop();
		
		}

		if(run_t.gPlasma==0){ //0 -ON
			PLASMA_SetHigh();
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
		}
		else{ //1 -OFF
			PLASMA_SetLow();
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic off
		}


		if(run_t.gDry==0){
		   FAN_CCW_RUN();
		   PTC_SetHigh();
			
		}
	   else{
			PTC_SetLow();
		}
		
		if(run_t.gAi==0){//ON
			
			AI_Function_OnOff(0);
		}
		else{ //Off
		
			AI_Function_OnOff(1);
		}
		//open PTC and FAN ,Ultrasonic 


		}

		if( run_t.gTimer_300ms==1){

			run_t.gTimer_300ms=0;
			if(run_t.gKeyLong ==1){
				

				m = (run_t.gTimes_hours /10) %10;
				n=  (run_t.gTimes_hours %10);
				TM1640_Write_4Bit_Data(0,0,m,n,1) ; //timer is default 12 hours "H0:12"
				if(run_t.gTimer_400ms==1){
					run_t.gTimer_400ms=0;

					 TM1640_Write_4Bit_Data(0,0,m,n,2) ; //timer is default 12 hours "H0:12"

				}
				TM1640_Write_4Bit_Data(0,0,m,n,1) ; 
				

		     }
		   else{ 
				run_t.gTimer_300ms=0;


			   if(run_t.gKey_long_flag==1){
					 run_t.gKey_long_flag=0;

                     m = (run_t.gTimes_hours /10) %10;
				     n=  (run_t.gTimes_hours %10);
				     TM1640_Write_4Bit_Data(0,0,m,n,1) ; //timer is default 12 hours "H0:12"

               }
			   else{
				if(run_t.gSig==0){
					run_t.gSig ++;
					run_t.gTimes_hours_temp =12;

				}
	            m = (run_t.gTimes_hours_temp /10) %10;
				n=  (run_t.gTimes_hours_temp %10);
				TM1640_Write_4Bit_Data(m,n,0,0,0) ; //timer is default 12 hours "12:00"

				}
			  }

		  }
		
						 

		if(run_t.gTimer_key_2s==1){//1s read one data
			run_t.gTimer_1s =0;
			Display_DHT11_Value(&DHT11);
		}
		if(run_t.gTimer_key_5s >10){
              run_t.gTimer_key_5s=11;
              run_t.gKeyLong =0;
					 
		}

		

	  break;

	  

	 }





}


