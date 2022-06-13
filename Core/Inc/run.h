#ifndef __RUN_H__
#define __RUN_H__
#include "main.h"


enum Signal {                   /* enumeration for CProcess signals */
   KEY_SIG,POWER_SIG,RUN_SIG,START_SIG,MODE_SIG,ADD_SIG,DEC_SIG,FAN_SIG,STER_SIG,DRY_SIG,AI_SIG
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

   uint8_t gKeyValue;   
   uint8_t gSig;
   uint8_t gRun_flag;
   uint16_t gTimer;


}RUN_T;

#define CProcessInit(me_) ((me_)->cmdCtr__ =0,(me_)->state__ = IDLE)

extern RUN_T run_t; 

void CProcessRun_Init(void);
void CProcess_Run(void);


#endif 


