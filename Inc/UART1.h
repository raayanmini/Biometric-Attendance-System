/**************************************************************************************
* File : UART1.h
* Description : This file contains the function declarations of UART1
***************************************************************************************/

#define TIMEOUT        -1


/*************************************************************************************
*                                   UART1_Init()
***************************************************************************************
*Description : This function Initializes the UART1
*Arguments : None
*Return Type : None (Void function)
*Notes : This function Initializes and configure PA9 and PA10 as UART1 pins and Enbales UART1 clock 
***************************************************************************************/

void UART1_Init(void);



/*************************************************************************************
*                                       UART1_Config()
***************************************************************************************
*Description : This function Configures the UART1
*Arguments : None
*Return Type : None
*Notes : set the Baud rate and enables UART1, UART1_TX, UART1_RX
***************************************************************************************/

void UART1_Config(void);



/*************************************************************************************
*                                  UART1_OutChar()
***************************************************************************************
*Description : This function Transmits a character to receiver
*Arguments : unsigned char --> a character
*Return Type : None (void function)
*Notes : This function checkes TXE and loads the character to USART1_DR
***************************************************************************************/

void UART1_OutChar(unsigned char );



/*************************************************************************************
*                                  UART1_OutStr()
***************************************************************************************
*Description : This function Trasmits a string
*Arguments : unsigned char --> str[]
             unsigned int  --> length (length of str[])
*Return Type : None (void function)
*Notes : This functions sends each character of string to UART1_OutChar() function.
***************************************************************************************/

void UART1_OutStr(const unsigned char str[],unsigned int length);


/*************************************************************************************
*                               USART1_InChar_TIMEOUT()
***************************************************************************************
*Description : This function Receives a character from Transmitter
*Arguments : unsigned int --> iteration_count
*Return Type : (0xFF & USART1_DR) --> SUCCESS (Data Received)
               1 --> TIMEOUT (Data Not Received)
*Notes : This function checkes RXE and returns the character from USART1_DR.
***************************************************************************************/

int UART1_InChar_TIMEOUT (unsigned int );

