/*
 * stm32f446xx.h
 *
 *  Created on: Jul 27, 2024
 *      Author: Hp
 */

#ifndef INC_STM32F446XX_H_
#define INC_STM32F446XX_H_

#include <stddef.h>
#include <stdint.h>

#define __weak __attribute__((weak))

/****************************START: PROCESSOR SPECIFIC DETAILS****************************/
/*
 * ARM CORTEX M4 Processor NVIC ISERx register addresses
 */
#define NVIC_ISER0			((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1			((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2			((volatile uint32_t*)0xE000E108)
#define NVIC_ISER3			((volatile uint32_t*)0xE000E10C)

/*
 * ARM CORTEX M4 Processor NVIC ICERx register addresses
 */
#define NVIC_ICER0			((volatile uint32_t*)0xE000E180)
#define NVIC_ICER1			((volatile uint32_t*)0xE000E184)
#define NVIC_ICER2			((volatile uint32_t*)0xE000E188)
#define NVIC_ICER3			((volatile uint32_t*)0xE000E18C)

/*
 * ARM CORTEX M4 Processor Priority register addresses calculation
 */
#define NVIC_PR_BASE_ADDR	((volatile uint32_t*)0xE000E400)

#define NO_PR_BITS_IMPLEMENTED	4


/*
 * Base address of FLASH and SRAM memories
 */

//LA U AL FINAL ES DE QUE ES UNSIGNED
#define FLASH_BASEADDR			0x08000000U		//UBICACION DE LA FLASH MEMORY
#define SRAM1_BASEADDR			0x20000000U		//UBICACION DE LA SRMA1
#define SRAM2_BASEADDR			0x2001C000U		//UBICACION DE LA SRAM2
#define ROM						0x1FFF0000U		//UBICACION DE LA ROM (SYSTEM MEMORY)
#define SRAM					SRAM1_BASEADDR

/*
 * AHBx & APBx Bus peripheral base address
 */
#define PERIPH_BASE				0x40000000U
#define APB1PERIPH_BASE			PERIPH_BASE
#define APB2PERIPH_BASE			0x40010000U
#define AHB1PERIPH_BASE			0x40020000U
#define AHB2PERIPH_BASE			0x50000000U
#define AHB3PERIPH_BASE			0x60000000U


/*
 * BASE ADDRESSES OF PERIPHERALS WHICH ARE HANGING ON AHB1 BUS
 * TODO: Complete for all other peripherals
 */
#define GPIOA_BASEADDR			(AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASEADDR			(AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASEADDR			(AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASEADDR			(AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASEADDR			(AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASEADDR			(AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASEADDR			(AHB1PERIPH_BASE + 0x1800)
#define GPIOH_BASEADDR			(AHB1PERIPH_BASE + 0x1C00)
#define RCC_BASEADDR			(AHB1PERIPH_BASE + 0x3800)
/*
 * BASE ADDRESSES OF PERIPHERALS WHICH ARE HANGING ON APB1 BUS
 * TODO: Complete for all other peripherals
 */

#define I2C1_BASEADDR			(APB1PERIPH_BASE + 0x5400)
#define I2C2_BASEADDR			(APB1PERIPH_BASE + 0x5800)
#define I2C3_BASEADDR			(APB1PERIPH_BASE + 0x5C00)

#define SPI2_BASEADDR			(APB1PERIPH_BASE + 0x3800)
#define SPI3_BASEADDR			(APB1PERIPH_BASE + 0x3C00)

#define USART2_BASEADDR			(APB1PERIPH_BASE + 0x4400)
#define USART3_BASEADDR			(APB1PERIPH_BASE + 0x4800)
#define UART4_BASEADDR			(APB1PERIPH_BASE + 0x4C00)
#define UART5_BASEADDR			(APB1PERIPH_BASE + 0x5000)

/*
 * BASE ADDRESSES OF PERIPHERALS WHICH ARE HANGING ON APB2 BUS
 * TODO: Complete for all other peripherals
 */
#define ADC1_BASEADDR			(APB2PERIPH_BASE + 0x2000)
#define ADC2_BASEADDR			(APB2PERIPH_BASE + 0x2100)
#define ADC3_BASEADDR			(APB2PERIPH_BASE + 0x2200)
#define C_ADC_BASEADDR			(APB2PERIPH_BASE + 0x2300)

#define SPI1_BASEADDR			(APB2PERIPH_BASE + 0x3000)

#define USART1_BASEADDR			(APB2PERIPH_BASE + 0x1000)
#define USART6_BASEADDR			(APB2PERIPH_BASE + 0x1400)

#define EXTI_BASEADDR			(APB2PERIPH_BASE + 0x3C00)
#define SYSCFG_BASEADDR			(APB2PERIPH_BASE + 0x3800)


/******************PERIPHERAL REGISTER DEFINITION STRUCTURE******************/

//GPIOS
typedef struct				//COMO LOS TIPOS SON DE 32 bits el offset ira aumentando de 0x04 en 0x04
{
	volatile uint32_t MODER;			//GPIO port mode register 0x00
	volatile uint32_t OTYPER;			//GPIO port output type register 0x04
	volatile uint32_t OSPEEDER;			//GPIO port output speed register 0x08
	volatile uint32_t PUPDR;			//GPIO port pull-up/pull-down register 0x0C
	volatile uint32_t IDR;				//GPIO port input data register 0x10
	volatile uint32_t ODR;				//GPIO port output data register 0x14
	volatile uint32_t BSRR;				//GPIO port bit set/reset register 0x18
	volatile uint32_t LCKR;				//GPIO port configuration lock register 0x1C
	//uint32_t AFRL;					//GPIO alternate function low register 0x20
	//uint32_t AFRH;					//GPIO alternate function high register 0x24
	volatile uint32_t AFR[2];			//Es lo mismo que los dos arriba pero juntados en un vector. AFR[0] = LOW, AFR[1] = HIGH

}GPIO_RegDef_t;

//RCC
typedef struct				//COMO LOS TIPOS SON DE 32 bits el offset ira aumentando de 0x04 en 0x04
{
	volatile uint32_t CR;				//RCC clock control register 0x00
	volatile uint32_t PLLCFGR;			//RCC PLL configuration register 0x04
	volatile uint32_t CFGR;				//RCC clock configuration register 0x08
	volatile uint32_t CIR;				//RCC clock interrupt register 0x0C
	volatile uint32_t AHB1RSTR;			//RCC AHB1 peripheral reset register 0x10
	volatile uint32_t AHB2RSTR;			//RCC AHB2 peripheral reset register 0x14
	volatile uint32_t AHB3RSTR;			//RCC AHB3 peripheral reset register 0x18
	uint32_t RESERVED0;
	volatile uint32_t APB1RSTR;			//GRCC APB2 peripheral reset register 0x24
	volatile uint32_t APB2RSTR;			//GPIO port output type register 0x28
	uint32_t RESERVED1[2];
	volatile uint32_t AHB1ENR;			//RCC AHB1 peripheral clock enable register 0x30
	volatile uint32_t AHB2ENR;			//RCC AHB2 peripheral clock enable register 0x34
	volatile uint32_t AHB3ENR;			//RCC AHB3 peripheral clock enable register 0x38
	uint32_t RESERVED2;
	volatile uint32_t APB1ENR;			//RCC APB1 peripheral clock enable register 0x40
	volatile uint32_t APB2ENR;			//RCC APB2 peripheral clock enable register 0x44
	uint32_t RESERVED3[2];
	volatile uint32_t AHB1LPENR;		//RCC AHB1 peripheral clock enable in low power mode register 0x50
	volatile uint32_t AHB2LPENR;		//RCC AHB2 peripheral clock enable in low power mode register 0x54
	volatile uint32_t AHB3LPENR;		//RCC AHB3 peripheral clock enable in low power mode register 0x58
	uint32_t RESERVED4;
	volatile uint32_t APB1LPENR;		//RCC APB1 peripheral clock enable in low power mode register 0x60
	volatile uint32_t APB2LPENR;		//RCC APB2 peripheral clock enable in low power mode register 0x64
	uint32_t RESERVED5[2];
	volatile uint32_t BDCR;				//RCC Backup domain control register 0x70
	volatile uint32_t CSR;				//RCC clock control & status register 0x74
	uint32_t RESERVED6[2];
	volatile uint32_t SSCGR;			//RCC spread spectrum clock generation register 0x80
	volatile uint32_t PLLI2SCFGR;		//RCC PLLI2S configuration register 0x84
	volatile uint32_t PLLSAICFGR;		//RCC PLL configuration register 0x88
	volatile uint32_t DCKCFGR;			//RCC dedicated clock configuration register 0x8C
	volatile uint32_t CKGATENR;			//RCC clocks gated enable register 0x90
	volatile uint32_t DCKCFGR2;			//RCC dedicated clocks configuration register 2 0x94

}RCC_RegDef_t;

//EXTI
typedef struct				//COMO LOS TIPOS SON DE 32 bits el offset ira aumentando de 0x04 en 0x04
{
	volatile uint32_t IMR;				//EXTI Interrupt mask register				0x00
	volatile uint32_t EMR;				//EXTI Event mask register					0x04
	volatile uint32_t RTSR;				//EXTI Rising trigger selection register 	0x08
	volatile uint32_t FTSR;				//EXTI Falling trigger selection register	0x0C
	volatile uint32_t SWIER;			//EXTI Software interrupt event register	0x10
	volatile uint32_t PR;				//EXTI Pending register						0x14
}EXTI_RegDef_t;

//SYSCFG
typedef struct
{
	volatile uint32_t MEMRMP;			//SYSCFG memory remap register 							0x00
	volatile uint32_t PMC;				//SYSCFG peripheral mode configuration register			0x04
	volatile uint32_t EXTICR[4];		//SYSCFG external interrupt configuration register 1	0x08 - 0x14
	uint32_t RESERVED1[2];
	volatile uint32_t CMPCR;			//SYSCFG Compensation cell control register				0x18
	uint32_t RESERVED2[2];
	volatile uint32_t CFGR;				//SYSCFG configuration register 						0x2C
}SYSCFG_RegDef_t;

//ADC
typedef struct
{
	volatile uint32_t SR;				//ADC status register							0x00
	volatile uint32_t CR1;				//ADC control register 1						0x04
	volatile uint32_t CR2;				//ADC control register 2						0x08
	volatile uint32_t SMPR1;			//ADC sample time register 1					0x0C
	volatile uint32_t SMPR2;			//ADC sample time register 2					0x10
	volatile uint32_t JOFR1;			//ADC injected channel data offset register 1	0x14
	volatile uint32_t JOFR2;			//ADC injected channel data offset register 2	0x18
	volatile uint32_t JOFR3;			//ADC injected channel data offset register 3	0x1C
	volatile uint32_t JOFR4;			//ADC injected channel data offset register 4	0x20
	volatile uint32_t HTR;				//ADC watchdog higher threshold register		0x24
	volatile uint32_t LTR;				//ADC watchdog lower threshold register			0x28
	volatile uint32_t SQR1;				//ADC regular sequence register 1				0x2C
	volatile uint32_t SQR2;				//ADC regular sequence register 2				0x30
	volatile uint32_t SQR3;				//ADC regular sequence register 3				0x34
	volatile uint32_t JSQR;				//ADC injected sequence register				0x38
	volatile uint32_t JDR1;				//ADC injected data register 1					0x3C
	volatile uint32_t JDR2;				//ADC injected data register 2					0x40
	volatile uint32_t JDR3;				//ADC injected data register 3					0x44
	volatile uint32_t JDR4;				//ADC injected data register 4					0x48
	volatile uint32_t DR;				//ADC regular data register						0x4C

}ADC_RegDef_t;

typedef struct{
	volatile uint32_t CSR;				//ADC Common status register									0x00
	volatile uint32_t CCR;				//ADC common control register									0x04
	volatile uint32_t CDR;				//ADC common regular data register for dual and triple modes	0x08

}ADC_C_RegDef_t;


//SPI
typedef struct{
	volatile uint32_t CR1;				//SPI control register 1 			0x00
	volatile uint32_t CR2;				//SPI control register 2 			0x04
	volatile uint32_t SR;				//SPI status register 				0x08
	volatile uint32_t DR;				//SPI data register 				0x0C
	volatile uint32_t CRCPR;			//SPI CRC polynomial register 		0x10
	volatile uint32_t RXCRCR;			//SPI RX CRC register 				0x14
	volatile uint32_t TXCRCR;			//SPI TX CRC register 				0x18
	volatile uint32_t I2SCFGR;			//SPI_I2S configuration register 	0x1C
	volatile uint32_t I2SPR;			//SPI_I2S prescaler register 		0x20

}SPI_RegDef_t;

//I2C
typedef struct{
	volatile uint32_t CR1;				//I2C control register 1 			0x00
	volatile uint32_t CR2;				//I2C control register 2 			0x04
	volatile uint32_t OAR1;				//I2C own address register 1		0x08
	volatile uint32_t OAR2;				//I2C own address register 2		0x0C
	volatile uint32_t DR;				//I2C data register			 		0x10
	volatile uint32_t SR1;				//I2C status register 1				0x14
	volatile uint32_t SR2;				//I2C status register 2				0x18
	volatile uint32_t CCR;				//I2C_clock control register	 	0x1C
	volatile uint32_t TRISE;			//I2C_TRISE register		 		0x20
	volatile uint32_t FLTR;				//I2C_FLTR register			 		0x24
}I2C_RegDef_t;

//USART
typedef struct{
	volatile uint32_t SR;				//USART Status register 					0x00
	volatile uint32_t DR;				//USART Data register	 					0x04
	volatile uint32_t BRR;				//USART Baud rate register					0x08
	volatile uint32_t CR1;				//USART Control register 1					0x0C
	volatile uint32_t CR2;				//USART Control register 2	 				0x10
	volatile uint32_t CR3;				//USART Control register 3					0x14
	volatile uint32_t GTPR;				//USART Guard time and prescaler register	0x18

}USART_RegDef_t;

/*
 * PERIPHERAL DEFINITIONS (PERIPHERAL BASE ADDRESSES TYPECASTED TO xxx_RegDef_t)
 */
#define GPIOA					((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB					((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC					((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD					((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE					((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF					((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG					((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH					((GPIO_RegDef_t*)GPIOH_BASEADDR)

#define RCC						((RCC_RegDef_t*)RCC_BASEADDR)
#define EXTI					((EXTI_RegDef_t*)EXTI_BASEADDR)
#define SYSCFG					((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

#define ADC1					((ADC_RegDef_t*)ADC1_BASEADDR)
#define ADC2					((ADC_RegDef_t*)ADC2_BASEADDR)
#define ADC3					((ADC_RegDef_t*)ADC3_BASEADDR)
#define ADC_C					((ADC_C_RegDef_t*)C_ADC_BASEADDR)


#define SPI1					((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2					((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3					((SPI_RegDef_t*)SPI3_BASEADDR)

#define I2C1					((I2C_RegDef_t*)I2C1_BASEADDR)
#define I2C2					((I2C_RegDef_t*)I2C2_BASEADDR)
#define I2C3					((I2C_RegDef_t*)I2C3_BASEADDR)

#define USART1					((USART_RegDef_t*)USART1_BASEADDR)
#define USART2					((USART_RegDef_t*)USART2_BASEADDR)
#define USART3					((USART_RegDef_t*)USART3_BASEADDR)
#define UART4					((USART_RegDef_t*)UART4_BASEADDR)
#define UART5					((USART_RegDef_t*)UART5_BASEADDR)
#define USART6					((USART_RegDef_t*)USART6_BASEADDR)

/*
 * Clock Enable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_EN()			(RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN()			(RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN()			(RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN()			(RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN()			(RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN()			(RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN()			(RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN()			(RCC->AHB1ENR |= (1 << 7))

/*
 * Clock Enable Macros for ADCx peripherals
 */
#define ADC1_PCLK_EN()			(RCC->APB2ENR |= (1 << 8))
#define ADC2_PCLK_EN()			(RCC->APB2ENR |= (1 << 9))
#define ADC3_PCLK_EN()			(RCC->APB2ENR |= (1 << 10))

/*
 * Clock Enable Macros for I2Cx peripherals
 */
#define I2C1_PCLK_EN()			(RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()			(RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()			(RCC->APB1ENR |= (1 << 23))

/*
 * Clock Enable Macros for SP1x peripherals
 */
#define SPI1_PCLK_EN()			(RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()			(RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()			(RCC->APB1ENR |= (1 << 15))

/*
 * Clock Enable Macros for USARTx peripherals
 */
#define USART1_PCLK_EN()			(RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN()			(RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()			(RCC->APB1ENR |= (1 << 18))
#define UART4_PCLK_EN()				(RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()				(RCC->APB1ENR |= (1 << 20))
#define USART6_PCLK_EN()			(RCC->APB2ENR |= (1 << 5))

/*
 * Clock Enable Macros for SYSCFG peripherals
 */
#define SYSCFG_PCLK_EN()			(RCC->APB2ENR |= (1 << 14))

/*
 * Clock Disable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 7))

/*
 * Clock Disable Macros for ADCx peripherals
 */
#define ADC1_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 8))
#define ADC2_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 9))
#define ADC3_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 10))

/*
 * Clock Disable Macros for I2Cx peripherals
 */
#define I2C1_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 23))

/*
 * Clock Disable Macros for SP1x peripherals
 */
#define SPI1_PCLK_()			(RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 15))

/*
 * Clock Disable Macros for USARTx peripherals
 */
#define USART1_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 18))
#define UART4_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 19))
#define UART5_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 20))
#define USART6_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 5))

/*
 * Clock Disable Macros for SYSCFG peripherals
 */
#define SYSCFG_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 14))

/*
 * Macros to reset GPIOx peripherals
 */
#define GPIOA_REG_RESET()			do{	(RCC->AHB1RSTR |= (1 << 0));	(RCC->AHB1RSTR &= ~(1 << 0));}while(0)
#define GPIOB_REG_RESET()			do{	(RCC->AHB1RSTR |= (1 << 1));	(RCC->AHB1RSTR &= ~(1 << 1));}while(0)
#define GPIOC_REG_RESET()			do{	(RCC->AHB1RSTR |= (1 << 2));	(RCC->AHB1RSTR &= ~(1 << 2));}while(0)
#define GPIOD_REG_RESET()			do{	(RCC->AHB1RSTR |= (1 << 3));	(RCC->AHB1RSTR &= ~(1 << 3));}while(0)
#define GPIOE_REG_RESET()			do{	(RCC->AHB1RSTR |= (1 << 4));	(RCC->AHB1RSTR &= ~(1 << 4));}while(0)
#define GPIOF_REG_RESET()			do{	(RCC->AHB1RSTR |= (1 << 5));	(RCC->AHB1RSTR &= ~(1 << 5));}while(0)
#define GPIOG_REG_RESET()			do{	(RCC->AHB1RSTR |= (1 << 6));	(RCC->AHB1RSTR &= ~(1 << 6));}while(0)
#define GPIOH_REG_RESET()			do{	(RCC->AHB1RSTR |= (1 << 7));	(RCC->AHB1RSTR &= ~(1 << 7));}while(0)

#define GPIO_BASEADDR_TO_CODE(x)	((x==GPIOA) ? 0 :\
									(x==GPIOB) ? 1 :\
									(x==GPIOC) ? 2 :\
									(x==GPIOD) ? 3 :\
									(x==GPIOE) ? 4 :\
									(x==GPIOF) ? 5 :\
									(x==GPIOG) ? 6 :\
									(x==GPIOH) ? 7 :0)

/*
 * Macros to reset GPIOx peripherals
 */
#define ADC_REG_RESET()			do{	(RCC->APB2RSTR |= (1 << 8));	(RCC->APB2RSTR &= ~(1 << 8));}while(0)

/*
 * MACROS for all the possible priority levels
 */
#define NVIC_IRQ_PRI0		0
#define NVIC_IRQ_PRI1		1
#define NVIC_IRQ_PRI2		2
#define NVIC_IRQ_PRI3		3
#define NVIC_IRQ_PRI4		4
#define NVIC_IRQ_PRI5		5
#define NVIC_IRQ_PRI6		6
#define NVIC_IRQ_PRI7		7
#define NVIC_IRQ_PRI8		8
#define NVIC_IRQ_PRI9		9
#define NVIC_IRQ_PRI10		10
#define NVIC_IRQ_PRI11		11
#define NVIC_IRQ_PRI12		12
#define NVIC_IRQ_PRI13		13
#define NVIC_IRQ_PRI14		14
#define NVIC_IRQ_PRI15		15



//somo generic macros
#define ENABLE 			1
#define	DISABLE			0
#define SET				ENABLE
#define RESET			DISABLE

#define GPIO_PIN_SET		SET
#define GPIO_PIN_RESET		RESET
#define FLAG_RESET			RESET
#define FLAG_SET			SET

/*
 * IRQ (Interrupts request) Numbers of STM32F446 MCU
 */

#define IRQ_NO_EXTI0 		6
#define IRQ_NO_EXTI1 		7
#define IRQ_NO_EXTI2 		8
#define IRQ_NO_EXTI3 		9
#define IRQ_NO_EXTI4 		10
#define IRQ_NO_EXTI9_5 		23
#define IRQ_NO_EXTI15_10 	40

#define IRQ_NO_SPI1			35
#define IRQ_NO_SPI2			36
#define IRQ_NO_SPI3			51

#define IRQ_NO_I2C1_EV		31
#define IRQ_NO_I2C1_ER		32
#define IRQ_NO_I2C2_EV		33
#define IRQ_NO_I2C2_ER		34
#define IRQ_NO_I2C3_EV		72
#define IRQ_NO_I2C3_ER		73

#define USART_NO_USART1		37
#define USART_NO_USART2		38
#define USART_NO_USART3		39
#define USART_NO_USART4		52
#define USART_NO_USART5		53


/**************************************************************
 * Bit position definitions of SPI peripheral
 **************************************************************/
/*
 * Bit position definitions CR1
 */
#define SPI_CR1_CPHA		0
#define SPI_CR1_CPOL		1
#define SPI_CR1_MSTR		2
#define SPI_CR1_BR			3
#define SPI_CR1_SPE			6
#define SPI_CR1_LSBFIRST	7
#define SPI_CR1_SSI			8
#define SPI_CR1_SSM			9
#define SPI_CR1_RXONLY		10
#define SPI_CR1_DFF			11
#define SPI_CR1_CRCNEXT		12
#define SPI_CR1_CRCEN		13
#define SPI_CR1_BIDIOE		14
#define SPI_CR1_BIDIMODE	15

/*
 * Bit position definitions CR2
 */
#define SPI_CR2_RXDMAEN		0
#define SPI_CR2_TXDMAEN		1
#define SPI_CR2_SSOE		2
#define SPI_CR2_FRF			4
#define SPI_CR2_ERRIE		5
#define SPI_CR2_RXNEIE		6
#define SPI_CR2_TXNEIE		7

/*
 * Bit position definitions SR
 */
#define SPI_SR_RXNE			0
#define SPI_SR_TXE			1
#define SPI_SR_CHSIDE		2
#define SPI_SR_UDR			3
#define SPI_SR_CRCERR		4
#define SPI_SR_MODF			5
#define SPI_SR_OVR			6
#define SPI_SR_BSY			7
#define SPI_SR_FRE			8



/**************************************************************
 * Bit position definitions of I2C peripheral
 **************************************************************/
/*
 * Bit position definitions CR1
 */
#define I2C_CR1_PE			0
#define I2C_CR1_NOSTRETCH	7
#define I2C_CR1_START		8
#define I2C_CR1_STOP		9
#define I2C_CR1_ACK			10
#define I2C_CR1_SWRST		15


/*
 * Bit position definitions CR2
 */
#define I2C_CR2_FREQ		0
#define I2C_CR2_ITERREN		8
#define I2C_CR2_ITEVTEN		9
#define I2C_CR2_ITBUFEN		10

/*
 * Bit position definitions SR1
 */
#define I2C_SR1_SB			0
#define I2C_SR1_ADDR		1
#define I2C_SR1_BTF			2
#define I2C_SR1_ADD10		3
#define I2C_SR1_STOPF		4
#define I2C_SR1_RXNE		6
#define I2C_SR1_TXE			7
#define I2C_SR1_BERR		8
#define I2C_SR1_ARLO		9
#define I2C_SR1_AF			10
#define I2C_SR1_OVR			11
#define I2C_SR1_TIMEOUT		14

/*
 * Bit position definitions SR2
 */
#define I2C_SR2_MSL			0
#define I2C_SR2_BUSY		1
#define I2C_SR2_TRA			2
#define I2C_SR2_GENCALL		4
#define I2C_SR2_DUALF		7

/*
 * Bit position definitions CCR
 */
#define I2C_CCR_CCR			0
#define I2C_CCR_DUTY		14
#define I2C_CCR_FS			15


/**************************************************************
 * Bit position definitions of USART peripheral
 **************************************************************/
#define USART_SR_PE		0
#define USART_SR_FE		1
#define USART_SR_NE		2
#define USART_SR_ORE	3
#define USART_SR_IDLE	4
#define USART_SR_RXNE	5
#define USART_SR_TC		6
#define USART_SR_TXE	7
#define USART_SR_LDB	8
#define USART_SR_CTS	9


#define USART_CR1_SBK		0
#define USART_CR1_RWU		1
#define USART_CR1_RE		2
#define USART_CR1_TE		3
#define USART_CR1_IDLEIE	4
#define USART_CR1_RXNEIE	5
#define USART_CR1_TCIE		6
#define USART_CR1_TXEIE		7
#define USART_CR1_PEIE		8
#define USART_CR1_PS		9
#define USART_CR1_PCE		10
#define USART_CR1_WAKE		11
#define USART_CR1_M			12
#define USART_CR1_UE		13
#define USART_CR1_OVER8		15

#define USART_CR2_ADD		0
#define USART_CR2_LBDL		5
#define USART_CR2_LBDIE		6
#define USART_CR2_LBCL		8
#define USART_CR2_CPHA		9
#define USART_CR2_CPOL		10
#define USART_CR2_CLKEN		11
#define USART_CR2_STOP		12
#define USART_CR2_LINEN		14

#define USART_CR3_EIE		0
#define USART_CR3_IREN		1
#define USART_CR3_IRLP		2
#define USART_CR3_HDSEL		3
#define USART_CR3_NACK		4
#define USART_CR3_SCEN		5
#define USART_CR3_DMAR		6
#define USART_CR3_DMAT		7
#define USART_CR3_RTSE		8
#define USART_CR3_CTSE		9
#define USART_CR3_CTSIE		10
#define USART_CR3_ONEBIT	11

/**************************************************************
 * Bit position definitions of ADC peripheral
 **************************************************************/
#define ADC_SR_AWD			0
#define ADC_SR_EOC			1
#define ADC_SR_JEOC			2
#define ADC_SR_JSTRT		3
#define ADC_SR_STRT			4
#define ADC_SR_OVR			5

#define ADC_CR1_AWDCH		0
#define ADC_CR1_EOCIE		5
#define ADC_CR1_AWDIE		6
#define ADC_CR1_JEOCIE		7
#define ADC_CR1_SCAN		8
#define ADC_CR1_AWDSGL		9
#define ADC_CR1_JAUTO		10
#define ADC_CR1_DISCEN		11
#define ADC_CR1_JDISCEN		12
#define ADC_CR1_DISCNUM		13
#define ADC_CR1_JAWDEN		22
#define ADC_CR1_AWDEN		23
#define ADC_CR1_RES			24
#define ADC_CR1_OVRIE		26

#define ADC_CR2_ADON		0
#define ADC_CR2_CONT		1
#define ADC_CR2_DMA			8
#define ADC_CR2_DDS			9
#define ADC_CR2_EOCS		10
#define ADC_CR2_ALIGN		11
#define ADC_CR2_JEXTEN		16
#define ADC_CR2_JSWSTART	22
#define ADC_CR2_EXTSEL		24
#define ADC_CR2_EXTEN		28
#define ADC_CR2_SWSTART		30

#define ADC_SQR1_SQ13		0
#define ADC_SQR1_SQ14		5
#define ADC_SQR1_SQ15		10
#define ADC_SQR1_SQ16		15
#define ADC_SQR1_L			20

#define ADC_SQR2_SQ7		0
#define ADC_SQR2_SQ8		5
#define ADC_SQR2_SQ9		10
#define ADC_SQR2_SQ10		15
#define ADC_SQR2_SQ11		20
#define ADC_SQR2_SQ12		25

#define ADC_SQR3_SQ1		0
#define ADC_SQR3_SQ2		5
#define ADC_SQR3_SQ3		10
#define ADC_SQR3_SQ4		15
#define ADC_SQR3_SQ5		20
#define ADC_SQR3_SQ6		25

#define ADC_CCR_MULTI		0
#define ADC_CCR_DELAY		8
#define ADC_CCR_DDS			13
#define ADC_CCR_DMA			14
#define ADC_CCR_ADCPRE		16
#define ADC_CCR_VBAT		22
#define ADC_CCR_TSVREFE		23

#define ADC_SMPR1_SPM10		0
#define ADC_SMPR1_SPM11		3
#define ADC_SMPR1_SPM12		6
#define ADC_SMPR1_SPM13		9
#define ADC_SMPR1_SPM14		12
#define ADC_SMPR1_SPM15		15
#define ADC_SMPR1_SPM16		18
#define ADC_SMPR1_SPM17		21
#define ADC_SMPR1_SPM18		24

#define ADC_SMPR2_SPM0		0
#define ADC_SMPR2_SPM1		3
#define ADC_SMPR2_SPM2		6
#define ADC_SMPR2_SPM3		9
#define ADC_SMPR2_SPM4		12
#define ADC_SMPR2_SPM5		15
#define ADC_SMPR2_SPM6		18
#define ADC_SMPR2_SPM7		21
#define ADC_SMPR2_SPM8		24
#define ADC_SMPR2_SPM9		27

/*
 * Macros to reset I2Cx peripherals
 */
//IR AL RM apartado RCC->PERIFERICO EN EL QUE ESTA MONTADO (APB1, APB2, AHB1)-> BUSCAR EL RSTR
#define I2C1_REG_RESET()		do{	(RCC->APB1RSTR |= (1 << 21));	(RCC->APB1RSTR &= ~(1 << 21));}while(0)
#define I2C2_REG_RESET()		do{	(RCC->APB1RSTR |= (1 << 22));	(RCC->APB1RSTR &= ~(1 << 22));}while(0)
#define I2C3_REG_RESET()		do{	(RCC->APB1RSTR |= (1 << 23));	(RCC->APB1RSTR &= ~(1 << 23));}while(0)

/*
 * Macros to reset SPIx peripherals
 */
#define SPI1_REG_RESET()		do{	(RCC->APB2RSTR |= (1 << 12));	(RCC->APB2RSTR &= ~(1 << 12));}while(0)
#define SPI2_REG_RESET()		do{	(RCC->APB1RSTR |= (1 << 14));	(RCC->APB1RSTR &= ~(1 << 14));}while(0)
#define SPI3_REG_RESET()		do{	(RCC->APB1RSTR |= (1 << 15));	(RCC->APB1RSTR &= ~(1 << 15));}while(0)

/*
 * Macros to reset USARTx peripherals
 */
#define USART1_REG_RESET()		do{	(RCC->APB2RSTR |= (1 << 4));	(RCC->APB2RSTR &= ~(1 << 4));}while(0)
#define USART2_REG_RESET()		do{	(RCC->APB1RSTR |= (1 << 17));	(RCC->APB1RSTR &= ~(1 << 17));}while(0)
#define USART3_REG_RESET()		do{	(RCC->APB1RSTR |= (1 << 18));	(RCC->APB1RSTR &= ~(1 << 18));}while(0)
#define UART4_REG_RESET()		do{	(RCC->APB1RSTR |= (1 << 19));	(RCC->APB1RSTR &= ~(1 << 19));}while(0)
#define UART5_REG_RESET()		do{	(RCC->APB1RSTR |= (1 << 20));	(RCC->APB1RSTR &= ~(1 << 20));}while(0)
#define USART6_REG_RESET()		do{	(RCC->APB2RSTR |= (1 << 5));	(RCC->APB2RSTR &= ~(1 << 5));}while(0)

#include "stm32f446xx_adc_driver.h"
#include "stm32f446xx_gpio_driver.h"
#include "stm32f446xx_usart_driver.h"
#include "stm32f446xx_rcc_driver.h"



#endif /* INC_STM32F446XX_H_ */
