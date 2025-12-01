/**************************************************************************************
* File : gpio.c
* Description : This file contains function definitions related to GPIO initialization
*               and configuration

* Ports : GPIOA,GPIOB,GPIOC
**************************************************************************************/

#include "stm32f401rbt6.h"
#include "gpio.h"


/********************************************gpio_port_Init()******************************/

void gpio_port_Init(int port)
{
	 switch(port)
	 {
		 case GPIOA :
			 //Check if GPIOA clock is already enabled (check the 0th bit)
		 if ((RCC_AHB1ENR & 0x01) == 0)   //If GPIOA clock is not enabled
     {
			 RCC_AHB1ENR |= 0x01;           //Enable GPIOA clock
     }		        
		 break;
		 case GPIOB :
     // Check if GPIOB clock is already enabled (check the 1st bit)
		 if ((RCC_AHB1ENR & (0x01 << 1)) == 0)   //If GPIOB clock is not enabled
     {
			 RCC_AHB1ENR |= (0x01 << 1);           //Enable GPIOB clock
     }		         
		 break;
		 case GPIOC:
     //Check if GPIOC clock is already enabled (check the 2nd bit)
		 if ((RCC_AHB1ENR & (0x01 << 2)) == 0)   //If GPIOc clock is not enabled
		 {
			 RCC_AHB1ENR |= (0x01 << 2);           //Enable GPIOC clock
     }		         
		 break;
	 }
 }	 

 

 /**************************************************gpio_port_Config()***********************************/
 
 void gpio_port_Config(int port,int mode,int pin)
 {
	 switch(port)
	 {
		 case GPIOA:
		 {
			 switch(mode)
			 {
				 case IN:
					 break;
				 case OUT:
					 break;
			 }				 
			 break;
		 } 
		 case GPIOB:
                 {
			 switch(mode)
			 {
				 case IN:
					 GPIOB_MODER &= ~(0x03 << (pin * 2));  // Clear the bits for input mode
					 break;
				 case OUT:
					 GPIOB_MODER &= ~(0x03 << (pin * 2));      // Clear the bits
					 GPIOB_MODER |= (0x01 << (pin*2));         //loading "01" into particular GPIOB_MODER pins to configure pin as output direction
					 break; 
			 }				 
			 break;
		 } 
		 case GPIOC:
		 {
			 switch(mode)
			 {
				 case IN:
					 GPIOC_MODER &= ~(0x03 << (pin * 2));  // Clear the bits for input mode
					 break;
				 case OUT:
					 break;
				 case ANALOG:
					 GPIOC_MODER &= ~(0x03 << (pin * 2));  // Clear the bits
				   GPIOC_MODER |= (0x03 << (pin * 2));   // Set "11" to configure pin as analog mode
           break;
				 case ALT: 
					 GPIOC_MODER &= ~(0x03 << (pin * 2));  // Clear the bits
				   GPIOC_MODER |= (0x02 << (pin * 2));   // Set "10" to configure pin as alternate function mode
           break;
				 case PULLUP:
					 GPIOC_PUPDR &= ~(0x03 << (pin * 2));  // Clear the bits
           GPIOC_PUPDR |= (0x01 << (pin * 2));   // Enable pull-up
					 break;
			 }				 
			 break;
		 } 
  		 
	 }
    
 }	 
	 
 
