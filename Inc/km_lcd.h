/**************************************************************************************
* File : km_lcd.h
* Description : This file contains function Declarations and Macros related to LCD
***************************************************************************************/



/*************************************************************************************
*                                 write_high_nibble()
***************************************************************************************
*Description : This function extract higher byte nibble and send to LCD
*Arguments : unsigned char ch
*Return Type : No return type (void function)
*Notes : 
***************************************************************************************/

void write_high_nibble( unsigned char );



/*************************************************************************************
*                                 write_low_nibble()
***************************************************************************************
*Description : This function extract lower byte nibble and send to LCD
*Arguments : unsigned char ch
*Return Type : No return type (void function)
*Notes : 
***************************************************************************************/

void write_low_nibble( unsigned char );



/*************************************************************************************
*                                KM_LCD_Init()
***************************************************************************************
*Description : This function Initializes the LCD
*Arguments : No Parameters
*Return Type : No return type (void function)
*Notes : 
***************************************************************************************/

void KM_LCD_Init(void);



/*************************************************************************************
*                                KM_LCD_Write_Cmd()
***************************************************************************************
*Description : This function writes command to LCD
*Arguments : unsigned char data
*Return Type : No return type (void function)
*Notes : 
***************************************************************************************/

void KM_LCD_Write_Cmd( unsigned char );



/*************************************************************************************
*                                KM_LCD_Write_Data()
***************************************************************************************
*Description : This function writes Data to LCD
*Arguments : unsigned char data
*Return Type : No return type (void function)
*Notes : 
***************************************************************************************/

void KM_LCD_Write_Data( unsigned char );


/*************************************************************************************
*                               KM_LCD_Write_Str()
***************************************************************************************
*Description : This function send string to LCD
*Arguments : unsigned char str[]
*Return Type : No return type (void function)
*Notes : sends each character of string to  KM_LCD_Write_Data() function
***************************************************************************************/

void KM_LCD_Write_Str(unsigned char []);

