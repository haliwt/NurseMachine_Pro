#include "smg.h"


#define DOUBLEDOT       seg_h

#define POWERLED        (seg_g + seg_f)
#define MODELED         (seg_b+seg_c+seg_d+seg_e+seg_f)

#define TEMPLED         (seg_g + seg_f+ seg_e)
#define FANLED          (seg_g + seg_f)
#define STERLED         (seg_g + seg_f)
#define DRYLED          (seg_g + seg_f)
#define AILED           (seg_g + seg_f)




const unsigned char segNumber[]={
	
     
		 seg_a+seg_b+seg_c+seg_d+seg_e+seg_f,        		// char "0"  0x00
		 seg_b+seg_c,                           			// char "1"  0x01
		 seg_a+seg_b+seg_d+seg_e+seg_g,              		// char "2"  0x02
		 seg_a+seg_b+seg_c+seg_d+seg_g,               		// char "3"  0x03
		 seg_b+seg_c+seg_f+seg_g,                   	    // char "4"  0x04
		 seg_a+seg_c+seg_d+seg_f+seg_g,              		// char "5"  0x05
		 seg_a+seg_c+seg_d+seg_e+seg_f+seg_g,              	// char "6"  0x06
		 seg_a+seg_b+seg_c+seg_f,                    		// char "7"  0x07
		 seg_a+seg_b+seg_c+seg_d+seg_e+seg_f+seg_g,  		// char "8"  0x08
		 seg_a+seg_b+seg_c+seg_d+seg_f+seg_g,        		// char "9"  0x09
		 seg_a+seg_b+seg_c+seg_e+seg_f+seg_g,               // char "A"  0x0A
		 seg_c+seg_d+seg_e+seg_f+seg_g,                     // char "b"  0x0B
		 seg_a+seg_d+seg_e+seg_f,							// char "C"  0X0C
		 seg_b+seg_c+seg_d+seg_e+seg_g,						// char "d"  0x0D
		 seg_a+seg_d+seg_e+seg_f+seg_g,                     // char "E"  0X0E
		 seg_a+seg_e+seg_f+seg_g,                           // char "F"  0X0F
         0                                      			// char "NLL"  0x16
                                               
                                             
};


static void TM1640_Start(void);
static void TM1640_Stop(void);
static void TM1640_Write_OneByte(uint8_t data);

static void TM1640_Start(void)
{
   TM1640_DIN_SetHigh() ;
	 TM1640_CLK_SetHigh();
	 HAL_Delay(5); //5ms
	 TM1640_DIN_SetLow();
	 HAL_Delay(5);
	 TM1640_CLK_SetLow();
	  HAL_Delay(5);
	
}
static void TM1640_Stop(void)
{
   TM1640_CLK_SetLow();
	 TM1640_DIN_SetLow();
	 HAL_Delay(5);
	 TM1640_CLK_SetHigh();
	 HAL_Delay(5);
	 TM1640_DIN_SetHigh();
	 HAL_Delay(5);

}

static void TM1640_Write_OneByte(uint8_t data)
{
	uint8_t i; 
	
	for(i=8;i>0;i--){
		
	TM1640_CLK_SetLow();
	 if(data & 0x01){
	     TM1640_DIN_SetHigh();
		   HAL_Delay(5); //5ms
	 }
	 else{
	     TM1640_DIN_SetLow();
		   HAL_Delay(5); //5ms
	 
	 }
	 TM1640_CLK_SetHigh();
	 data = data >> 1;   //LSB is the first send 
 }
 HAL_Delay(5);
}
/*******************************************************************************************************
    *
*Function Name:void TM1640_Write_4Bit_Data(uint8_t onebit,uint8_t twobit,uint8_t threebit,uint8_t fourbit)
*Function :Smg display times hour minute
*Input Ref: onebit ,twobit hours ,threebit fourbit minute
*Return Ref: NO
    *
********************************************************************************************************/
void TM1640_Write_4Bit_Data(uint8_t onebit,uint8_t twobit,uint8_t threebit,uint8_t fourbit)
{
     TM1640_Start();
     TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xC0);//0xC0H->GRID1->BIT_1
     TM1640_Write_OneByte(segNumber[onebit]);//display "1"
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xC1);//0xC1H->GRID1->BIT_2
     TM1640_Write_OneByte(segNumber[twobit]);//display "2"
     TM1640_Stop();
    
    //diplay ":"
    TM1640_Start();
    TM1640_Write_OneByte(0xC1);//0xC1H->GRID1->BIT_2
    TM1640_Write_OneByte(DOUBLEDOT);//display ":"
    TM1640_Stop();
    
    //minute 
    TM1640_Start();
    TM1640_Write_OneByte(0xC2);//0xC2H->GRID3->BIT_3
    TM1640_Write_OneByte(segNumber[threebit]);//display ""
    TM1640_Stop();
    
    //minute 
    TM1640_Start();
    TM1640_Write_OneByte(0xC3);//0xC2H->GRID3->BIT_3
    TM1640_Write_OneByte(segNumber[fourbit]);//display ""
    TM1640_Stop();
    
    //open diplay
    
    TM1640_Start();
    TM1640_Write_OneByte(Set14_16TM1640);//display Freq
    TM1640_Stop();
    
    
    TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640);//0xC2H->GRID3->BIT_3
    TM1640_Stop();
    
    
    
    
}
/*******************************************************************************************************
    *
*Function Name:void TM1640_Write_2bit_HumData(uint8_t onebit,uint8_t twobit)
*Function :Smg display humidity of value
*Input Ref: onebit,twobit  value
*Return Ref: NO
    *
********************************************************************************************************/
void TM1640_Write_2bit_HumData(uint8_t onebit,uint8_t twobit)
{
     TM1640_Start();
     TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xC4);//0xC4H->GRID5->BIT_1
     TM1640_Write_OneByte(segNumber[onebit]);//display ""
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xC5);//0xC5H->GRID6->BIT_2
     TM1640_Write_OneByte(segNumber[twobit]);//display ""
     TM1640_Stop();
    
     //open diplay
    TM1640_Start();
    TM1640_Write_OneByte(Set14_16TM1640);//display Freq
    TM1640_Stop();
    
    TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640);//
    TM1640_Stop();
    

}

/*******************************************************************************************************
    *
*Function Name:void TM1640_Write_2bit_TempData(uint8_t onebit,uint8_t twobit)
*Function :Smg display temprature of value
*Input Ref: onebit,twobit  value
*Return Ref: NO
    *
********************************************************************************************************/
void TM1640_Write_2bit_TempData(uint8_t onebit,uint8_t twobit)
{
     TM1640_Start();
     TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xC6);//0xC4H->GRID7->BIT_1
     TM1640_Write_OneByte(segNumber[onebit]);//display ""
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xC7);//0xC7H->GRID8->BIT_2
     TM1640_Write_OneByte(segNumber[twobit]);//display ""
     TM1640_Stop();
    
     //open diplay
    TM1640_Start();
    TM1640_Write_OneByte(Set14_16TM1640);//display Freq
    TM1640_Stop();
    
    TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640);//
    TM1640_Stop();
    
}

/***************************************************************************
    *
*Function Name:void LED_Power(void)
*Function :KEY OF LED 
*Input Ref: NO
*Return Ref: NO
    *
*****************************************************************************/
void LED_Power(void)
{
     
     TM1640_Start();
     TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xCE);//0xC4H->GRID15
     TM1640_Write_OneByte(POWERLED);//display "power Key"
     TM1640_Stop();

    //open diplay
    TM1640_Start();
    TM1640_Write_OneByte(Set14_16TM1640);//display Freq
    TM1640_Stop();
    
    
    TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640);//
    TM1640_Stop();
}
/***************************************************************
*
*Function Name: void LED_TempHum(void)
*Function : display temperature and humidity and times led 
*
*
*
**************************************************************/
void LED_TempHum(void)
{
     TM1640_Start();
     TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xC9);//0xC9H->GRID10
     TM1640_Write_OneByte(TEMPLED);//display "TEMPLED Key"
     TM1640_Stop();

    //open diplay
    TM1640_Start();
    TM1640_Write_OneByte(Set14_16TM1640);//display Freq
    TM1640_Stop();
    
    
    TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640);//
    TM1640_Stop();



}

void LED_MODE(void)
{
     TM1640_Start();
     TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xC8);//0xC8H->GRID9
     TM1640_Write_OneByte(MODELED);//display "MODE Key"
     TM1640_Stop();

    //open diplay
    TM1640_Start();
    TM1640_Write_OneByte(Set14_16TM1640);//display Freq
    TM1640_Stop();
    
    
    TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640);//
    TM1640_Stop();



}

void LED_Fan(void)
{
     TM1640_Start();
     TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xCD);//0xCDH->GRID14
     TM1640_Write_OneByte(FANLED);//display "FAN led"
     TM1640_Stop();

    //open diplay
    TM1640_Start();
    TM1640_Write_OneByte(Set14_16TM1640);//display Freq
    TM1640_Stop();
    
    
    TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640);//
    TM1640_Stop();



}
void LED_Sterilizer(void)
{
    TM1640_Start();
    TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xCC);//0xCCH->GRID13
     TM1640_Write_OneByte(STERLED);//display "temperature led"
     TM1640_Stop();

    //open diplay
    TM1640_Start();
    TM1640_Write_OneByte(Set14_16TM1640);//display Freq
    TM1640_Stop();
    
    
    TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640);//
    TM1640_Stop();
}
void LED_Dry(void)
{
     TM1640_Start();
    TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xCB);//0xCBH->GRID12
     TM1640_Write_OneByte(DRYLED);//display "temperature led"
     TM1640_Stop();

    //open diplay
    TM1640_Start();
    TM1640_Write_OneByte(Set14_16TM1640);//display Freq
    TM1640_Stop();
    
    
    TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640);//
    TM1640_Stop();


}
void LED_AI(void)
{
   TM1640_Start();
    TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xCA);//0xCAH->GRID11
     TM1640_Write_OneByte(AILED);//display "temperature led"
     TM1640_Stop();

    //open diplay
    TM1640_Start();
    TM1640_Write_OneByte(Set14_16TM1640);//display Freq
    TM1640_Stop();
    
    
    TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640);//
    TM1640_Stop();


}

