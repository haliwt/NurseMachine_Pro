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

static void Timer_Handle(void);


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
     static uint8_t powerflag ,beepflag=0xff;
	 

    if(sig!=0){
		 if(beepflag !=run_t.gKeyValue){
		 	beepflag = run_t.gKeyValue ;
            Buzzer_On();
			
		 }
     }
	 else{
         return ;

	 }
	 
       switch(sig){
	   	
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
            
             }
         break;
         
         case 0x40: //CIN1 -> MODE KEY
             if(run_t.gPower_On ==1){
			 	 
                run_t.gKeyInput_flag=1;
			  	if(run_t.gKeyMode ==0){
				   run_t.gKeyMode++;
				   run_t.gTimer_4s=0;
                   run_t.gTimer_3s=0;
				}
			    
            if(run_t.gTimer_3s  >1){  //Mode key be pressed long 
               run_t.gKeyLong =1;
                run_t.gTimer_key_5s=0;
                run_t.gKeyValue++ ;
			    run_t.gKeyMode=0;
			   run_t.gTimer_3s=0;
			   run_t.gRun_flag= RUN_SIG ;
			 
           } 
           else if(run_t.gKeyLong ==0){ //shot be pressed 
               run_t.gKey_display_timer=1;
               run_t.gKeyValue++ ;
			   run_t.gRun_flag= RUN_SIG ;
			
           }
		   
          }
		break;
         
         case 0x20: //CIN2 ->ADD KEY
             if(run_t.gPower_On ==1){

					run_t.gKeyMode=0;
					run_t.gKeyInput_flag=1;
				 if( run_t.gKeyLong ==1){

					 run_t.gTimer_key_5s=0;// run_t.gTimer_5s_start =0; //timer is 5s start be pressed key 
					 run_t.gTimer_adtem=0;
					run_t.gTimes_hours++;
				    if(run_t.gTimes_hours >24){
						run_t.gTimes_hours=0;
					}
                    run_t.gTimer_key_5s=0;//run_t.gTimer_5s_start =1; //timer is 5s start be pressed key 
				 }
				 else{
                      run_t.gTimer_adtem=1; //don't be key pressed long times
					//  run_t.gKey_display_timer=0;//don't input timer of times edit funciton
				      run_t.gSig =1;
					 //setup temperature of value 
					 run_t.gSig_flag =1;
					 run_t.gTemp_flag=0; //be clear be check setup shut off times ,int timer in counter
					 run_t.gTemperature ++;

					 if(run_t.gTemperature < 20)run_t.gTemperature= 21;
					 else if(run_t.gTemperature >40) run_t.gTemperature=20;
					 run_t.gTemp_flag =0;
					
				 }
			 
			 	 run_t.gRun_flag= RUN_SIG ;
             
             	}
            
             
         break;
         
         case 0x10: //CIN3 -> DEC KEY
             if(run_t.gPower_On ==1){
			 	  run_t.gKeyMode=0;
				  run_t.gKeyInput_flag=1;
				  // setup Timer of times 
			 	  if( run_t.gKeyLong ==1){
                      run_t.gTimer_key_5s=0;//run_t.gTimer_5s_start =0; //timer is 5s start be pressed key 
              
					run_t.gTimes_hours--;
				    if(run_t.gTimes_hours <0){
						run_t.gTimes_hours=24;
					}
					run_t.gTimer_key_5s=0;//run_t.gTimer_5s_start =1; //timer is 5s start be pressed key 

				 }
				 else{
                      
                    run_t.gTimer_adtem=1;//don't be key pressed long times
					// run_t.gKey_display_timer=0;
					  run_t.gSig =1;
				      run_t.gSig_flag =1;
					  run_t.gTemp_flag=0; //be clear be check setup shut off times 
					//setup temperature of value 
					 run_t.gTemperature --;
					 if(run_t.gTemperature<20) run_t.gTemperature=40;
					  run_t.gTemp_flag =0;
				 }
			 
			 	 run_t.gRun_flag= RUN_SIG ;
             
             }
           
             
         break;
         
         case 0x08: //CIN4 -> FAN KEY 
               if(run_t.gPower_On ==1){
                   run_t.gKeyMode=0;
				   run_t.gKeyInput_flag=1;
			    
			    run_t.gFan =run_t.gFan^ 0x01;
				if(run_t.gFan == 1)run_t.gDry =1;
			     
				
			   	 run_t.gRun_flag= RUN_SIG ;
                
               
             
             }
            
         break;
         
         case 0x04: //CIN5  -> STERILIZATION KEY 
             if(run_t.gPower_On ==1){
				 run_t.gKeyMode=0;
			 run_t.gKeyInput_flag=1;

			   run_t.gPlasma =run_t.gPlasma ^ 0x01;
				
               run_t.gRun_flag= RUN_SIG ;
             
             }
            
            
         break;
         
         case 0x02: //CIN6  ->DRY KEY 
               if(run_t.gPower_On ==1){
				  run_t.gKeyMode=0;
				 run_t.gKeyInput_flag=1;

			      run_t.gDry = run_t.gDry^ 0x01;
			      if(run_t.gDry == 0)run_t.gFan =0;
				  
                  run_t.gRun_flag= RUN_SIG ;
             }
           
             
         break;
         
         case 0x01: //CIN7 -> AI KEY
             if(run_t.gPower_On ==1){

			       run_t.gKeyMode=0;
				   run_t.gKeyInput_flag=1;
				  run_t.gAi = run_t.gAi ^ 0x01;
				  if(run_t.gAi ==1){
					   run_t.gAi_Led =0;
			
					   run_t.gFan=1;
					   run_t.gPlasma=1;
						run_t.gDry=1;
						run_t.gAi_Led =1; //
						run_t.gAi_fun = 0; //AI functin ON
				

                 }
                   else{ //AI ON 
					  run_t.gAi=0;
					  run_t.gAi_Led =0;
                      run_t.gAi_fun =0; //AI Function ON
			
						 run_t.gFan=0;
						 run_t.gPlasma=0;
						 run_t.gDry=0;
						 run_t.gAi_Led =0;
					
						}
                      run_t.gRun_flag= RUN_SIG ;

             }
                  
          break;
             
         default:
             
         break;
       
       } //switch(ReceiveBuffer) --end 
      
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

	switch(run_t.gRun_flag){

      case IDEL_SIG :
	  	 if(run_t.gPower_On ==0){

			  
			  	 run_t.gPower_On=0XFF;
			 	Smg_AllOff();
				   run_t.gKeyLong = 0;
                  run_t.gKeyValue++;
		  
				 //run_t.gPower_On =0;
				 run_t.gFan=0;
				 run_t.gPlasma=0;
				 run_t.gDry=0;
				 run_t.gAi_Led =0;
				run_t.gTemperature =0;
				run_t.gSig = 0;

				  run_t.gTimer_Cmd=0; //timer of command "1"->timer is start
				  run_t.gTimes_hours=0;
				  run_t.gTimes_minutes=0;
				  run_t.gSig=0;
				  run_t.gKeyLong =0;
				  run_t.gKey_display_timer=0;
				  run_t.gTimer_adtem =0;
				  run_t.gDht11_flag=0; //the first power on display "00"
				  run_t.gTimes_hours_temp=12;
				  run_t.gTimes_minutes_temp=0;
				  run_t.gSig = 0;
				   run_t.gSig_flag =0;
				   run_t.gTemp_flag=0;
				  ShutDown_AllFunction();
                  
			  	
			  
			   
	  	 	}
		
	 
          
           Breath_Led();
		
		 


	  break;

	  case  RUN_SIG: //1
		 run_t.gKeyValue++;
		
	   if(run_t.gTimer_500ms ==1){
		
			run_t.gTimer_500ms = 0;
			KeyLed_Power_On();
		
	        AI_Function_OnOff();
	  }
	   
      if( run_t.gTimer_60ms==1 ){
	  	 run_t.gTimer_60ms=0;
		 //run_t.gKeyInput_flag=0;
	     Display_Function_OnOff();
      }
	
     Timer_Handle();
		
		
		if(run_t.gTimer_4s==1 || run_t.gDht11_flag ==0 ){//1s read one data
		
			
			run_t.gTimer_4s =0;
			run_t.gTimer_3s=0;
			run_t.gKeyMode =0;
			Display_DHT11_Value(&DHT11);
		   //setup temperature and environment 
		   if(run_t.gTemp_flag == 1){ //60s be check once 
		       run_t.gTemp_flag =0;
		       if(run_t.gSig ==1){
			     if(run_t.gTemperature <= DHT11.temp_high8bit){
							
				         run_t.gPower_On=0;
					     run_t.gRun_flag= IDEL_SIG ; //turn off 
			   }
		    }
		    
		  }
		
	   }

	  break;

	  

	 }





}
/**********************************************************************
*
*Functin Name: static void Timer_Handle(void)
*Function : Timer of key be pressed handle
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
static void Timer_Handle(void)
{
     static uint8_t m,n,p,q;
	//mode key long be pressed handle
	if( run_t.gTimer_30ms==1 || run_t.gKeyLong ==1){

	run_t.gTimer_30ms=0;
	if(run_t.gKeyLong ==1 || run_t.gKey_display_timer ==1){	//gkey_
		

	   if(run_t.gKey_display_timer ==1 || run_t.gKeyLong ==1 ){
			m = (run_t.gTimes_hours /10) ;
			n=	(run_t.gTimes_hours %10);
		}
	   //display of timer of times but don't edit timer of times,shot times key
	    if(run_t.gTimer_Cmd==1 && run_t.gKey_display_timer==1){
			if(run_t.gTimes_minutes >0){
                 if(run_t.gTimes_hours ==0){
				 	m=0;
				 	n = 0x01;

				 }
				 else{
                    m = ((run_t.gTimes_hours + 1)/10) ;
			        n=	((run_t.gTimes_hours+1) %10);
					
                    
				 }
 
			}
			
		   }
		
			
	   TM1640_Write_4Bit_Data(0,0,m,n,1) ; //timer is default 12 hours "H0:12"
		
		if(run_t.gKeyLong ==1){ //long times be pressed blank led
			
			 
			  if(run_t.gTimer_10ms==1){
			  	  run_t.gTimer_10ms=0;
				  Times_Led_IndicationOnOff(1); //Off
				}
			  else{
				 Times_Led_IndicationOnOff(0); //On

			  }
			
			
			
		}
		run_t.gKey_display_timer=0;//display Timers of hours but don't edit hours numbers

	}
	else{ // short times be pressed 
			 run_t.gTimer_30ms=0;
             if(run_t.gTimer_Cmd==1 && run_t.gTimer_adtem !=1){

                
				m = (run_t.gTimes_hours /10) ;
			    n=	(run_t.gTimes_hours %10); 
				p = (run_t.gTimes_minutes /10);
				q=  (run_t.gTimes_minutes %10);
			    TM1640_Write_4Bit_Data(m,n,p,q,0) ; //timer is default 12 hours "12:00"

			}
		    else{

                    if(run_t.gSig==0 || run_t.gSig_flag ==0){ //display normal times don't edit  timer of times 

						m = (run_t.gTimes_hours_temp /10) ;
					    n=	(run_t.gTimes_hours_temp%10); 
						p = (run_t.gTimes_minutes_temp /10);
						q=  (run_t.gTimes_minutes_temp %10);
						TM1640_Write_4Bit_Data(m,n,p,q,0) ; //timer is default 12 hours "12:00"
				
					}
					else{ //Temperature of setup auto shut off machine
						
							m = (run_t.gTemperature /10);
							n=	(run_t.gTemperature %10);
					        if(run_t.gTimer_Cmd==1)run_t.gTimer_adtem =0;
					        else run_t.gSig_flag =0;
						    TM1640_Write_4Bit_TemperatureData(m,n);
				       }
				
		       }
			
			

		 }
	}


	//timer is times
	if(run_t.gTimer_key_5s >10 &&  run_t.gKeyLong ==1){
		 if(run_t.gTimes_hours >0){	 
		      run_t.gTimer_Cmd=1;	  //timer is times start  
		      run_t.gTimes_minutes =0;
		      run_t.gTimer_flag=0;
			  
		 }
		 else{
		    run_t.gTimer_Cmd=0;
			run_t.gTimes_minutes =0;
         }
		run_t.gKeyLong =0;		
	}



}


