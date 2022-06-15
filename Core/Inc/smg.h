#ifndef __SMG_H__
#define __SMG_H__
#include "main.h"


//IO��������
#define TM1640_DIN_IO_IN()    {GPIOB->MODER&=0XFCFFFFFF;GPIOB->MODER|=0<<24;}  //0x00 input mode
#define TM1640_DIN_IO_OUT()   {GPIOB->MODER&=0XFCFFFFFF;GPIOB->MODER|=1<<24;}   //0x01 output  mode 

#define TM1640_DIN    GPIO_PIN_12
#define TM1640_CLK    GPIO_PIN_11
#define TM1640_GPIO   GPIOB 

#define TM1640_CLK_SetHigh()            HAL_GPIO_WritePin(TM1640_GPIO,TM1640_CLK,GPIO_PIN_SET)    // ����ߵ�ƽ
#define TM1640_CLK_SetLow()             HAL_GPIO_WritePin(TM1640_GPIO,TM1640_CLK,GPIO_PIN_RESET)  // ����͵�ƽ

#define TM1640_DIN_SetHigh()            HAL_GPIO_WritePin(TM1640_GPIO,TM1640_DIN,GPIO_PIN_SET)    // ����ߵ�ƽ
#define TM1640_DIN_SetLow()             HAL_GPIO_WritePin(TM1640_GPIO,TM1640_DIN,GPIO_PIN_RESET)    // output low level


//extern volatile uint8_t DispData[3];//??LED??
#define		BitSET(x,y)		x|=(1<<y)				//?1
#define		BitCLR(x,y)		x&=~(1<<y)				//?0


		
#define AddrAutoAdd        0x40// Auto add one address 
#define AddrFixed 	       0x44// 
 

#define OpenDispTM1640          0x88//
#define CloseDispTM1640		    0x80// 

 
 //Display Address 
#define Addr00H  		0xC0//??00H
#define Addr01H  		0xC1//??01H
#define Addr02H  		0xC2//??02H
#define Addr03H  		0xC3//??03H
#define Addr04H  		0xC4//??03H
#define Addr05H  		0xC5//??03H
#define Addr06H  		0xC6//??00H
#define Addr07H  		0xC7//??01H
#define Addr08H  		0xC8//??02H
#define Addr09H  		0xC9//??03H
#define Addr0AH  		0xCA//??03H
#define Addr0BH  		0xCB//??03H
#define Addr0CH         0xCC//??0CH
#define Addr0DH         0xCD//??0DH
#define Addr0EH         0xCE//??0CH
#define Addr0FH         0xCF//??0DH



#define Set1_16TM1640           0X88

#define Set14_16TM1640 		    0X8F//??????? 14/16 

    
#define   seg_a  0x01      //SEG1   //seg_e = 0x10  
#define   seg_b  0x02      //SEG2 , //seg_f = 0x20
#define   seg_c  0x04      //SEG3 , //seg_g = 0x40
#define   seg_d  0x08      //SEG4 , //seg_d = 0x08
#define   seg_e  0x10      //SEG5 , //seg_c = 0x04
#define   seg_f  0x20      //SEG6   //seg_a = 0x01
#define   seg_g  0x40      //SEG7   //seg_b = 0x02 
#define   seg_h  0x80      //SEG8 -> ":"



void TM1640_Write_4Bit_Data(uint8_t onebit,uint8_t twobit,uint8_t threebit,uint8_t fourbit,uint8_t sl);
void TM1640_Write_2bit_TempData(uint8_t onebit,uint8_t twobit);
void TM1640_Write_2bit_HumData(uint8_t onebit,uint8_t twobit);
void TM1640_TimeLed_OnOff(uint8_t sel);


void LED_Power_OnOff(uint8_t sel);
void LED_MODE_On(void);
void LED_TempHum_On(void);
void LED_Fan_OnOff(uint8_t sel);
void LED_Sterilizer_OnOff(uint8_t sel);
void LED_Dry_OnOff(uint8_t sel);
void LED_AI_OnOff(uint8_t sel);

void Breath_Led(void);
void Smg_AllOff(void);

#endif 

