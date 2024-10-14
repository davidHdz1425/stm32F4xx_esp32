/*
 * stm32f446xx_gpio_driver.c
 *
 *  Created on: Jul 29, 2024
 *      Author: Hp
 */


#include "stm32f446xx_gpio_driver.h"


/*
 * Periphercal clock setup
 */
/***************************************************************
 * @FunctionName			- GPIO_Init
 *
 * @brief					- This fuction enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]				- *pGPIOx base address of the GPIO peripheral
 * @param[in]				- EnorDi ENABLE or DISABLE macros
 *
 * @return					- return type: none
 *
 * @note					- espcial note to use API: none
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pGPIOx == GPIOA){
			GPIOA_PCLK_EN();
		} else if(pGPIOx == GPIOB){
			GPIOB_PCLK_EN();
		} else if(pGPIOx == GPIOC){
			GPIOC_PCLK_EN();
		} else if(pGPIOx == GPIOD){
			GPIOD_PCLK_EN();
		} else if(pGPIOx == GPIOE){
			GPIOE_PCLK_EN();
		} else if(pGPIOx == GPIOF){
			GPIOF_PCLK_EN();
		} else if(pGPIOx == GPIOG){
			GPIOG_PCLK_EN();
		} else if(pGPIOx == GPIOH){
			GPIOH_PCLK_EN();
		}
	}

	else{
		if(pGPIOx == GPIOA){
			GPIOA_PCLK_DI();
		} else if(pGPIOx == GPIOB){
			GPIOB_PCLK_DI();
		} else if(pGPIOx == GPIOC){
			GPIOC_PCLK_DI();
		} else if(pGPIOx == GPIOD){
			GPIOD_PCLK_DI();
		} else if(pGPIOx == GPIOE){
			GPIOE_PCLK_DI();
		} else if(pGPIOx == GPIOF){
			GPIOF_PCLK_DI();
		} else if(pGPIOx == GPIOG){
			GPIOG_PCLK_DI();
		} else if(pGPIOx == GPIOH){
			GPIOH_PCLK_DI();
		}
	}
}

/*
 * Init and De-init
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle){
	uint32_t temp=0;

	// Enable the peripheral clock
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	// 1. Mode Config
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG){
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <<(2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber) );

		// First, Clear the two bits that we want to use 11=0x3
		pGPIOHandle->pGPIOx->MODER &= ~(0x3<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		// Load the temp value on the current reguster
		pGPIOHandle->pGPIOx->MODER |= temp;
	}
	else{
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT){
			// 1. Config the register as Falling Trigger (FTSR)
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// Clear RTSR bit
			EXTI->RTSR &= ~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT){
			// 1. Config the register as Rising Trigger (RTSR)
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// Clear FTSR bit
			EXTI->FTSR &= ~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT){
			// 1. Config the register as Rising Falling Trigger (RFTSR)
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// Clear RFTSR bit
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		// 2. Config the GPIO Port in the SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);

		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] = portcode << (temp2*4);

		// 3. Enable the EXTI interrupt using IMR (Interrupt Mask Register)
		EXTI->IMR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}
	// 2. Config the speed
	temp = 0;
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed <<(2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDER &= ~(0x3<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OSPEEDER |= temp;
	// 3. Config Pull up-Pull down setting
	temp = 0;
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl <<(2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->PUPDR |= temp;
	// 4. Config the Output type (Optype)
	temp = 0;
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType <<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER |= temp;
	// 5. Config the alter function
	temp = 0;

	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){
		uint8_t temp1, temp2;
		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber/8;
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber%8;

		//temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4* temp2));
		//pGPIOHandle->pGPIOx->AFR[temp1] |= temp;
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF<<(4* temp2));
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4* temp2));
	}



}
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){
	if(pGPIOx == GPIOA){
		GPIOA_REG_RESET();
	} else if(pGPIOx == GPIOB){
		GPIOB_REG_RESET();
	} else if(pGPIOx == GPIOC){
		GPIOC_REG_RESET();
	} else if(pGPIOx == GPIOD){
		GPIOD_REG_RESET();
	} else if(pGPIOx == GPIOE){
		GPIOE_REG_RESET();
	} else if(pGPIOx == GPIOF){
		GPIOF_REG_RESET();
	} else if(pGPIOx == GPIOG){
		GPIOG_REG_RESET();
	} else if(pGPIOx == GPIOH){
		GPIOH_REG_RESET();
	}
}

/*
 * Data read and write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	uint8_t value;
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);
	return value;
}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){
	uint16_t value;
	value = (uint16_t) pGPIOx->IDR;
	return value;
}
void GPIO_WriteToOuputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value){
	if(Value == GPIO_PIN_SET){
		pGPIOx->ODR |= (1<<PinNumber);
	}
	else{
		pGPIOx->ODR &= ~(1<<PinNumber);
	}
}
void GPIO_WriteToOuputPort(GPIO_RegDef_t *pGPIOx,uint16_t Value){
	pGPIOx->ODR = Value;
}
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	pGPIOx->ODR = pGPIOx->ODR ^ (1<<PinNumber);
}

/*
 * IRQ Configuration and IRQ Handling
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(IRQNumber <= 31){
			// Programm ISER0 register
			*NVIC_ISER0 |= (1<<IRQNumber);

		}else if(IRQNumber > 31 && IRQNumber < 64){
			// Programm ISER1 register
			*NVIC_ISER1 |= (1<<(IRQNumber%32));

		}else if(IRQNumber >= 64  && IRQNumber < 96){
			// Programm ISER2 register
			*NVIC_ISER2 |= (1<<(IRQNumber%64));
		}
	}
	else{
		if(IRQNumber <= 31){
			// Programm ISER0 register
			*NVIC_ICER0 |= (1<<IRQNumber);
		}else if(IRQNumber > 31 && IRQNumber < 64){
			// Programm ISER1 register
			*NVIC_ICER0 |= (1<<(IRQNumber%32));

		}else if(IRQNumber >= 64  && IRQNumber < 96){
			// Programm ISER2 register
			*NVIC_ICER0 |= (1<<(IRQNumber%64));

		}
	}
}

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){
	// 1. Find the IPR register
	uint8_t iprx = IRQNumber/4;
	uint8_t iprx_section = IRQNumber%4;

	uint8_t shift_amount = (8*iprx_section)+ (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + (iprx))|= (IRQPriority << shift_amount);
}

void GPIO_IRQHandling(uint8_t PinNumber){
	// Clear the PR resgiter from the EXTI to the pin number
	if(EXTI->PR & (1<<PinNumber)){
		// Clear
		EXTI->PR |= (1 << PinNumber);
	}
}
