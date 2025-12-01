/**************************************************************************************
* File : km_lcd.c
* Description : This file contains contains function Definitions related to LCD
***************************************************************************************/

#include "stm32f401rbt6.h"
#include "gpio.h"
#include "systemticktimer.h"
#include "km_lcd.h"



/***********************************write_high_nibble()**********************************/	

void write_high_nibble(unsigned char ch)
{
	ch = ch >> 4;                  //extract higher byte of character

  GPIOB_ODR &= 0xFFFFFFF0;      //clear 0-3 bits of GPIOB_ODR
  GPIOB_ODR |= ch;              //load ch into GPIOB_ODR
     
  GPIOB_ODR |= (0x01 << 8);	   //set 8th bit of GPIOB_ODR to set LCD_EN
  KM_Delay_ms(20);               //20msec delay
  GPIOB_ODR &= ~(0x01 << 8);	 //clear 8th bit of GPIOB_ODR to clear LCD_EN
}



/*************************************write_low_nibble()***********************************/

void write_low_nibble(unsigned char ch)
{
	ch = ch & 0x0f;                  //extract lower byte of character

  GPIOB_ODR &= 0xFFFFFFF0;         //clear 0-3 bits of GPIOB_ODR
  GPIOB_ODR |= ch;                 //load ch into GPIOB_ODR
     
  GPIOB_ODR |= (0x01 << 8);	       //set 8th bit of GPIOB_ODR to set LCD_EN
  KM_Delay_ms(20);                   //20msec delay
  GPIOB_ODR &= ~(0x01 << 8);	     //clear 8th bit of GPIOB_ODR to clear LCD_EN
}



/************************************KM_LCD_Write_Cmd()*******************************/

void KM_LCD_Write_Cmd(unsigned char data)
{
	GPIOB_ODR &= ~(0x01 << 5);    //clear 5th bit of GPIOB_ODR to enable write mode of LCD_RW
	  
  GPIOB_ODR &= ~(0x01 << 4);    //clear 4th bit of GPIOB_ODR to enable command mode of LCD_RS
	
  write_high_nibble(data);      //send higher byte of Data as command
  write_low_nibble(data);       //send lower byte of Data as command
}



/***************************************KM_LCD_Write_Data()**********************************/

void KM_LCD_Write_Data(unsigned char data)
{
	GPIOB_ODR &= ~(0x01 << 5);   //clear 5th bit of GPIOB_ODR to enable write mode of LCD_RW
	  
  GPIOB_ODR |= (0x01 << 4);    //clear 4th bit of GPIOB_ODR to enable data mode of LCD_RS
	
  write_high_nibble(data);     //send higher byte of Data as command
  write_low_nibble(data);      //send lower byte of Data as command
}




/*********************************************KM_LCD_Init()***********************************/

void KM_LCD_Init(void)
{
	gpio_port_Init(GPIOB);            //clock for port B
	gpio_port_Config(GPIOB,OUT,0);    //configure PB0 as output direction
  gpio_port_Config(GPIOB,OUT,1);    //configure PB1 as output direction
  gpio_port_Config(GPIOB,OUT,2);    //configure PB2 as output direction
  gpio_port_Config(GPIOB,OUT,3);    //configure PB3 as output direction
  gpio_port_Config(GPIOB,OUT,4);    //configure PB4 as output direction
  gpio_port_Config(GPIOB,OUT,5);    //configure PB5 as output direction
  gpio_port_Config(GPIOB,OUT,8);    //configure PB8 as output direction

  KM_Delay_ms(20);                    //20 msec delay
	   
  KM_LCD_Write_Cmd(0x33);
  KM_Delay_ms(1);                     //1 msec delay
	 
  KM_LCD_Write_Cmd(0x32);
	
  KM_LCD_Write_Cmd(0x0C);           //Write 0x0C (Display On, Cursor OFF) Command
	
  KM_LCD_Write_Cmd(0x01);           //Write 0x01 (Display Clear) Command
}



/********************************************KM_LCD_Write_Str()**************************/

void KM_LCD_Write_Str(unsigned char str[])
{
	unsigned int i=0;
  //KM_LCD_Write_Cmd (0x80);
	//KM_LCD_Write_cmd (0xC0);
	
	for(i=0;str[i];i++)
	{
		if(i==16)
		{
			KM_LCD_Write_Cmd (0xC0);
		}
		KM_LCD_Write_Data(str[i]);
	}

}	
