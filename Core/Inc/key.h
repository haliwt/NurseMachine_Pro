#ifndef __KEY_H__
#define __KEY_H__
#include  "main.h"

#define SC12B_ADDR     0x40

//IO·½ÏòÉèÖÃ
#define I2C_SDA_IO_IN()      {GPIOB->MODER&=0XFFFF3FFF;GPIOB->MODER|=0<<14;}  //0x00 input mode
#define I2C_SDA_IO_OUT()     {GPIOB->MODER&=0XFFFF3FFF;GPIOB->MODER|=1<<14;}   //0x01 output  mode 




#define I2C_SDA         GPIO_PIN_14
#define I2C_SCL         GPIO_PIN_13
#define I2C_GPIO        GPIOB


#define I2C_SDA_SetHigh()            HAL_GPIO_WritePin(I2C_GPIO,I2C_SDA,GPIO_PIN_SET)    // output high level
#define I2C_SDA_SetLow()             HAL_GPIO_WritePin(I2C_GPIO,I2C_SDA,GPIO_PIN_RESET)    // output low level

#define I2C_SCL_SetHigh()            HAL_GPIO_WritePin(I2C_GPIO,I2C_SCL,GPIO_PIN_SET)    // output high level
#define I2C_SCL_SetLow()             HAL_GPIO_WritePin(I2C_GPIO,I2C_SCL,GPIO_PIN_RESET)    // output low level

#define I2C_SDA_ReadData()           HAL_GPIO_ReadPin(I2C_GPIO,I2C_SDA)




#define SenSet0_REG                  0x00    //CIN4 channel sensitivity of address
#define SenSetCom_REG                0x01    //others channel CINx sensitivity  of address 
#define CTRL0_REG                    0x02    //CTRL0 control register setup of address
#define CTRL1_REG                    0x03    //CTRL1  control register setup of address
#define OUPUT_REG                    0x08    //output register state output of address
#define SAMP_REG                     0x0A    //touch data be save value output address 

#define RTM0                         0 
#define RTM1                         1
#define RTM2                         2
#define RTM3                         3


#define KVF_STOP_CORREC              (1<<2)  //touch is avail ,touch don't need calibration 
#define KVF_50S_CORREC              (0<<2)  //touch is avail ,50s start  calibration
#define HOLD             (1<<3)  //base line  ,don't need  calibration
#define NOTHOLD         (0<<3)    //base line ,continue calibration
#define SLPCYC_LGT       (0<<5)   //infint maxmium 
#define SLPCYC_0R5T       (1<<5)   //to sleep for sample interval 60ms 
#define SLPCYC_0R5T       (1<<5)   //to sleep for sample interval 60ms 
#define SLPCYC_5R5T       (6<<5)   //to sleep for sample interval 660ms 
#define SLPCYC_6R5T       (7<<5)   //to sleep for sample interval 780ms 

#define FAST_T0_SLEEP      (1<<4)    //fast input sleep 





uint8_t I2C_SimpleRead_From_Device(uint8_t *dat8);

uint8_t I2C_Read_From_Device(uint8_t reg, uint16_t *dat16);

#endif 


