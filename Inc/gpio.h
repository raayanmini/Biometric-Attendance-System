/**************************************************************************************
* File : gpio.h
* Description : This file contains function declarations and Macros related to GPIO

* Ports : GPIOA,GPIOB,GPIOC
**************************************************************************************/

/*************************GPIO PORT MACROS********************************/
#define GPIOA         0
#define GPIOB         1
#define GPIOC         2

/************************GPIO PORT MODES*********************************/
#define IN            0
#define OUT           1
#define ANALOG        2
#define ALT           3

#define PULLUP        4

//GPIO function declarations

/*************************************************************************************
*                                 gpio_port_Init()
***************************************************************************************
*Description : This function Initializes the GPIO ports 
*Arguments : int port --> port name (GPIOA/GPIOB/GPIOC)
*Return Type : No return type (void function)
*Notes : Enable GPIOA, GPIOB, GPIOC ports cloks.
***************************************************************************************/

void gpio_port_Init(int port);



/*************************************************************************************
*                                gpio_port_Config()
***************************************************************************************
*Description : This function configures GPIO ports
*Arguments : int port --> port name (GPIOA/GPIOB/GPIOC)
*            int mode --> mode (IN/OUT/ANALOG/ALT)
*            int pin  --> pin number 
*Return Type : No return type (void function)
*Notes : configures pin to output/Input/Alternate/Analog modes
***************************************************************************************/

void gpio_port_Config(int port,int mode,int pin);
