#include "run.h"
#include "key.h"
#include "smg.h"
#include "dht11.h"
#include "fan.h"
#include "tim.h"



static void CProcessDispatch(CProcess1 *me, uint8_t sig) ;

DHT11_Data_TypeDef *DHT11;

uint8_t ReceiveBuffer[1]={0};
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
   
    run_t.gKeyValue = I2C_SimpleRead_From_Device(ReceiveBuffer) ;
    while(run_t.gKeyValue !=0){
    
       switch(ReceiveBuffer[1]){
       
         case 0x80: //CIN0 -> POWER KEY 
             run_t.gSig = POWER_SIG ;
             cprocess.state__ =  CODE ;
             cprocess.cmdCtr__ = 1;
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
                  run_t.gSig = STER_SIG;
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
   static uint8_t fira,fird;
   static int8_t n,m;
    
   switch (me->state__) {
   case IDLE: //state 
     switch (sig) {
          case KEY_SIG:
             //run_t.gTimer = 0 ;  //timer 500ms scan key value 
            
            break;
          }
     break;
     case CODE: //state 
      switch (sig) {
          case POWER_SIG:
                me->cmdCtr__ = 2;     /* SLASH-STAR count as comment */
                if(run_t.gTimer_500ms ==1){
                     run_t.gRun_flag = POWER_SIG;
                     run_t.gTimer_500ms = 0;
                     LED_Power_On();
                     LED_MODE_On();
                     LED_TempHum_On();
                     LED_Fan_On();
                     LED_Sterilizer_On();
                     LED_Dry_On();
                     LED_AI_On();
                     //open PTC and FAN ,Ultrasonic 
                     TM1640_Write_4Bit_Data(0x01,0x2,0x00,0x00,0) ;  //display times  4bit
                     FAN_CCW_RUN();    //FAN ON 
                     PLASMA_SetHigh() ; //sterilization ON
                     PTC_SetHigh() ; //PTC ON   
                     HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
                    
                 }
                 if(run_t.gTimer_1s==1){//1s read one data
                   Display_DHT11_Value(DHT11);
                   run_t.gTimer_1s =0;
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
               TM1640_Write_4Bit_Data(0x00,0x0,0x01,0x02,1) ; //timer is default 12 hours "H0:12"
               if(run_t.gTimer_key_2s==1){
                  TM1640_TimLed_Off();
                     
               }
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
            break;
          case ADD_SIG:
             if(run_t.gTimer_key_5s < 1 && run_t.gKeyLong ==1){
                   run_t.gTimer_start =0;
                   if(fira == 0){
                      fira =1;
                       n = 3;
                   }
                   else{
                      n ++;
                       
                   }
                   if(n ==10 && m< 3){
                      n=0;
                      m++;
                   }
                  if(m==2){
                    if(n> 4){
                       m=0;
                       n=0;
                    }
                     
                  }
                TM1640_Write_4Bit_Data(0,0,m,n,1) ;   
                sig = START_SIG ;                  
             
             }
             else{
          
             
             }
              
          break; 
             
          case START_SIG:
               if(run_t.gKeyLong ==1 && (fira !=0 || fird !=0)){
                 if(run_t.gTimer_key_5s==1){
                    run_t.gTimer_Cmd =1;
                    run_t.gTime_hour = (m*10 + n)*3600;  
                    run_t.gKeyLongPressed =0;
                 
                 }
               
               }
               else{
                  if(run_t.gTimer_key_5s==1 && (run_t.gKeyLong ==1)){
                     run_t.gTimer_Cmd =1;
                    run_t.gTime_hour = 12*3600; //hours
                    run_t.gKeyLongPressed =0;
                 }
               
               
               }
          
          break;
          
          case DEC_SIG:
              if(run_t.gTimer_key_5s < 1 && run_t.gKeyLong ==1){
                   if(fird == 0){
                      fird =1;
                       n = 1;
                   }
                   else{
                      n --;
                       
                   }
                 if(n < 0 ){
                      n=9;
                      m--;
                     if(m < 0 ){
                        n=4;
                        m=2;
                      }
                       
                   }
                 
                TM1640_Write_4Bit_Data(m,n,0x00,0x00,1) ;                   
             
             }
              
          break;
          
          case FAN_SIG :
              
          break;
          
          case DRY_SIG:
              
          break;
          
          case STER_SIG:
              
          break;
          
          case AI_SIG:
              
          break;
          
      }
      break;
  
   }
}







