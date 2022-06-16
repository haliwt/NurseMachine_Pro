#include "smg.h"




#define DOUBLEDOT       seg_h

#define POWERLED        (seg_g + seg_f)
#define MODELED         (seg_b+seg_c+seg_d+seg_e+seg_f)

#define TEMPLED         (seg_g + seg_f+ seg_e)
#define FANLED          (seg_g + seg_f)
#define STERLED         (seg_g + seg_f)
#define DRYLED          (seg_g + seg_f)
#define AILED           (seg_g + seg_f)
#define HOUR            (seg_b + seg_c + seg_e+ seg_f + seg_g)

#define TIMELED         (seg_e)
#define OFFLED           0


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
void Bdelay_us(uint16_t t);
/*****************************************************
 * 
 * 
 * 
 * 
******************************************************/
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
		   HAL_Delay(1); //5ms
	 }
	 else{
	     TM1640_DIN_SetLow();
		   HAL_Delay(1); //5ms
	 
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
*Input Ref: onebit ,twobit hours ,threebit fourbit minute,sl -select "H" or "numbers"
*Return Ref: NO
    *
********************************************************************************************************/
void TM1640_Write_4Bit_Data(uint8_t onebit,uint8_t twobit,uint8_t threebit,uint8_t fourbit,uint8_t sl)
{
     TM1640_Start();
     TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xC0);//0xC0H->GRID1->BIT_1
     if(sl ==0)
         TM1640_Write_OneByte(segNumber[onebit]);//display "1"
     else if(sl==1){
        TM1640_Write_OneByte(HOUR);//display "H"
     }
     else if(sl==2){ //turn off SMG display
        TM1640_Write_OneByte(OFFLED);//display "NULL"
     }
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xC1);//0xC1H->GRID1->BIT_2
     if(sl==0){
         TM1640_Write_OneByte(segNumber[twobit]|DOUBLEDOT);//display "2 :"
         // TM1640_Write_OneByte(DOUBLEDOT);//display ":"
     }
     else if(sl ==1){
          TM1640_Write_OneByte(segNumber[0]);//display "0"  
      }
     else if(sl==2){
        TM1640_Write_OneByte(OFFLED);//display "NULL"
     }
     TM1640_Stop();
    
    //diplay ":"
   // TM1640_Start();
  //  TM1640_Write_OneByte(0xC1);//0xC1H->GRID1->BIT_2
  //  if(sl == 2) TM1640_Write_OneByte(OFFLED);//display "NULL"
  //  else TM1640_Write_OneByte(DOUBLEDOT);//display ":"
  //  TM1640_Stop();
    
    //minute 
    TM1640_Start();
    TM1640_Write_OneByte(0xC2);//0xC2H->GRID3->BIT_3
    if(sl==2)TM1640_Write_OneByte(OFFLED);//display "NULL"
    else TM1640_Write_OneByte(segNumber[threebit]);//display ""
    TM1640_Stop();
    
    //minute 
    TM1640_Start();
    TM1640_Write_OneByte(0xC3);//0xC2H->GRID3->BIT_3
    if(sl==2)TM1640_Write_OneByte(OFFLED);//display "NULL"
    else TM1640_Write_OneByte(segNumber[fourbit]);//display ""
    TM1640_Stop();
    
    //open diplay
    TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640|0x8f);//0xC2H->GRID3->BIT_3
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
    TM1640_Write_OneByte(OpenDispTM1640|0x8f);//
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
    TM1640_Write_OneByte(OpenDispTM1640|0x8f);//
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
void LED_Power_OnOff(uint8_t sel)
{
     
     TM1640_Start();
     TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xCE);//0xC4H->GRID15
     if(sel==0)
         TM1640_Write_OneByte(POWERLED);//display "power Key"
     else 
          TM1640_Write_OneByte(OFFLED);//display "NULL"
     TM1640_Stop();

    //open diplay 
    TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640|0x80);//
    TM1640_Stop();
}

void KeyLed_Power_On(void)
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
    TM1640_Write_OneByte(OpenDispTM1640|0x8F);//
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
void LED_TempHum_On(void)
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
    TM1640_Write_OneByte(OpenDispTM1640|0X8F);//
    TM1640_Stop();



}

void LED_MODE_On(void)
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
    TM1640_Write_OneByte(OpenDispTM1640|0x8F);//
    TM1640_Stop();
}

void LED_Fan_OnOff(uint8_t sel)
{
     TM1640_Start();
     TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xCD);//0xCDH->GRID14
     if(sel==0)
        TM1640_Write_OneByte(FANLED);//display "FAN led"
     else
        TM1640_Write_OneByte(OFFLED);//display "FAN led"
     TM1640_Stop();

    //open diplay
 
    TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640|0X8F);//
    TM1640_Stop();



}
void LED_Sterilizer_OnOff(uint8_t sel)
{
    TM1640_Start();
    TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xCC);//0xCCH->GRID13
     if(sel==0)
       TM1640_Write_OneByte(STERLED);//display "temperature led"
     else 
        TM1640_Write_OneByte(OFFLED);//display "NULL"
     TM1640_Stop();

    //open diplay
   TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640|0X8F);//
    TM1640_Stop();
}
void LED_Dry_OnOff(uint8_t sel)
{
     TM1640_Start();
    TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xCB);//0xCBH->GRID12
     if(sel==0)
        TM1640_Write_OneByte(DRYLED);//display "temperature led"
     else 
        TM1640_Write_OneByte(OFFLED);//display "temperature led"
     TM1640_Stop();

    //open diplay
   TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640|0X8F);//
    TM1640_Stop();


}
void LED_AI_OnOff(uint8_t sel)
{
   TM1640_Start();
    TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xCA);//0xCAH->GRID11
     if(sel==0)
           TM1640_Write_OneByte(AILED);//display "temperature led"
     else{
        TM1640_Write_OneByte(OFFLED);//display "NULL"
     }
     TM1640_Stop();

    //open diplay
    TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640|0X8F);//
    TM1640_Stop();


}

void TM1640_TimeLed_OnOff(uint8_t sel)
{
    TM1640_Start();
     TM1640_Write_OneByte(AddrFixed);//Add fixed reg
     TM1640_Stop();
    
     TM1640_Start();
     TM1640_Write_OneByte(0xC9);//0xC9H->GRID10
     if(sel==0)
         TM1640_Write_OneByte(TIMELED);//display "time led"
     else{
         TM1640_Write_OneByte(OFFLED);//display "time led"
     }
     TM1640_Stop();

    //open diplay
    TM1640_Start();
    TM1640_Write_OneByte(OpenDispTM1640|0X8F);//
    TM1640_Stop();

}


/**
 * @brief 
 * 
 */
void Bdelay_us(uint16_t t)
{
  //__IO uint32_t Delay = udelay * 72 / 8;//(SystemCoreClock / 8U / 1000000U)
    //?stm32f1xx_hal_rcc.c -- static void RCC_Delay(uint32_t mdelay)
    uint16_t j;
	for(j=0;j<t;j++)
	{
       for(int i = 0; i < 50; i++)//better for(int i = 0; i < 40; i++)    //for(int i = 0; i < 20; i++)    
        {
            __asm("NOP");//等待1个指令周期，系统主频16M
            __asm("NOP");//等待1个指令周期，系统主频16M
            __asm("NOP");//等待1个指令周期，系统主频16M
        }
	}

}




void Breath_Led(void)
{
    
    LED_Power_OnOff(0);
    #if 0
    if(powerLed ==0){
        for(i=0;i<10;i++){
            LED_Power_OnOff(0); //POWER LED ON 
            //open diplay 
            TM1640_Start();
            TM1640_Write_OneByte(OpenDispTM1640|0x80);//
            TM1640_Stop();
            Bdelay_us(t);

            TM1640_Start();
            TM1640_Write_OneByte(OpenDispTM1640|0x81);//
            TM1640_Stop();
             Bdelay_us(t);

            TM1640_Start();
            TM1640_Write_OneByte(OpenDispTM1640|0x82);//
            TM1640_Stop();
             Bdelay_us(t);

              //open diplay 

            TM1640_Start();
            TM1640_Write_OneByte(OpenDispTM1640|0x82);//
            TM1640_Stop();
            Bdelay_us(30001-t)  ;
  

            TM1640_Start();
            TM1640_Write_OneByte(OpenDispTM1640|0x81);//
            TM1640_Stop();
             Bdelay_us(30001-t)  ;

            TM1640_Start();
            TM1640_Write_OneByte(OpenDispTM1640|0x80);//
            TM1640_Stop();
             Bdelay_us(30001-t)  ;


             TM1640_Start();
             TM1640_Write_OneByte(CloseDispTM1640);//
             TM1640_Stop();
            Bdelay_us(30001-t)  ;
        }
     }
     t++;
     if(t==30000){
        powerLed = 1;
     }

     if(powerLed==1){
         for(i=0;i<40;i++){
           // LED_Power_OnOff(0); //POWER LED ON 
            TM1640_Start();
            TM1640_Write_OneByte(OpenDispTM1640|0x82);//
            TM1640_Stop();
            Bdelay_us(t);

              TM1640_Start();
            TM1640_Write_OneByte(OpenDispTM1640|0x81);//
            TM1640_Stop();
             Bdelay_us(t);

              TM1640_Start();
            TM1640_Write_OneByte(OpenDispTM1640|0x80);//
            TM1640_Stop();
            Bdelay_us(t);

            Bdelay_us(t);
            TM1640_Start();
            TM1640_Write_OneByte(CloseDispTM1640);//
            TM1640_Stop();
            Bdelay_us(t) ;
           //LED_Power_OnOff(1); //POWER LED OFF

             Bdelay_us(t);
            TM1640_Start();
            TM1640_Write_OneByte(CloseDispTM1640);//
            TM1640_Stop();
           Bdelay_us(30001-t)  ;

             TM1640_Start();
            TM1640_Write_OneByte(OpenDispTM1640|0x80);//
            TM1640_Stop();
            Bdelay_us(30001-t)  ;

            TM1640_Start();
            TM1640_Write_OneByte(OpenDispTM1640|0x81);//
            TM1640_Stop();
             Bdelay_us(30001-t)  ;

            TM1640_Start();
            TM1640_Write_OneByte(OpenDispTM1640|0x82);//
            TM1640_Stop();
            



           Bdelay_us(30001-t)  ;

         }
         t--;
         if(t==1) powerLed =0;

     }
    #endif 
    
}

void Smg_AllOff(void)
{
    //close diplay
    TM1640_Start();
    TM1640_Write_OneByte(CloseDispTM1640);//
    TM1640_Stop();

}




