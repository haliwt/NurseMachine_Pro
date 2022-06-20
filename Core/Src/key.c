#include "key.h"

static void Delay_I2C(uint8_t t);
static uint8_t I2C_Receive8Bit(void);
static void I2C_Stop(void);
static uint8_t Send_OneByte_Ack(uint8_t dat);
static void I2C_Respond(uint8_t ack);
static void I2C_SDA_IO_IN(void) ;
static void I2C_SDA_IO_OUT(void);

static void I2C_SDA_IO_IN(void) 	//PB11配置成输入  
{  
	//__HAL_RCC_GPIOB_CLK_ENABLE();//GPIO时钟使能
GPIO_InitTypeDef GPIO_InitStruct = {0};
GPIO_InitStruct.Pin = I2C_SDA ;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
} 

static void I2C_SDA_IO_OUT(void)//PB11配置成开漏输出
{
	//__HAL_RCC_GPIOB_CLK_ENABLE();//GPIO时钟使能
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = I2C_SDA ;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP ;//GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}

static void Delay_I2C(uint8_t t)
{
  uint16_t j;
	for(j=0;j<t;j++)
	{
       for(int i = 0; i <30; i++)//better for(int i = 0; i < 40; i++)    //for(int i = 0; i < 20; i++)    
        {
            __asm("NOP");//等待1个指令周期，系统主频16M
           
        }
	}
}


void I2C_Start(void)
{
  I2C_SDA_IO_OUT();
  I2C_SDA_SetHigh();
  I2C_SCL_SetHigh();
  Delay_I2C(5);
  I2C_SDA_SetLow();
  Delay_I2C(5);
  I2C_SCL_SetLow();
  Delay_I2C(5);
}
#if 1
void I2C_ACK(void)
{
   I2C_SCL_SetLow();
   I2C_SDA_IO_OUT();
   I2C_SDA_SetLow();
   Delay_I2C(1);
   I2C_SCL_SetHigh();
   Delay_I2C(2);
   I2C_SDA_SetLow();
   
}

void I2C_NACK(void)
{
   I2C_SCL_SetLow();
   I2C_SDA_IO_OUT();
   I2C_SDA_SetHigh();
   Delay_I2C(1);
   I2C_SCL_SetHigh();
   Delay_I2C(1);
   I2C_SDA_SetLow();
   
}
#endif 

void I2C_Stop(void)
{
   I2C_SDA_IO_OUT();
   I2C_SDA_SetLow();
   I2C_SCL_SetLow();
   Delay_I2C(4);
     I2C_SCL_SetHigh();
   I2C_SDA_SetHigh();
   Delay_I2C(4);
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
    uint8_t i,temp;
    I2C_SDA_IO_OUT();
    for(i=0;i<8;i++){
        I2C_SCL_SetLow();
         Delay_I2C(1);
         temp = (dat >>7)&0x01;
        if(temp & 0x01){
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
    uint8_t buf1;
    
   I2C_Start();
   if(Send_OneByte_Ack(SC12B_ADDR << 1)| 0x01){
            
          I2C_Stop();
          return 0;
   }
   
   buf1 = I2C_Receive8Bit(); //reg address = 0x08
   I2C_Respond(0); //acknowledge
   
    I2C_Receive8Bit(); //reg address = 0x09
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


/***************************************************************************************************************************
SC系列B版本芯片 简易读取按键值函数（默认直接读取）
此函数只有初始化配置默认的情况下，直接调用，如果在操作前有写入或者其他读取不能调用默认
**********************************************************************************************************************************/
Complete_Status I2C_Simple_Read_From_Device(unsigned char deviceAddr,unsigned char* target,unsigned char len)
{
			unsigned char i;
			unsigned char *p;
			p = target;
			I2C_Start();
			if (Send_OneByte_Ack((deviceAddr<<1) | 0x01)) {
					I2C_Stop();
					return UNDONE;
			}
			for(i = 0; i < len; i++)
			{
			 		*p= I2C_Receive8Bit();
			 		 p++;
				 	if(i < len-1)
			  			I2C_Respond(0);
			 		else 
			 	 			I2C_Respond(1);
			}
			I2C_Stop();
			return DONE;
}
/*******************************************************************************************************************************
* SC系列B版本芯片
deviceAddr 设置器件地址, REG 设置寄存器地址, target 读取地址对应数据内容。
**************************************************************************************************************************************/
Complete_Status I2C_Read_From_Device(unsigned char deviceAddr,unsigned char REG,unsigned char* target,unsigned char len)
{
	  unsigned char i;
		unsigned char *p;
	  p = target;
		I2C_Start();
		if (Send_OneByte_Ack((deviceAddr<<1) & ~0x01)) {
				I2C_Stop();
				return UNDONE;
		}
		if (Send_OneByte_Ack(REG)) {
			I2C_Stop();
			return UNDONE;
		}
		I2C_Stop();
		I2C_Start();
		if (Send_OneByte_Ack((deviceAddr<<1) | 0x01)) {
			I2C_Stop();
			return UNDONE;
		}
		for(i = 0; i < len; i++)
		{
			 	*p= I2C_Receive8Bit();
			 	p++;
				if(i < len-1)
			  	 I2C_Respond(0);
			  else 
			 	 	 I2C_Respond(1);
		}
		I2C_Stop();
		return DONE;
}


