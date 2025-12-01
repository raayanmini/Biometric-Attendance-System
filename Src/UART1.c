/**************************************************************************************
* File : UART1.c
* Description : This file contains the function definitions of UART1
***************************************************************************************/

#include "stm32f401rbt6.h"
#include "UART1.h"

/**************************************************UART1_Init()****************************************************/

void UART1_Init(void)
{
	RCC_AHB1ENR |= (0x01 << 0);           //set 0th bit of  RCC_AHB1ENR to Enable Port A clock

  GPIOA_MODER &= (0xFFC3FFFF);          //clear 21-18 bits of   GPIOA_MODER
	GPIOA_MODER |= (0x00280000);          /*Load "10" into 21-20,19-18 bit fields of GPIOA_MODER to configure
	                                         PA10,PA9 as Alternate Function pins*/
	
	GPIOA_AFRH &= 0xFFFFF00F;             //clear 11-4 bit fields of GPIOA_AFRH
	GPIOA_AFRH |= 0x00000770;             /*load "0111" (AF07) into 11-8,7-4 bit fields of GPIOA_AFRH to enable 
	                                        PA10 as USART1_RX,PA9 as USART1_TX  functionality pins*/
	
	RCC_APB2ENR |= (0x01 << 4);          // set 4th bit (USART1EN) of RCC_APB2ENR to enable USART1 clock
}

/************************************************UART1_Config()***********************************************/

void UART1_Config(void)
{
	USART1_BRR = 0x683;                  //load 0x683 into USART1_BRR to set baud rate of 9600 8N1.
	USART1_CR1 |= (0x01 << 13);          //set 13th bit of  USART1_CR1 to enable USART1
	USART1_CR1 |= (0x01 << 3);           //set 3rd bit of  USART1_CR1 to enable Transmitter
	USART1_CR1 |= (0x01 << 2);           //set 2nd bit of  USART1_CR1 to enable Receiver
}

/***********************************************UART1_OutChar()*********************************************/

void UART1_OutChar(unsigned char ch)
{
	while(!(USART1_SR & (0x01 << 7)));   //checking 7th(TXE) of USART1_SR is 0(empty)

	USART1_DR = ch;	                     //loading ch value to USART1_DR
}

/***************************************************UART1_OutStr()*********************************************/

void UART1_OutStr(const unsigned char str[],unsigned int length)
{
	unsigned int i=0;
	while(i < length)
	{
		UART1_OutChar(str[i]);
		i++;
	}
}

/***************************************************USART1_InChar_TIMEOUT()****************************************/

int UART1_InChar_TIMEOUT (unsigned int iteration_count)
{ 
	while(iteration_count)                 //loop until iteration_count=0
	{
 		if((USART1_SR & (0x01 << 5)))        //checking 5th(RXNE) bit of USART1_SR is 0(empty) 
	  {
			return (0xFF & USART1_DR);         //returning to main (0xFF & USART1_DR)
	  }	 
		iteration_count--;
	}	
	return TIMEOUT;                        //returning to main (TIMEOUT)
}
