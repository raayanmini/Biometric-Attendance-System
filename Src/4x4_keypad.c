/**************************************************************************************
* File :4x4_keypad.c
* Description : This file contains the function definitions related to 4x4 Mtrix Keypad
***************************************************************************************/

#include "stm32f401rbt6.h"
#include "km_lcd.h"
#include "systemticktimer.h"
#include "my_string.h"
#include "FP_gt521f32.h"



#define PW_LENGTH  5    //4-character password + null terminator
char password[PW_LENGTH];

#define ADMIN_PASSWORD "1239"  //Change this to your actual admin password
#define MAX_ATTEMPTS 3

extern int ent_sw_flag;
extern volatile int timer_counter;    //Declare global variable



const char Keypad[4][4] = {
                            {'1', '2', '3', 'A'},
                            {'4', '5', '6', 'B'},
                            {'7', '8', '9', 'C'},
                            {'*', '0', '#', 'D'}
                          };

/****************************************************************************
*                              Keypad_Init()
******************************************************************************/													
																										
void Keypad_Init(void) 
{
	RCC_AHB1ENR |= (0x01 << 2);       //set 2nd bit of RCC_AHB1ENR to Enable Port C clock
	
	GPIOC_MODER &= ~(0x000000FF);     //clear 7-0 bits of GPIOC_MODER
	GPIOC_MODER |= 0x00000055;        /*load "01" into 7-6,5-4,3-2,1-0 bit fields of GPIOC_MODER to configure
                                      PC3,PC2,PC1,PC0 as ouput mode pins*/

  GPIOC_MODER &= ~(0x003C0F00);       /*clear 25-22,11-8 bits of GPIOC_MODER to confiure PC12,PC11,PC5,PC4
	                                    as input mode pins*/
	
  GPIOC_PUPDR &= ~(0x003C0F00);	      //clear 25-22,11-8 bits of GPIOC_PUPDR
	GPIOC_PUPDR |= 0x02800A00;        /*load "10" into 25-24,23-22,11-10,9-8 bitfields to configure 
	                                    PC12,PC11,PC5,PC4 as pull-downs*/
}
				


/****************************************************************************/

const int row_pins[4] = {0, 1, 2, 3};     //Rows pins   : PC0 - PC3
const int col_pins[4] = {4, 5, 11, 12};   //Columns pins: PC4,PC5,PC11,PC12 

/****************************************************************************
*                              Keypad_Scan()
******************************************************************************/

int Keypad_Scan(void)
{
	int row,col;
	  
  for(row=0; row<4; row++)                            //Loop through each row (PC0 - PC3)
	{		
    GPIOC_ODR |= (0x01 << row_pins[row]);             //Set only the current row as output (HIGH)

		for(col=0; col<4; col++)                          //Loop through each column (PC4,PC5,PC11,PC12)
		{
			if(GPIOC_IDR & (0x1 << col_pins[col]))          //If column reads LOW (key pressed)
			{ 
        while(GPIOC_IDR & (1 << col_pins[col]));      //Wait for key release
				GPIOC_ODR &= ~(0x1 << row_pins[row]);         //Reset row to input mode(LOW)
        return Keypad[row][col];                       //Return the pressed key
      }
		}
		
    GPIOC_ODR &= ~(0x1 << row_pins[row]);             //Reset the row back to input mode
	}
	
  return -1;                                           //return 0 if No key pressed
}



/****************************************************************************
*                             EnterUserID()
*****************************************************************************/

void EnterUserID(void) 
{
	int index = 0;
  int key;

  while (1)  //Loop until a valid password is entered
  {
		KM_LCD_Write_Cmd(0x01);  //Clear LCD
    KM_LCD_Write_Str((unsigned char *)"ENTER STUDENT ID");
    KM_LCD_Write_Cmd(0xC0);  //Move to second line

    index = 0;  //Reset index for storing the ID
   		
		while(1)  //Infinite loop to handle both entry and backspacing
    {
			key = Keypad_Scan();

      if(key != -1)  
      {
				timer_counter = 0;  //Reset timer on keypress
				
				if(key == '*')  //Backspace functionality
        {
					if(index > 0)  
          {
						index--;  //Move back one position
            KM_LCD_Write_Cmd(0xC0 + index);  //Move cursor back
            KM_LCD_Write_Data(' ');  //Erase character
            KM_LCD_Write_Cmd(0xC0 + index);  //Move cursor back again
          }
        }
        else if(key == '#')  //If '#' is pressed before entering 4 characters
        {
					KM_LCD_Write_Cmd(0x01);  //Clear LCD
					KM_LCD_Write_Cmd(0x83);
          KM_LCD_Write_Str((unsigned char *)"INVALID ID");
					KM_LCD_Write_Cmd(0xC0);
					KM_LCD_Write_Str((unsigned char *)"ENTER 4 DIGIT ID");
          KM_Delay_ms(200);       
          break;  //Restart password entry
        }
        else if(index < 4)  //Normal character entry
        {
					password[index] = key;  
          KM_LCD_Write_Data(key);      
          index++;

          if(index == 4)  //Move to confirmation after 4 characters
						break;
        }
       KM_Delay_ms(300);  
      }
			if(ent_sw_flag)
			{
				return;
			}
    }

	
    if(index == 4)  //Only proceed if 4 digits were entered
    {
			password[4] = '\0';  //Null terminate after entering 4 digits

      //Now wait for `#` confirmation
      KM_LCD_Write_Cmd(0xC7);
      KM_LCD_Write_Str((unsigned char*)"PRESS #");

      while(1)  
      {
				key = Keypad_Scan();

        if(key == '*')  //Allow backspacing even after 4 digits
        {
					if(index > 0)  
          {
						index--;                     //Remove last entered character
            KM_LCD_Write_Cmd(0xC0 + index);
            KM_LCD_Write_Data(' ');      //Erase character
            KM_LCD_Write_Cmd(0xC0 + index);  //Move cursor back
          }
        }
        else if(index < 4 && key != -1)  //Allow re-entry after deletion
        {
					password[index] = key;
          KM_LCD_Write_Data(key);
          index++;
        }
				//else if(key == '#')
        if(key == '#')  
        {
					return;  //Exit after confirmation
        }
				
        if(ent_sw_flag)
				{
					return;
				}
        KM_Delay_ms(100);
      }
    }
		if(ent_sw_flag)
	  {
			return;
	  }
  }
}



/****************************************************************************
*                              VerifyAdminPassword()
*****************************************************************************/

int VerifyAdminPassword(void) 
{
	int index, key, attempts = 0;
	
	while (attempts < MAX_ATTEMPTS)   //Allow max 3 attempts
  {
		KM_LCD_Write_Cmd(0x01);         //Clear LCD
    KM_LCD_Write_Str((unsigned char *)"ENTER ADMIN PWD:");
    KM_LCD_Write_Cmd(0xC0);         //Move to second line
    index = 0;  

    while(1)  //Handle input
    {
			key = Keypad_Scan();

      if(key != -1)  
      {
				if(key == '*')  //Backspace functionality
        {
					if(index > 0)  
          {
						index--;
            KM_LCD_Write_Cmd(0xC0 + index);
            KM_LCD_Write_Data(' ');  
            KM_LCD_Write_Cmd(0xC0 + index);  
          }
        }
        else if(key == '#')  // If '#' is pressed before entering 4 characters
        {
					KM_LCD_Write_Cmd(0x01);  
          KM_LCD_Write_Str((unsigned char *)"INVALID PASSWORD");
          KM_Delay_ms(1000);       
          break;  // Restart password entry
        }
        else if(index < 4)  // Normal character entry
        {
					password[index] = key;  
          KM_LCD_Write_Data('*');  // Mask character
          index++;

          if(index == 4)
						break;
        }

        KM_Delay_ms(300);  
      }
    }

    if(index == 4)  //Only proceed if 4 digits were entered
    {
			password[4] = '\0';  

      //Now wait for '#' confirmation
      KM_LCD_Write_Cmd(0xC7);
      KM_LCD_Write_Str((unsigned char*)"PRESS #");

      while(1)  
      {
				key = Keypad_Scan();

        if(key == '*')  //Allow backspacing even after 4 digits
        {
					if(index > 0)  
          {
						index--;                    
            KM_LCD_Write_Cmd(0xC0 + index);
            KM_LCD_Write_Data(' ');      
            KM_LCD_Write_Cmd(0xC0 + index);  
          }
        }
        else if(index < 4 && key != -1)  
        {
					password[index] = key;
          KM_LCD_Write_Data('*');  
          index++;
        }
        else if(key == '#')  
        {
					if(my_strcmp(password, ADMIN_PASSWORD) == 0)
          {
						KM_LCD_Write_Cmd(0x01);  
            KM_LCD_Write_Str((unsigned char*)"ACCESS GRANTED");
            KM_Delay_ms(1000);
            return 1;  //Success
          }
          else
          {
						attempts++;
            KM_LCD_Write_Cmd(0x01);
            KM_LCD_Write_Str((unsigned char*)"ACCESS DENIED");
            KM_LCD_Write_Cmd(0xC0);
            KM_LCD_Write_Str((unsigned char *)"TRIES LEFT: ");
            KM_LCD_Write_Data('0' + (MAX_ATTEMPTS - attempts));  //Show remaining attempts
            KM_Delay_ms(1500);
            break;  //Retry password entry
          }
        }

        KM_Delay_ms(300);
      }
    }
  }

  //If max attempts exceeded, exit admin mode
  return 0;  //Failure
}

