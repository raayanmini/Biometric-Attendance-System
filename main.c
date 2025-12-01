/*
 Program : BIOMETRIC ATTENDANCE SYSTEM USING GT-521F32 FINGERPRINT SENSOR
 File Name : BAS_using_GT521F32.c
 Author : G. Naga Sai Krishna Chaitanya
 Date :

 Board : RAAYAN MINI
 Version : 4.0
 Controller : STM32F401RBT6
 Debugger : STLink Debugger V2.0
 Pins used :

*/


#include "stm32f401rbt6.h"
#include "UART1.h"
#include "FP_gt521f32.h"
#include "systemticktimer.h"
#include "km_lcd.h"
#include "stdio.h"
#include "Interrupt.h"
#include "4x4_keypad.h"


int touch_flag=0;
int ent_sw_flag=0;
int sw_up_flag=0;
int sw_down_flag=0;
int id;

char Buffer1[20];
 
 
/*****************************************************************************
*                             MAIN CONTEXT
******************************************************************************/

int main()
{
	/*Phase 1: Device Initialization */
	/*Phase 2: Device configuration */
	SysTick_Init(MDELAY);
	UART1_Init();
	UART1_Config();
	KM_LCD_Init();
	Keypad_Init();
	Touch_IC_Config();
	switch_down_intr_config();
	enter_switch_intr_config();
	
	
  /*Initialize fingerprint module*/
	KM_LCD_Write_Cmd(0X01);
	KM_LCD_Write_Cmd(0x82);
	KM_LCD_Write_Str((unsigned char *)"INITIALIZING");
	KM_LCD_Write_Cmd(0XC0);
	KM_LCD_Write_Str((unsigned char *)"BIOMETRIC SENSOR");
  SendCommand(OPEN, 0, 150000);
  KM_Delay_ms(1500);
	
	KM_LCD_Write_Cmd(0xC0);
	KM_LCD_Write_Str((unsigned char *)"                ");
	KM_LCD_Write_Cmd(0xC1);
	KM_LCD_Write_Str((unsigned char *)"MATRIX KEYPAD");
	KM_Delay_ms(1500);
	
	KM_LCD_Write_Cmd(0x01);
	KM_LCD_Write_Cmd(0x81);
	KM_LCD_Write_Str((unsigned char *)"INITIALIZATION");
	KM_LCD_Write_Cmd(0xC4);
	KM_LCD_Write_Str((unsigned char *)"SUCCESS");
	KM_Delay_ms(1500);
	
	KM_LCD_Write_Cmd(0x01);
	KM_LCD_Write_Cmd(0x84);
	KM_LCD_Write_Str((unsigned char *)"BIOMETRIC");
	KM_LCD_Write_Cmd(0xC3);	
	KM_LCD_Write_Str((unsigned char *)"ATTENDANCE");
	KM_Delay_ms(2000);


  while (1)
  {
		KM_LCD_Write_Cmd(0x01);
		KM_LCD_Write_Str((unsigned char *)"TO MARK PRESENT PRESS DN SWITCH");
		KM_Delay_ms(1000);
	
		if(sw_down_flag)      //If Ent_Switch is pressed
    {
			sw_down_flag = 0;   //Reset flag
		  UserMode();         //UserMode()
		}
		
		if(ent_sw_flag)      //If Ent_Switch is pressed
    {
			ent_sw_flag = 0;   //Reset flag
			AdminMode();       //Go to Admin Mode			
		}
  }		
}



/*****************************************************************************
*                             INTERRUPT CONTEXT
******************************************************************************/


/***************************EXTI9_5_IRQHandler******************************/

void EXTI9_5_IRQHandler(void) 
{
	if (EXTI_PR & (0x01 << 6))                 //Check if PA6 triggered interrupt
  {		
		if(GPIOA_IDR & (0x01 << 6))              //If PA6 is HIGH (pressed)
    {  
      touch_flag=1;                          //set the touch_flag     
			SendCommand(CMOSLED, 1, 10000);       //ON CMOS LED of GT-521F32 Module 
    }
    else                                     //If PA6 is LOW (released)
    {
      touch_flag=0;                          //clear the touch_flag 
			SendCommand(CMOSLED, 0, 10000);       //OFF CMOS LED of GT-521F32 Module
    }
		
	  EXTI_PR |= (0x01 << 6);                 //Clear the pending interrupt flag
  }
	

	
  if(EXTI_PR & (0x01 << 9))                 //Check if PC9 triggered interrupt
  {	
		sw_down_flag=1;                         //set sw_down_flag
    EXTI_PR |=  (0x01 << 9);                //clear the pending interrupt flag
  }
	
}



/***************************EXTI15_10_IRQHandler*******************************/

void EXTI15_10_IRQHandler(void) 
{
	if(EXTI_PR & (0x01 << 10))              //Check if PC10 triggered interrupt
  {
		ent_sw_flag=1;                        //set ent_sw_flag
    EXTI_PR |=  (0x01 << 10);             //clear the pending interrupt flag
  }
}	

