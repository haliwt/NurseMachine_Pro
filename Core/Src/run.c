#include "run.h"
#include "key.h"
#include "smg.h"
#include "dht11.h"
#include "fan.h"
#include "tim.h"

 

//static void CProcessDispatch(CProcess1 *me, uint8_t sig) ;
static void SteupTimes_Handle(void);

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


 
void CProcess_Run(void)
{
    static uint8_t powerflag ,beepflag=0xff;
   // run_t.gKeyValue = I2C_SimpleRead_From_Device(ReceiveBuffer) ;
    if(ReceiveBuffer[0] !=0 && ReceiveBuffer[0] !=0xff){
		 if(beepflag !=run_t.gKeyValue){
		 	beepflag = run_t.gKeyValue ;
            Buzzer_On();
			
		 }
       switch(ReceiveBuffer[0]){
         case 0x80: //CIN0 -> POWER KEY 
             powerflag = powerflag ^ 0x01;
             if(powerflag == 1){
			 	run_t.gPower_On =1;
                run_t.gSig = POWER_SIG ;
                cprocess.state__ =  CODE ;
                cprocess.cmdCtr__ = 1;
             }
             else{
                 Smg_AllOff();
				 run_t.gPower_On =0;
                 run_t.gSig = KEY_SIG ;
                 cprocess.state__=IDLE; //state 
                 run_t.gSig =KEY_SIG ;
             }
         break;
         
         case 0x40: //CIN1 -> MODE KEY
             if(run_t.gPower_On ==1){
                
				 
				 run_t.gAdd_flag=0;
                run_t.gDec_flag=0; // //input setup times handle   
				 run_t.gSig = MODE_SIG;
                 cprocess.state__ =  RUN ;
                 cprocess.cmdCtr__ = 3;
                
             }
          
         break;
         
         case 0x20: //CIN2 ->ADD KEY
             if(run_t.gPower_On ==1){
                  run_t.gSig = ADD_SIG;
                  cprocess.state__ =  RUN ;
                  cprocess.cmdCtr__ = 5;
             
             }
            
             
         break;
         
         case 0x10: //CIN3 -> DEC KEY
             if(run_t.gPower_On ==1){
                  run_t.gSig = DEC_SIG;
                  cprocess.state__ =  RUN ;
                 cprocess.cmdCtr__ = 7;
             
             }
           
             
         break;
         
         case 0x08: //CIN4 -> FAN KEY 
               if(run_t.gPower_On ==1){
                  run_t.gSig = FAN_SIG;
                   cprocess.state__ =  RUN ;
                   cprocess.cmdCtr__ = 11;
             
             }
            
         break;
         
         case 0x04: //CIN5  -> STERILIZATION KEY 
             if(run_t.gPower_On ==1){
                  run_t.gSig = PLASMA_SIG;
                  cprocess.state__ =  RUN ;
                   cprocess.cmdCtr__ = 13;
             
             }
            
            
         break;
         
         case 0x02: //CIN6  ->DRY KEY 
               if(run_t.gPower_On ==1){
                  run_t.gSig = DRY_SIG;
                    cprocess.state__ =  RUN ;
                    cprocess.cmdCtr__ = 15;
             
             }
           
             
         break;
         
         case 0x01: //CIN7 -> AI KEY
             if(run_t.gPower_On ==1){
                  run_t.gSig = AI_SIG;
                   cprocess.state__ =  RUN ;
                   cprocess.cmdCtr__ = 17;
             
             }
          
         break;
             
         default:
              run_t.gSig = KEY_SIG;
         break;
       
       } //switch(ReceiveBuffer) --end 
      
    }
 //   CProcessDispatch(&cprocess, run_t.gSig);
    
}
    

/************************************************************************
*
*Functin Name :void CProcessDispatch(CProcess1 *me, uint8_t sig)
*Function : dispatch task 
*Input Ref: CProcess *me -> pointer is state , sig - special signale
*Return Ref : NO
*
************************************************************************/ 
#if 0
static void CProcessDispatch(CProcess1 *me, uint8_t sig)
{

   static uint8_t fan,dry,ster,ai,po,longtimes,keyMode=0xff;
   static uint8_t fanflag,dryflag,sterflag,aiflag;
   static uint8_t n,m,p;
    
   switch (me->state__) {
   case IDLE: //state 
     switch (sig) {
          case KEY_SIG:
              me->cmdCtr__ = 0;   //timer 500ms scan key value

	         if(run_t.gPower_Cmd==1 || po==0 ){
			 	  if(po==0)po++;
			 	  if(run_t.gPower_Cmd==1)run_t.gPower_Cmd++ ;
                 Smg_AllOff();
	         }
         
            Breath_Led();
            run_t.gKeyValue++ ;
            break;
          }
     break;
     case CODE: //state 
      switch (sig) {
          case POWER_SIG:
                me->cmdCtr__ = 1;     /* SLASH-STAR count as comment */
	            run_t.gKeyValue=1 ;
                switch(run_t.gPower_Cmd){
                
                    case 0:
				
                     if(run_t.gTimer_500ms ==1){
                         run_t.gRun_flag = POWER_SIG;
                         run_t.gTimer_500ms = 0;
                         KeyLed_Power_On();
                    
                         
                         if(fan==0){ //0 -ON
                            LED_Fan_OnOff(0);
                            FAN_CCW_RUN();
                            run_t.gFan_flag=1;
                         }
                         else{ //1-OFF
                            LED_Fan_OnOff(1);
                            PTC_SetLow(); //PTC turn off 
                            FAN_Stop();
                            run_t.gFan_flag=0;
                         }
                         
                         if(ster==0){ //0 -ON
                            LED_Sterilizer_OnOff(0);
                            PLASMA_SetHigh();
                            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
                         }
                         else{ //1 -OFF
                              LED_Sterilizer_OnOff(1);
                              PLASMA_SetLow();
                             HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic off
                         }
                            
                              
                         if(dry==0){
                            LED_Dry_OnOff(0);
                            if(run_t.gFan_flag==1) //fan open maybe open PTC ,
                                 PTC_SetHigh();
                            else{
                                PTC_SetLow();
                            }
                         }
                         else{
                            LED_Dry_OnOff(1);
                            PTC_SetLow();
                         }
                         
                         if(ai==0){//ON
                            LED_AI_OnOff(0);
                            AI_Function_OnOff(0);
                         }
                         else{ //Off
                             LED_AI_OnOff(1);
                             AI_Function_OnOff(1);
                         }
                         //open PTC and FAN ,Ultrasonic 
                      
                     
                   }
			
                    if(run_t. gTimer_1s==1){
                       
                         if(run_t.gKey_long_flag==1){
						 	 
                             m = (run_t.gTimes_hours /10) %10;
					         n=  (run_t.gTimes_hours %10);
					         TM1640_Write_4Bit_Data(0,0,m,n,1) ; //timer is default 12 hours "H0:12"
                             if(run_t.gTimer_4s==1){
								run_t. gTimer_1s=0;
								run_t.gKey_long_flag =0;
							    run_t.gTimer_4s=0;
							 }
							 
                         }
                         else{ 
						 	 run_t. gTimer_1s=0;
                              if(p==0){

									p++;
									run_t.gTimes_hours_temp=12;
							  }

							  m = (run_t.gTimes_hours_temp /10) %10;
						      n=  (run_t.gTimes_hours_temp %10);
						      TM1640_Write_4Bit_Data(m,n,0,0,0) ; //timer is default 12 hours "12:00"

						 }

                    }

				 

                   if(run_t.gTimer_key_2s==1){//1s read one data
                       run_t.gTimer_1s =0;
                       Display_DHT11_Value(&DHT11);
					   
                    
                  	}
			
				   
                 break;
                  
                  case 1:
                         me->state__=IDLE;
                         sig =KEY_SIG ;
				  
                   break;
             }
           break;
                 
          case RUN_SIG:
              
              
          break;
    
      }
      break;
   /**********************state RUN*************************************/
   case RUN: //state
      switch (sig) {
       
          case MODE_SIG: //0x04

	       if(longtimes != keyMode ){
		   	     longtimes = keyMode;
                run_t.gKeyLongPressed ++;  //long be pressed counter numbers
	       }
           if(run_t.gKeyLongPressed > 100){  //Mode key be pressed long 
                run_t.gKeyLongPressed =0;
               run_t.gKeyLong =1;
               run_t.gTimer_5s_start =1; //timer is 5s start be pressed key 
               me->state__=RUN;
              sig = START_SIG ; 
			  run_t.gKeyValue++ ;
           } 
           else if(run_t.gKeyLong ==0){ //shot be pressed 

               run_t.gKey_long_flag=1;
			   run_t.gTimer_4s=0;
               me->state__=CODE;
		       sig = POWER_SIG ; 
			   keyMode++;
               run_t.gKeyValue++ ;
           }
		    
          break;

		  
          case ADD_SIG:
		  	
                run_t.gAdd_flag=1;
                run_t.gDec_flag=0; // //input setup times handle   
                run_t.gTimer_5s_start =0;  
				  me->state__=RUN;
                sig = START_SIG ;  //input setup times handle

			keyMode++;
               run_t.gKeyValue++ ; 
          break; 
             
          case DEC_SIG: //"-"
		  	   run_t.gAdd_flag=0;
               run_t.gDec_flag=1; // //input setup times handle   
              run_t.gTimer_5s_start =0; 
			   me->state__=RUN;
              sig = START_SIG ;    
             keyMode++;
               run_t.gKeyValue++ ;
          break;

		  //times setup timer handle 
             
           case START_SIG:
		   	   keyMode++;
              SteupTimes_Handle();
              run_t.gKeyValue++ ;  
          break;
             
             
          
          case FAN_SIG :
              fanflag = fanflag ^ 0x01;
              if(fanflag == 1){
                  fan = 1;
              
              }
              else{
                 fan =0;
              
              }
              //state transfer
             me->state__=CODE;
             sig =POWER_SIG;
              run_t.gKeyValue++ ;
              
          break;
          
          case DRY_SIG:
              dryflag = dryflag ^ 0x01;
              if(dryflag ==1){
                  dry=1;
              }
              else{
                  dry=0;
              }
              //state transfer
             me->state__=CODE;
             sig =POWER_SIG;
			  run_t.gKeyValue++ ;
              
          break;
          
          case PLASMA_SIG:
              sterflag = sterflag ^ 0x01;
              if(sterflag==1){
                 ster =1;
              }
              else{
               ster =0;  
              
              }
             //state transfer
             me->state__=CODE;
             sig =POWER_SIG;
             run_t.gKeyValue++ ;
          break;
          
          case AI_SIG:
              aiflag = aiflag ^ 0x01;
              if(aiflag==1){
                  ai =1;
              
              }
              else{
                 ai =0;
              
              }
              
            //state transfer
             me->state__=CODE;
             sig =POWER_SIG;
             run_t.gKeyValue++ ;
          break;
          
      }
      break;
  
   }
}

#endif 
static void SteupTimes_Handle(void)
{

  static uint8_t firstSetup,m,n,timesf;
  static int8_t times;
   //the first input setup times
   if(run_t.gKeyLong ==1){ // long times be pressed
   	    firstSetup ++;
   	   
        if(run_t.gTimer_key_5s < 1){
   
           
                if(run_t.gAdd_flag ==1){
				    run_t.gTimes_hours++;
					if(run_t.gTimes_hours>24)run_t.gTimes_hours=0 ;
                 }
				 else if(run_t.gDec_flag ==1){
                    run_t.gTimes_hours -- ;
					if(run_t.gTimes_hours <0)run_t.gTimes_hours=24;

				 }



			
			m = (run_t.gTimes_hours /10) %10;
            n=  (run_t.gTimes_hours %10);
           TM1640_Write_4Bit_Data(0,0,m,n,1) ; //timer is default 12 hours "H0:12"

        }
	    else{ //shot times be pressed 


		     run_t.gTime_total_hours = run_t.gTimes_hours * 3600;  
                     
             if(run_t.gTimes_hours ==0)run_t.gTimer_Cmd =0; //don't timer times
              else run_t.gTimer_Cmd =1; //start timers times times 
                     
               run_t.gKeyLongPressed =0;
               run_t.gKeyLong=0;
              
         
            //state transfer
            cprocess.state__=CODE;
            run_t.gSig =POWER_SIG;

			 
		}
   }
   else{ //shot times be pressed
         
          if(timesf ==0 ){
		   	 times ++ ;
		     times=12;
           if(run_t.gAdd_flag ==1){
				    times++;
					if(times>24)times =0 ;
             }
			else if(run_t.gDec_flag ==1){
                    times -- ;
					if(times <0)times=24;

				 }
                

			}  
         else{

				if(run_t.gAdd_flag ==1){
				    times++;
					if(times>24)times =0 ;
             }
				 else if(run_t.gDec_flag ==1){
                    times -- ;
					if(times <0)times=24;

				 }
                

			}  



		 if(timesf ==0 ){
		   	times=12;
		 }
		 run_t.gTimes_hours_temp =times;

         m = (run_t.gTimes_hours_temp /10) %10;
         n=  (run_t.gTimes_hours_temp %10);
         TM1640_Write_4Bit_Data(m,n,0,0,0) ; //timer is default 12 hours "12:00"
           //state transfer
		  cprocess.state__=CODE;
            run_t.gSig =POWER_SIG;
                  


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
void RunCommand_Mode(uint8_t sig)
{
     static uint8_t powerflag ,beepflag=0xff,dispt=0,longtimes,keyMode=0xff;
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
                  
                  run_t.gRun_flag= POWER_SIG ;
				 run_t.gPower_On =0;

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
			 	 
              if(longtimes != keyMode ){
		   	       longtimes = keyMode;
                   run_t.gKeyLongPressed ++;  //long be pressed counter numbers
	           }
           if(run_t.gKeyLongPressed > 50){  //Mode key be pressed long 
                run_t.gKeyLongPressed =0;
               run_t.gKeyLong =1;
               run_t.gTimer_5s_start =1; //timer is 5s start be pressed key 
            
                run_t.gKeyValue++ ;
			   run_t.gRun_flag= RUN_SIG ;
           } 
           else if(run_t.gKeyLong ==0){ //shot be pressed 

               run_t.gKey_long_flag=1;
			   run_t.gTimer_4s=0;
		      
			   keyMode++;
               run_t.gKeyValue++ ;
			   run_t.gRun_flag= RUN_SIG ;
           }

		break;
         
         case 0x20: //CIN2 ->ADD KEY
             if(run_t.gPower_On ==1){


				 if( run_t.gKeyLong ==1){

                    run_t.gKey_long_flag=1;
					run_t.gTimes_hours++;
				    if(run_t.gTimes_hours >24){
						run_t.gTimes_hours=0;
					}

				 }
				 else{

					if(dispt==0){

						dispt++;
						run_t.gTimes_hours_temp=12;
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
			 	  if( run_t.gKeyLong ==1){

                    run_t.gKey_long_flag=1;
					run_t.gTimes_hours--;
				    if(run_t.gTimes_hours <0){
						run_t.gTimes_hours=24;
					}

				 }
				 else{

                     if(dispt==0){

						dispt++;
						run_t.gTimes_hours_temp=12;
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
          run_t.gKeyValue++;


	  break;

	  case  RUN_SIG: //1
         run_t.gKeyLongPressed=0;
		 run_t.gKeyValue++;
		
		if(run_t.gTimer_500ms ==1){
			run_t.gRun_flag = POWER_SIG;
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

		if(run_t. gTimer_1s==1){

			run_t. gTimer_1s=0;
			if(run_t.gKey_long_flag==1){
				run_t.gKey_long_flag =0;

				m = (run_t.gTimes_hours /10) %10;
				n=  (run_t.gTimes_hours %10);
				TM1640_Write_4Bit_Data(0,0,m,n,1) ; //timer is default 12 hours "H0:12"
				

		     }
		   else{ 
			run_t. gTimer_1s=0;
			
            m = (run_t.gTimes_hours_temp /10) %10;
			n=  (run_t.gTimes_hours_temp %10);
			TM1640_Write_4Bit_Data(m,n,0,0,0) ; //timer is default 12 hours "12:00"

			}

		}

						 

		if(run_t.gTimer_key_2s==1){//1s read one data
			run_t.gTimer_1s =0;
			Display_DHT11_Value(&DHT11);
		}

		

	  break;

	  

	 }





}


