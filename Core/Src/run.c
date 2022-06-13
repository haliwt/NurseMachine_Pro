#include "run.h"
#include "key.h"

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
             
             }
             else{
                run_t.gSig = KEY_SIG ;
             
             }
         break;
         
         case 0x20: //CIN2 ->ADD KEY
             if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = ADD_SIG;
             
             }
             else{
                run_t.gSig = KEY_SIG ;
             
             }
             
         break;
         
         case 0x10: //CIN3 -> DEC KEY
             if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = DEC_SIG;
             
             }
             else{
                run_t.gSig = KEY_SIG ;
             
             }
             
         break;
         
         case 0x08: //CIN4 -> FAN KEY 
               if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = FAN_SIG;
             
             }
             else{
                run_t.gSig = KEY_SIG ;
             
             }
         break;
         
         case 0x04: //CIN5  -> STERILIZATION KEY 
             if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = STER_SIG;
             
             }
             else{
                run_t.gSig = KEY_SIG ;
             
             }
            
         break;
         
         case 0x02: //CIN6  ->DRY KEY 
               if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = DRY_SIG;
             
             }
             else{
                run_t.gSig = KEY_SIG ;
             
             }
             
         break;
         
         case 0x01: //CIN7 -> AI KEY
               if(run_t.gRun_flag ==POWER_SIG){
                  run_t.gSig = AI_SIG;
             
             }
             else{
                run_t.gSig = KEY_SIG ;
             
             }
             
         break;
       
       } //switch(ReceiveBuffer) --end 
      CProcessDispatch(&cprocess, run_t.gSig);
    }


}
    

 
 
uint8_t CProcessInit(void)
{
    
    run_t.gKeyValue = I2C_SimpleRead_From_Device(ReceiveBuffer) ;
    return run_t.gKeyValue;
   
}


void CProcessDispatch(CProcess1 *me, uint8_t sig)
{
   switch (me->state__) {
   case IDLE:
      switch (sig) {
      case KEY_SIG:
       //  CParser1Tran(me, SLASH);         /* transition to "slash" */
         break;
      }
      break;
   case CODE:
      switch (sig) {
      case POWER_SIG:
         me->cmdCtr__ += 2;     /* SLASH-STAR count as comment */
        // CParser1Tran(me, COMMENT);     /* transition to "comment" */
         break;
      case RUN_SIG:
      
        // CParser1Tran(me, CODE);              /* go back to "code" */
         break;
      }
      break;
   case RUN:
      switch (sig) {
      case START_SIG:
           // CParser1Tran(me, STAR);           /* transition to "star" */
     
         ++me->cmdCtr__;             /* count the comment char */
         break; 
      }
      break;
  
   }
}







