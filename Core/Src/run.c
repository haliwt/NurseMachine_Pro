#include "run.h"
#include "key.h"
#include "smg.h"
#include "dht11.h"
#include "fan.h"
#include "tim.h"



static void CProcessDispatch(CProcess1 *me, uint8_t sig) ;

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
                run_t.gSig = POWER_SIG ;
                cprocess.state__ =  CODE ;
                cprocess.cmdCtr__ = 1;
             }
             else{
                 Smg_AllOff();
                 run_t.gSig = KEY_SIG ;
                 cprocess.state__=IDLE; //state 
                 run_t.gSig =KEY_SIG ;
             }
         break;
         
         case 0x40: //CIN1 -> MODE KEY
             if(run_t.gRun_flag ==POWER_SIG){
                 run_t.gSig = MODE_SIG;
                 cprocess.state__ =  RUN ;
                 cprocess.cmdCtr__ = 3;
                 run_t.gKeyLongPressed++ ; //adjust long key be pressed numbers
                 
             
             }
          
         break;
         
         case 0x20: //CIN2 ->ADD KEY
             if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = ADD_SIG;
                  cprocess.state__ =  RUN ;
                  cprocess.cmdCtr__ = 5;
             
             }
            
             
         break;
         
         case 0x10: //CIN3 -> DEC KEY
             if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = DEC_SIG;
                  cprocess.state__ =  RUN ;
                 cprocess.cmdCtr__ = 7;
             
             }
           
             
         break;
         
         case 0x08: //CIN4 -> FAN KEY 
               if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = FAN_SIG;
                   cprocess.state__ =  RUN ;
                   cprocess.cmdCtr__ = 11;
             
             }
            
         break;
         
         case 0x04: //CIN5  -> STERILIZATION KEY 
             if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = PLASMA_SIG;
                  cprocess.state__ =  RUN ;
                   cprocess.cmdCtr__ = 13;
             
             }
            
            
         break;
         
         case 0x02: //CIN6  ->DRY KEY 
               if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = DRY_SIG;
                    cprocess.state__ =  RUN ;
                    cprocess.cmdCtr__ = 15;
             
             }
           
             
         break;
         
         case 0x01: //CIN7 -> AI KEY
             if(run_t.gRun_flag ==POWER_SIG){
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
    CProcessDispatch(&cprocess, run_t.gSig);
    
}
    

/************************************************************************
*
*Functin Name :void CProcessDispatch(CProcess1 *me, uint8_t sig)
*Function : dispatch task 
*Input Ref: CProcess *me -> pointer is state , sig - special signale
*Return Ref : NO
*
************************************************************************/ 
static void CProcessDispatch(CProcess1 *me, uint8_t sig)
{
   static uint8_t fira,fird,fan,dry,ster,ai,po;
   static uint8_t fanflag,dryflag,sterflag,aiflag;
   static int8_t n,m,p,q;
    
   switch (me->state__) {
   case IDLE: //state 
     switch (sig) {
          case KEY_SIG:
             //run_t.gTimer = 0 ;  //timer 500ms scan key value
            if(po==0){
                po++;
                Smg_AllOff();
            }
            if(run_t.gPower_Cmd==1){
                 Smg_AllOff();
                 run_t.gPower_Cmd++ ;
            }
            run_t.gRun_flag = KEY_SIG;
            Breath_Led();
            run_t.gKeyValue++ ;
            break;
          }
     break;
     case CODE: //state 
      switch (sig) {
          case POWER_SIG:
                me->cmdCtr__ = 2;     /* SLASH-STAR count as comment */
                switch(run_t.gPower_Cmd){
                
                    case 0:
				
                     if(run_t.gTimer_500ms ==1){
                         run_t.gRun_flag = POWER_SIG;
                         run_t.gTimer_500ms = 0;
                         KeyLed_Power_On();
                         LED_MODE_On();
                         LED_TempHum_On();
                         
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
                      
                     
                   // }
                    if(run_t. gTimer_1s==1){
                        run_t. gTimer_1s=0;
                         if(fira !=0 || fird !=0){
                             m = (run_t.gTimes_hours /10) %10;
                             n=  (run_t.gTimes_hours %10);
                             p= (run_t.gTimes_minutes /10) %10;
                             q= run_t.gTimes_minutes %10;
                             TM1640_Write_4Bit_Data(m,n,p,q,0) ;
                         }
                         else  
                            TM1640_Write_4Bit_Data(0x01,0x2,0x00,0x00,0) ;  //display times  4bit

                    }

                   if(run_t.gTimer_key_2s==1){//1s read one data
                       run_t.gTimer_1s =0;
                       Display_DHT11_Value(&DHT11);
					   
                    
                  }
				   run_t.gKeyValue++ ;
                 break;
                  
                  case 1:
                         me->state__=IDLE;
                         sig =KEY_SIG ;
				  run_t.gKeyValue++ ;
                   break;
             }
           break;
                 
          case RUN_SIG:
              
              
          break;
    
      }
      break;
   case RUN: //state
      switch (sig) {
       
          case MODE_SIG:
           if(run_t.gKeyLongPressed > 300){  //Mode key be pressed long   
               run_t.gKeyLong =1;
               run_t.gTimer_start =1; //timer is 5s start 
              // TM1640_Write_4Bit_Data(0x00,0x0,0x01,0x02,1) ; //timer is default 12 hours "H0:12"
               
              sig = START_SIG ; 
           } 
           else{ //shot be pressed 
               if((fira !=0 || fird !=0)){
                   TM1640_Write_4Bit_Data(0,0,m,n,1) ;   
               }
               else{
                  TM1640_Write_4Bit_Data(0x0,0x0,0x01,0x02,1) ;  //display times  4bit //display times of timer "H0:XX"
               }
               run_t.gKeyLong =0;
               run_t.gKeyLongPressed=0;
           
           }
		    run_t.gKeyValue++ ;
            break;
          case ADD_SIG:
             if(run_t.gTimer_key_5s < 1 && run_t.gKeyLong ==1){
                   run_t.gTimer_start =0;
                   if(fira == 0){
                      fira =1;
                        run_t.gTimes_hours = 13; // 11 hours
                   }
                   else{
                     run_t.gTimes_hours++;
                       
                   }
                   if(run_t.gTimes_hours >24){
                      run_t.gTimes_hours=0;
                   }
                 
                     
                m = (run_t.gTimes_hours /10) %10;
                n=  (run_t.gTimes_hours %10);
                TM1640_Write_4Bit_Data(0,0,m,n,1) ;
                run_t.gTimer_start =1;                    
                sig = START_SIG ;                  
             
             }
             else{
          
             
             }
              run_t.gKeyValue++ ; 
          break; 
             
          case DEC_SIG:
              if(run_t.gTimer_key_5s < 1 && run_t.gKeyLong ==1){
                   run_t.gTimer_start =0;
                   if(fird == 0){
                      fird =1;
                      run_t.gTimes_hours = 11; // 11 hours
                   }
                   else{
                      run_t.gTimes_hours--;
                       
                   }
                 if(run_t.gTimes_hours < 0 ){
                     run_t.gTimes_hours=24; //hours
                       
                   }
                 m = (run_t.gTimes_hours /10) %10;
                 n=  (run_t.gTimes_hours %10);
                 TM1640_Write_4Bit_Data(0,0,m,n,1) ;  
                 run_t.gTimer_start =1;                   
                 sig = START_SIG ;    
             }
               run_t.gKeyValue++ ;
          break;
             
           case START_SIG:
               if(run_t.gKeyLong ==1 && (fira !=0 || fird !=0)){
                 if(run_t.gTimer_key_5s==1){
            
                    run_t.gTime_total_hours = run_t.gTimes_hours * 3600;  
                     
                    if(run_t.gTimes_hours ==0)run_t.gTimer_Cmd =0; //don't timer times
                    else run_t.gTimer_Cmd =1; //start timers times times 
                     
                    run_t.gKeyLongPressed =0;
                    run_t.gKeyLong=0;
                    fira=0;
                    fird =0;
                    run_t.gTimer_key_5s=0;
                    //state transfer
                    me->state__=CODE;
                    sig =POWER_SIG;
                 
                 }
               
               }
               else{
                  if(run_t.gTimer_key_5s==1 && (run_t.gKeyLong ==1)){
                    run_t.gTimer_Cmd =1;
                    run_t.gTime_total_hours = 12*3600; //hours
                    run_t.gTimes_hours =12;
                    run_t.gKeyLongPressed =0;
                    run_t.gKeyLong=0;
                    run_t.gTimer_key_5s=0;
                     //state transfer
                    me->state__=CODE;
                    sig =POWER_SIG;
                 }
              }
               
             if(fira !=0 || fird !=0){ //blank SMG effect
                 m = (run_t.gTimes_hours /10) %10;
                 n=  (run_t.gTimes_hours %10);
                 TM1640_Write_4Bit_Data(0,0,m,n,1) ; 
             }
             else{
                 TM1640_Write_4Bit_Data(0,0,1,2,1) ; //timer is default 12 hours "H0:12"
             
             }
              
             if(run_t.gKeyLong==1){ //turn off Smg display 
                  
                  if(run_t.gTimer_key_2s==1){ //turn off smg
                      run_t.gTimer_key_2s=0;
                      TM1640_Write_4Bit_Data(0,0,1,2,2) ; //turn off 4BIT smg
                  }
             
             }
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

}





