#ifndef __RUN_H__
#define __RUN_H__
#include "main.h"


enum Signal {                   /* enumeration for CProcess signals */
    KEY_SIG,RUN_SIG,IDEL_SIG
};

enum State {                     /* enumeration for CProcess states */
   IDLE, CODE, RUN
};

typedef struct CProcess1 CProcess1;  
struct CProcess1 {
   enum State state__;                /* the scalar state-variable */
   uint8_t cmdCtr__;                 /* comment character counter */
   /* ... */                          /* other CParser1 attributes */
};

typedef struct _RUN_T{

   uint8_t gPower_Cmd;
   uint8_t gKeyValue;   
   uint8_t gSig;
   uint8_t gPower_On;
   uint8_t gRun_flag;

   uint8_t gKey_display_timer;

  
   uint8_t  gTimer_10ms;
   uint8_t  gTimer_20ms;
   uint8_t  gTimer_30ms;
   uint8_t  gTimer_50ms;
   uint8_t  gTimer_100ms;
   uint8_t  gTimer_200ms;
   uint8_t  gTimer_300ms;
   uint8_t  gTimer_400ms;
   uint8_t  gTimer_500ms;
   uint8_t  gTimer_1s;
   uint8_t  gTimer_key_2s;
   uint8_t  gTimer_3s;
   uint8_t  gKeyLong;
   uint8_t  gTimer_4s;
   uint8_t  gTimer_key_5s;
   uint8_t  gTimer_Cmd;
   uint8_t  gFan;
   uint8_t  gPlasma;
   uint8_t  gDry;
   uint8_t  gAi;
   int8_t   gTimes_minutes;
   int8_t   gTimes_hours;
   int8_t gTimes_hours_temp;
   uint16_t gTime_total_hours;
   uint16_t gKeyLongPressed;

}RUN_T;

#define CProcessInit(me_) ((me_)->cmdCtr__ =0,(me_)->state__ = IDLE)
extern uint8_t ReceiveBuffer[1];
extern RUN_T run_t; 

//void CProcessRun_Init(void);
//void CProcess_Run(void);
void RunCommand_Mode(uint8_t sig);
void RunCommand_Order(void);



#endif 


