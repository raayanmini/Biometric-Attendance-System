/**************************************************************************************
* File : systemticktimer.h
* Description : This file contains the function declarations of SysTick Timer to generate
                delays (in msec, usec)
***************************************************************************************/


//milli second delay Macro
#define MDELAY        0

//micro second delay Macro
#define UDELAY        1

#define MAX_UINT_32   4294967295u

/*************************************************************************************
*                            SysTick_Init()
***************************************************************************************
*Description : This function Initializes and configures SysTick timer for Polling Metod
*Arguments : None
*Return Type : None (void function)
*Notes :
***************************************************************************************/

void SysTick_Init(int delay);

/*************************************************************************************
*                               SysTick_Handler()
***************************************************************************************
*Description : This function Handles the interrupt of SysTick timer
*Arguments : None
*Return Type : None (void function)
*Notes :
***************************************************************************************/

void SysTick_Handler(void);

/*************************************************************************************
*                               KM_mdelay()
***************************************************************************************
*Description : This function provides delay in milli seconds
*Arguments : int delay --> amount of msec delay to be generated
*Return Type : None (void function)
*Notes : This function generates delay amount of msec delay
***************************************************************************************/

void KM_Delay_ms(unsigned int delay);

/*************************************************************************************
*                               KM_udelay()
***************************************************************************************
*Description : This function provides delay in micro seconds
*Arguments : int delay --> amount of usec delay to be generated
*Return Type : None (void function)
*Notes : This function generates delay amount of usec delay
***************************************************************************************/

void KM_Delay_us(unsigned int delay);

