/*************************************************************************************
* File : stm32f401rbt6.h
* Description : This file contains all BASE ADDRESSES and PHYSICAL ADDRESSES 
*               of STM32 peripherals and macros used.
**************************************************************************************/

/***********************************************************************************
*                          BASE ADDRESSES OF STM32
************************************************************************************/


//RCC
#define RCC_BASE_ADDR    0x40023800

//GPIO
#define GPIOA_BASE_ADDR  0x40020000
#define GPIOB_BASE_ADDR  0x40020400
#define GPIOC_BASE_ADDR  0x40020800

//SYSCFG
#define SYSCFG_BASE_ADDR 0x40013800

//EXTI
#define EXTI_BASE_ADDR   0x40013C00

//ADC
#define ADC_BASE_ADDR    0x40012000

//UART
#define USART1_BASE_ADDR 0x40011000
#define USART2_BASE_ADDR 0x40004400
#define USART6_BASE_ADDR 0x40011400


//I2C
#define I2C1_BASE_ADDR   0x40005400


/****************************************************************************************
 *                            BASE ADDRESSES OF ARM
*****************************************************************************************/


//NVIC
#define NVIC_BASE_ADDR   0xE000E100

//STK
#define STK_BASE_ADDR    0xE000E010



/********************************************************************************************
*                           PHYSICAL ADDRESSES OF STM32 REGISTERS
*********************************************************************************************/


//RCC
#define RCC_AHB1ENR    *((int *)(RCC_BASE_ADDR + 0x30))
#define RCC_AHB2ENR    *((int *)(RCC_BASE_ADDR + 0x34))
#define RCC_APB1ENR    *((int *)(RCC_BASE_ADDR + 0x40))
#define RCC_APB2ENR    *((int *)(RCC_BASE_ADDR + 0x44))


//GPIOA
#define GPIOA_ODR      *((int *)(GPIOA_BASE_ADDR + 0x14))
#define GPIOA_IDR      *((int *)(GPIOA_BASE_ADDR + 0x10))
#define GPIOA_MODER    *((int *)(GPIOA_BASE_ADDR + 0x00))
#define GPIOA_OTYPER   *((int *)(GPIOA_BASE_ADDR + 0x04))
#define GPIOA_PUPDR    *((int *)(GPIOA_BASE_ADDR + 0x0C))
#define GPIOA_AFRL     *((int *)(GPIOA_BASE_ADDR + 0x20))
#define GPIOA_AFRH     *((int *)(GPIOA_BASE_ADDR + 0x24))
	

//GPIOB
#define GPIOB_ODR      *((int *)(GPIOB_BASE_ADDR + 0x14))
#define GPIOB_IDR      *((int *)(GPIOB_BASE_ADDR + 0x10))
#define GPIOB_MODER    *((int *)(GPIOB_BASE_ADDR + 0x00))
#define GPIOB_OTYPER   *((int *)(GPIOB_BASE_ADDR + 0x04))
#define GPIOB_PUPDR    *((int *)(GPIOB_BASE_ADDR + 0x0C))
#define GPIOB_AFRL     *((int *)(GPIOB_BASE_ADDR + 0x20))
#define GPIOB_AFRH     *((int *)(GPIOB_BASE_ADDR + 0x24))
	
	
//GPIOC
#define GPIOC_ODR      *((int *)(GPIOC_BASE_ADDR + 0x14))
#define GPIOC_IDR      *((int *)(GPIOC_BASE_ADDR + 0x10))
#define GPIOC_MODER    *((int *)(GPIOC_BASE_ADDR + 0x00))
#define GPIOC_OTYPER   *((int *)(GPIOC_BASE_ADDR + 0x04))
#define GPIOC_PUPDR    *((int *)(GPIOC_BASE_ADDR + 0x0C))
#define GPIOC_AFRL     *((int *)(GPIOC_BASE_ADDR + 0x20))
#define GPIOC_AFRH     *((int *)(GPIOC_BASE_ADDR + 0x24))
	
	

//SYSCFG
#define SYSCFG_EXTICR2 *((int *)(SYSCFG_BASE_ADDR + 0x0C))
#define SYSCFG_EXTICR3 *((int *)(SYSCFG_BASE_ADDR + 0x10))
	

//EXTI
#define EXTI_IMR       *((int *)(EXTI_BASE_ADDR + 0x00))
#define EXTI_FTSR      *((int *)(EXTI_BASE_ADDR + 0x0C))
#define EXTI_RTSR      *((int *)(EXTI_BASE_ADDR + 0x08))	
#define EXTI_PR        *((int *)(EXTI_BASE_ADDR + 0x14))
	
//ADC
#define ADC_SQR1       *((int *)(ADC_BASE_ADDR + 0x2C))	
#define ADC_SQR3       *((int *)(ADC_BASE_ADDR + 0x34))
#define ADC_CR2        *((int *)(ADC_BASE_ADDR + 0x08))
#define ADC_CR1        *((int *)(ADC_BASE_ADDR + 0x04))	
#define ADC_SR         *((int *)(ADC_BASE_ADDR + 0x00))
#define ADC_DR         *((int *)(ADC_BASE_ADDR + 0x4C))
#define ADC_HTR        *((int *)(ADC_BASE_ADDR + 0x24))	
#define ADC_LTR        *((int *)(ADC_BASE_ADDR + 0x28))	


//USART1
#define USART1_SR       *((int *)(USART1_BASE_ADDR + 0x00))	
#define USART1_DR       *((int *)(USART1_BASE_ADDR + 0x04))	
#define USART1_BRR      *((int *)(USART1_BASE_ADDR + 0x08))	
#define USART1_CR1      *((int *)(USART1_BASE_ADDR + 0x0C))	
#define USART1_CR2      *((int *)(USART1_BASE_ADDR + 0x10))	
#define USART1_CR3      *((int *)(USART1_BASE_ADDR + 0x14))	
#define USART1_GTPR     *((int *)(USART1_BASE_ADDR + 0x18))	
	
//USART2
#define USART2_SR       *((int *)(USART2_BASE_ADDR + 0x00))	
#define USART2_DR       *((int *)(USART2_BASE_ADDR + 0x04))	
#define USART2_BRR      *((int *)(USART2_BASE_ADDR + 0x08))	
#define USART2_CR1      *((int *)(USART2_BASE_ADDR + 0x0C))	
#define USART2_CR2      *((int *)(USART2_BASE_ADDR + 0x10))	
#define USART2_CR3      *((int *)(USART2_BASE_ADDR + 0x14))	
#define USART2_GTPR     *((int *)(USART2_BASE_ADDR + 0x18))	
	
//USART6
#define USART6_SR       *((int *)(USART6_BASE_ADDR + 0x00))	
#define USART6_DR       *((int *)(USART6_BASE_ADDR + 0x04))	
#define USART6_BRR      *((int *)(USART6_BASE_ADDR + 0x08))	
#define USART6_CR1      *((int *)(USART6_BASE_ADDR + 0x0C))	
#define USART6_CR2      *((int *)(USART6_BASE_ADDR + 0x10))	
#define USART6_CR3      *((int *)(USART6_BASE_ADDR + 0x14))	
#define USART6_GTPR     *((int *)(USART6_BASE_ADDR + 0x18))
	

//I2C1
#define  I2C1_CR1       *((int *)(I2C1_BASE_ADDR + 0x00))
#define  I2C1_CR2       *((int *)(I2C1_BASE_ADDR + 0x04))
#define  I2C1_OAR1      *((int *)(I2C1_BASE_ADDR + 0x08))
#define  I2C1_OAR2      *((int *)(I2C1_BASE_ADDR + 0x0C))
#define  I2C1_DR        *((int *)(I2C1_BASE_ADDR + 0x10))
#define  I2C1_SR1       *((int *)(I2C1_BASE_ADDR + 0x14))
#define  I2C1_SR2       *((int *)(I2C1_BASE_ADDR + 0x18))
#define  I2C1_CCR       *((int *)(I2C1_BASE_ADDR + 0x1C))
#define  I2C1_TRISE     *((int *)(I2C1_BASE_ADDR + 0x20))
#define  I2C1_FLTR      *((int *)(I2C1_BASE_ADDR + 0x24))

/*******************************************************************************************
*                             PHYSICAL ADDRESSES OF ARM PERIPHERALS
********************************************************************************************/

//NVIC
#define NVIC_ISER0    *((int *)(NVIC_BASE_ADDR + 0x00))
#define NVIC_ISER1    *((int *)(NVIC_BASE_ADDR + 0x04))


//STK
#define STK_CTRL       *((int *)(STK_BASE_ADDR + 0x00))
#define STK_LOAD       *((int *)(STK_BASE_ADDR + 0x04))
#define STK_VAL        *((int *)(STK_BASE_ADDR + 0x08))

