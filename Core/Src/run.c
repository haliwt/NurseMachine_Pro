#include "run.h"
#include "key.h"
#include "smg.h"
#include "dht11.h"



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
             run_t.gRun_flag = POWER_SIG;
             cprocess.state__ =  CODE ;
             cprocess.cmdCtr__ = 1;
         break;
         
         case 0x40: //CIN1 -> MODE KEY
             if(run_t.gRun_flag ==POWER_SIG){
                 run_t.gSig = MODE_SIG;
                 cprocess.state__ =  RUN ;
                 cprocess.cmdCtr__ = 3;
             
             }
             else{
                run_t.gSig = KEY_SIG ;
             
             }
         break;
         
         case 0x20: //CIN2 ->ADD KEY
             if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = ADD_SIG;
                  cprocess.state__ =  RUN ;
                  cprocess.cmdCtr__ = 5;
             
             }
             else{
                run_t.gSig = KEY_SIG ;
             
             }
             
         break;
         
         case 0x10: //CIN3 -> DEC KEY
             if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = DEC_SIG;
                  cprocess.state__ =  RUN ;
                 cprocess.cmdCtr__ = 7;
             
             }
             else{
                run_t.gSig = KEY_SIG ;
             
             }
             
         break;
         
         case 0x08: //CIN4 -> FAN KEY 
               if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = FAN_SIG;
                   cprocess.state__ =  RUN ;
                   cprocess.cmdCtr__ = 11;
             
             }
             else{
                run_t.gSig = KEY_SIG ;
             
             }
         break;
         
         case 0x04: //CIN5  -> STERILIZATION KEY 
             if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = STER_SIG;
                  cprocess.state__ =  RUN ;
                   cprocess.cmdCtr__ = 13;
             
             }
             else{
                run_t.gSig = KEY_SIG ;
             
             }
            
         break;
         
         case 0x02: //CIN6  ->DRY KEY 
               if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = DRY_SIG;
                    cprocess.state__ =  RUN ;
                    cprocess.cmdCtr__ = 15;
             
             }
             else{
                run_t.gSig = KEY_SIG ;
             
             }
             
         break;
         
         case 0x01: //CIN7 -> AI KEY
               if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = AI_SIG;
                   cprocess.state__ =  RUN ;
                   cprocess.cmdCtr__ = 17;
             
             }
             else{
                run_t.gSig = KEY_SIG ;
             
             }
             
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
void CProcessDispatch(CProcess1 *me, uint8_t sig)
{
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
                 LED_Power_On();
                 LED_MODE_On();
                 LED_TempHum_On();
                 LED_Fan_On();
                 LED_Sterilizer_On();
                 LED_Dry_On();
                 LED_AI_On();
                 //1s read one data
                 Display_DHT11_Value(DHT11);
           break;
    
      }
      break;
   case RUN: //state
      switch (sig) {
       
          case MODE_SIG:
              
            break;
          case ADD_SIG:
              
          break; 
          
          case DEC_SIG:
              
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







