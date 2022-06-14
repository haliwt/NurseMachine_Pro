#ifndef __FAN_H__
#define __FAN_H__
#include "main.h"


#define FAN_CCW       GPIO_PIN_4
#define FAN_CW        GPIO_PIN_5
#define FAN_GPIO      GPIOA

#define FAN_CCW_SetHigh()            HAL_GPIO_WritePin(FAN_GPIO,FAN_CCW,GPIO_PIN_SET)    // output high level
#define FAN_CCW_SetLow()             HAL_GPIO_WritePin(FAN_GPIO,FAN_CCW,GPIO_PIN_RESET)    // output low level

#define FAN_CW_SetHigh()            HAL_GPIO_WritePin(FAN_GPIO,FAN_CW,GPIO_PIN_SET)    // output high level
#define FAN_CW_SetLow()             HAL_GPIO_WritePin(FAN_GPIO,FAN_CW,GPIO_PIN_RESET)    // output low level


#define PTC_PIN     GPIO_PIN_6
#define PTC_GPIO    GPIOA

#define PTC_SetHigh()            HAL_GPIO_WritePin(PTC_GPIO,PTC_PIN ,GPIO_PIN_SET)    // output high level
#define PTC_SetLow()             HAL_GPIO_WritePin(PTC_GPIO,PTC_PIN ,GPIO_PIN_RESET)    // output low level

#define BUZZER_PIN      GPIO_PIN_7
#define BUZZER_GPIO     GPIOA

#define BUZZER_SetHigh()            HAL_GPIO_WritePin(BUZZER_GPIO,BUZZER_PIN,GPIO_PIN_SET)    // output high level
#define BUZZER_SetLow()             HAL_GPIO_WritePin(BUZZER_GPIO,BUZZER_PIN,GPIO_PIN_RESET)    // output low level

#define PLASMA_PIN      GPIO_PIN_3
#define PLASMA_GPIO     GPIOA

#define PLASMA_SetHigh()            HAL_GPIO_WritePin(PLASMA_GPIO,PLASMA_PIN,GPIO_PIN_SET)    // output high level
#define PLASMA_SetLow()             HAL_GPIO_WritePin(PLASMA_GPIO,PLASMA_PIN,GPIO_PIN_RESET)    // output low level




void FAN_CCW_RUN(void);
void FAN_Stop(void);

void AI_Function_OnOff(uint8_t sel);



#endif 