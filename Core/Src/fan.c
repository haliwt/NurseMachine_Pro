#include "fan.h"
#include "main.h"

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

