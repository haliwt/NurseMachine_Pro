#include "key.h"

static void Delay_I2C(uint8_t t);
static uint8_t I2C_Receive8Bit(void);
static void I2C_Stop(void);
static uint8_t Send_OneByte_Ack(uint8_t dat);
static void I2C_Respond(uint8_t ack);





static void Delay_I2C(uint8_t t)
{
  uint8_t a;
  for(a=t;a>0;a--);
}


void I2C_Start(void)
{
  I2C_SDA_IO_OUT();
  I2C_SDA_SetHigh();
  I2C_SCL_SetHigh();
  Delay_I2C(1);
  I2C_SDA_SetLow();
  Delay_I2C(1);
  I2C_SCL_SetLow();
  Delay_I2C(1);
}
#if 0
void I2C_ACK(void)
{
   I2C_SCL_SetLow();
   I2C_SDA_IO_OUT();
   I2C_SDA_SetHigh();
   Delay_I2C(1);
   I2C_SCL_SetHigh();
   Delay_I2C(1);
   I2C_SDA_SetLow();
   Delay_I2C(1);
}
#endif 

void I2C_Stop(void)
{
   I2C_SDA_IO_OUT();
   I2C_SDA_SetLow();
   I2C_SCL_SetLow();
   Delay_I2C(1);
   I2C_SDA_SetHigh();
   Delay_I2C(1);
   I2C_SCL_SetHigh();
   Delay_I2C(1);

}

/************************************************************
*
*Function Name: uint8_t uint8_t Send_OneByte_Ack(uint8_t dat)
*Function : I2C to send one byte data
*Input Ref: send data
*Return Ref: 0 -fail 1 -success
*
************************************************************/
uint8_t Send_OneByte_Ack(uint8_t dat)
{
    uint8_t i;
    I2C_SDA_IO_OUT();
    for(i=0;i<8;i++){
        if(dat & 0x80){
           I2C_SDA_SetHigh();
        }
        else{
           I2C_SDA_SetLow();
        }
        Delay_I2C(1);
        I2C_SCL_SetHigh();
        Delay_I2C(1);
        //I2C_SCL_SetLow();
        
        dat<<=1;   //MSB the first ahead ,LSB the last end
    }
    I2C_SCL_SetLow();
    
    Delay_I2C(3);
    I2C_SDA_IO_IN();
    Delay_I2C(3);
    I2C_SCL_SetHigh();
    Delay_I2C(1);
    i=250;
    while(i--){
        
        if(I2C_SDA_ReadData()==0){
              I2C_SCL_SetLow();
              return 0;
        }
        
    }
    
    I2C_SCL_SetLow();
    return 1;
    
}

/************************************************************
*
*Function Name: void I2C_Respond(uint8_t ack)
*Function : I2C to send one byte data
*Input Ref: acknowledge  - 0 ,don't acknowledge -- 1
*Return Ref: NO
*
************************************************************/
void I2C_Respond(uint8_t ack)
{
    I2C_SDA_IO_OUT();
    I2C_SDA_SetLow();
    I2C_SCL_SetLow();
    if((ack & 0x01) == 0x01) I2C_SDA_SetHigh();
    else I2C_SDA_SetLow();
    
    Delay_I2C(1);
    I2C_SCL_SetHigh();
    Delay_I2C(1);
    I2C_SCL_SetLow();
}

/************************************************************
*
*Function Name: uint8_t I2C_Receive8Bit(void)
*Function : I2C to send one byte data
*Input Ref: acknowledge  - 0 ,don't acknowledge -- 1
*Return Ref: NO
*
************************************************************/
uint8_t I2C_Receive8Bit(void)
{
   uint8_t i,buffer;
   I2C_SDA_IO_IN();
   I2C_SCL_SetLow();
   for(i=0;i<8;i++){
       Delay_I2C(1);
       I2C_SCL_SetHigh();
       buffer = (buffer << 1)|I2C_SDA_ReadData();
       Delay_I2C(1);
       I2C_SCL_SetLow();
   }

   return buffer;

}
/*********************************************************************************************
*
*Function Name: uint8_t I2C_SimpleRead_From_Device(uint16_t *dat16)
*Function : I2C to send one byte data
*Input Ref: device address= 0x40 ,device register address =0x08,0x09 , read data is dat16
*Return Ref: 0--fail  1 - success
*
*********************************************************************************************/
uint8_t I2C_SimpleRead_From_Device(uint8_t *dat8)
{
   uint8_t buf1,buf2;
    
   I2C_Start();
   if(Send_OneByte_Ack(SC12B_ADDR << 1)| 0x01){
            
          I2C_Stop();
          return 0;
   }
   
   buf1 = I2C_Receive8Bit(); //reg address = 0x08
   I2C_Respond(0); //acknowledge
   
   buf2 = I2C_Receive8Bit(); //reg address = 0x09
   I2C_Respond(1); //don't ack
   
   I2C_Stop();
   
   //*dat16 = ((uint16_t)buf1) <<8 | buf2; //
    *dat8 = buf1;
   
   return 1;
   
}
/*********************************************************************************************
*
*Function Name: uint8_t I2C_Read_From_Device(uint8_t reg, uint16_t *dat16)
*Function : I2C to send one byte data
*Input Ref: device address =0x40 ,device register address , read data is dat16
*Return Ref: 0--fail  1 - success
*
*********************************************************************************************/
uint8_t I2C_Read_From_Device(uint8_t reg, uint16_t *dat16)
{
   uint8_t buf1,buf2;
    
   I2C_Start();
   if(Send_OneByte_Ack( SC12B_ADDR << 1) & ~0x01){
            
          I2C_Stop();
          return 0;
   
   }
   
   if(Send_OneByte_Ack(reg)){
        I2C_Stop();
        return 0;
   
   }
   
   I2C_Stop();
   I2C_Start();
   
   if(Send_OneByte_Ack(SC12B_ADDR << 1)|0x01){
            
          I2C_Stop();
          return 0;
   
   }
   
   buf1 = I2C_Receive8Bit();
   I2C_Respond(0); //acknowledge
   
   buf2 = I2C_Receive8Bit();
   I2C_Respond(1); //don't ack
   
   I2C_Stop();
   
   *dat16 = ((uint16_t)buf1) <<8 | buf2;
   
   return 1;
}




