/**************************************************************************************
* File : Interrupt.c
* Description : This file contains the Function Definitions Related to Interrupts Initializing
                 and Configuring.
***************************************************************************************/


#include "stm32f401rbt6.h"


/*************************************************************************************
*                               Touch_IC_Config()
**************************************************************************************/

void Touch_IC_Config(void)
{
	RCC_AHB1ENR |= (0x01 << 0);          //Set 0th bit of RCC_AHB1ENR to Enable Port A clock
	RCC_APB2ENR |= (0x01 << 14);         //set 14th bit of RCC_APB2ENR to Enable SYSCFG clock
		
	GPIOA_MODER &= ~(0x03 << (6*2));     //clear 13-12 bits of GPIOA_MODER (by default it is in input mode only)
	GPIOA_PUPDR &= ~(0x03 << (6*2));     //clear 13-12 bits of GPIOA_PUPDR
	GPIOA_PUPDR |=  (0x02 << (6*2));     //load "10" into 13-12 bit fields in GPIOA_PUPDR to make PC0 as pull-down
	
	SYSCFG_EXTICR2 &= 0xFFFFF0FF;       //configure PA6 as external interrupt using SYSCFG_EXTICR2
	EXTI_RTSR |= (0x01 << 6);           //Enable Rising Edge Trigger
	EXTI_FTSR |= (0x01 << 6);           //Enable Falling Edge Trigger
	EXTI_IMR  |= (0x01 << 6);
  NVIC_ISER0 |= (0x01 << 23);         //set 23rd bit in NVIC_ISTR0 to enable NVIC 23rd interrupt line

}


/*************************************************************************************
*                               switch_up_intr_config()
**************************************************************************************/

void switch_up_intr_config(void)
{
	RCC_AHB1ENR |= (0x01 << 2);     //set 2nd bit of RCC_AHB1ENR to enable port C clock
	RCC_APB2ENR |= (0x01 << 14);    //set 14th bit of RCC_APB2ENR to Enable clock to SYSCONFIG

  GPIOC_MODER &= 0xFFFCFFFF;      //clear 17-16 bits of GPIOC_MODER (by default it is in input mode only)
	GPIOC_PUPDR &= 0xFFFCFFFF;      //clear 17-16 bits of GPIOC_PUPDR
	GPIOC_PUPDR |= 0x00010000;      //load "01" inti 17-16 bit fields in GPIOC_PUPDR
	
	SYSCFG_EXTICR3 &= 0xFFFFFFF0;   //clear 3-0 bits of SYSCFG_EXTICR3
	SYSCFG_EXTICR3 |= 0x00000002;   //load "0010" bits in to 3-0 bit fields of SYSCFG_EXTICR3 to configure PC8 as external interrupt
	   
	EXTI_FTSR |= (0x01 << 8);       //set 8th bit in EXTI_FTSR to configure PC8 as falling Edge interrupt
	EXTI_IMR  |= (0x01 << 8);       //set 8th bit in EXTI_IMR to enable EXTI9_5 interrupt line
	
	NVIC_ISER0 |= (0x01 << 23);     //set 23rd bit in NVIC_ISTR0 to enable NVIC 23rd interrupt line	
}



/*************************************************************************************
*                               switch_down_intr_config()
**************************************************************************************/


void switch_down_intr_config(void)
{
	RCC_AHB1ENR |= (0x01 << 2);     //set 2nd bit of RCC_AHB1ENR to enable port C clock
	RCC_APB2ENR |= (0x01 << 14);    //set 14th bit of RCC_APB2ENR to Enable clock to SYSCONFIG

  GPIOC_MODER &= 0xFFF3FFFF;      //clear 19-18 bits of GPIOC_MODER (by default it is in input mode only)
	GPIOC_PUPDR &= 0xFFF3FFFF;      //clear 19-18 bits of GPIOC_PUPDR
	GPIOC_PUPDR |= 0x00040000;      //load "01" inti 19-18 bit fields in GPIOC_PUPDR
	
	SYSCFG_EXTICR3 &= 0xFFFFFF0F;   //clear 7-4 bits of SYSCFG_EXTICR3
	SYSCFG_EXTICR3 |= 0x00000020;   //load "0010" bits in to 7-4 bit fields of SYSCFG_EXTICR3 to configure PC9 as external interrupt
	   
	EXTI_FTSR |= (0x01 << 9);       //set 9th bit in EXTI_FTSR to configure PC9 as falling Edge interrupt
	EXTI_IMR  |= (0x01 << 9);       //set 8th bit in EXTI_IMR to enable EXTI9_5 interrupt line
	
	NVIC_ISER0 |= (0x01 << 23);     //set 23rd bit in NVIC_ISTR0 to enable NVIC 23rd interrupt line	
}



/*************************************************************************************
*                               enter_switch_intr_config()
**************************************************************************************/

void enter_switch_intr_config(void)
{
	RCC_AHB1ENR |= (0x01 << 2);     //set 2nd bit of RCC_AHB1ENR to enable port C clock
	RCC_APB2ENR |= (0x01 << 14);    //set 14th bit of RCC_APB2ENR to Enable clock to SYSCONFIG

  GPIOC_MODER &= 0xFFCFFFFF;      //clear 21-20 bits of GPIOC_MODER (by default it is in input mode only)
	GPIOC_PUPDR &= 0xFFCFFFFF;      //clear 21-20 bits of GPIOC_PUPDR
	GPIOC_PUPDR |= 0x00100000;      //load "01" inti 21-20 bit fields in GPIOC_PUPDR
	
	SYSCFG_EXTICR3 &= 0xFFFFF0FFF;  //clear 11-8 bits of SYSCFG_EXTICR3
	SYSCFG_EXTICR3 |= 0x00000200;   //load "0010" bits in to 11-8 bit fields of SYSCFG_EXTICR3 to configure PC10 as external interrupt
	   
	EXTI_FTSR |= (0x01 << 10);      //set 10th bit in EXTI_FTSR to configure PC10 as falling Edge interrupt
	EXTI_IMR  |= (0x01 << 10);      //set 10th bit in EXTI_IMR to enable EXTI15_10 interrupt line
	
	NVIC_ISER1 |= (0x01 << 8);      //set 8th bit in NVIC_ISTR1 to enable NVIC 40th interrupt line
}


