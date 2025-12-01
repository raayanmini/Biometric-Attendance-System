/**************************************************************************************
* File : systemticktimer.c
* Description : This file contains the function definitions of SysTick Timer to generate
                delays (in msec, usec)
								
***************************************************************************************/


#include "stm32f401rbt6.h"
#include "systemticktimer.h"

volatile unsigned  int Tick = 0;
volatile int timer_counter = 0;  //Global timer variable



/***********************************SysTick_Init()********************************/

void SysTick_Init(int delay)
{
	STK_CTRL |= 0x04;                   //select clock source as 16MHz
	STK_CTRL |= (0x01 << 1);            //set 1st bit of STK_CTRL to configure it to Interrupt mode
	switch(delay)
	{
		case MDELAY :
			STK_LOAD = 16000;               //load 16000 into STK_LOAD to configure delay in milli sec
			break;
		case UDELAY :
			STK_LOAD = 16;                  //load 16 into STK_LOAD to configure delay in micro sec
			break;
	}	
	STK_VAL = 0x123;                    //load some value into STK_VAL to clear field to 0 and COUNTFLG bit in STK_CTRL register to 0	
  STK_CTRL |= 0x01 ;                  //enable The SysTick timer	
	
}

/******************************************INTERRUPT HANDLER***********************************************/

void SysTick_Handler(void) 
{
  if(Tick == MAX_UINT_32)          //Check if the "Tick" variable has reached the maximum value for an unsigned 32-bit integer.
  {
      Tick = 0;                    // Reset the "Tick" counter if it reaches the maximum value (overflow)
  }
  Tick++;                          //Increment the "Tick" variable by 1. This could represent one tick of time.  
  timer_counter++;	
}



/********************************************KM_mdelay()*************************************/

void KM_Delay_ms(unsigned int delay)
{  	
	unsigned int delay_limit;
  if((Tick + delay) >= MAX_UINT_32)
  {
    Tick = 0;
  }
  delay_limit = Tick + delay;
  
	while(Tick < delay_limit);
}


/**************************************************KM_udelay()******************************/

void KM_Delay_us(unsigned int delay)
{  
	unsigned int delay_limit;
  if((Tick + delay) >= MAX_UINT_32)
  {
    Tick = 0;
  }
  delay_limit = Tick + delay;
  while(Tick < delay_limit);
}

