/*
 * stm32f446xx_rcc_driver.h
 *
 *      Author: David Alejandro Hernandez Rios
 */

#ifndef INC_STM32F446XX_RCC_DRIVER_H_
#define INC_STM32F446XX_RCC_DRIVER_H_

#include "stm32f446xx.h"

// Return the APB1 clock value
uint32_t RCC_GetPCLCK1Value(void);

// Return the APB2 clock value
uint32_t RCC_GetPCLCK2Value(void);

uint32_t RCC_GetPLLOutputClock(void);


#endif /* INC_STM32F446XX_RCC_DRIVER_H_ */
