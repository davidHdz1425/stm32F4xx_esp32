/*
 * stm32f446xx_adc_driver.c
 *
 *  Created on: Aug 25, 2024
 *      Author: David Alejandro Hernandez Rios
 */


#include <stm32f446xx_adc_driver.h>


/*
 * Periphercal clock setup
 */
/***************************************************************
 * @FunctionName			- ADC_PeriClockControl
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
void ADC_PeriClockControl(ADC_RegDef_t *pADCx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pADCx == ADC1){
			ADC1_PCLK_EN();
		} else if(pADCx == ADC2){
			ADC2_PCLK_EN();
		} else if(pADCx == ADC3){
			ADC3_PCLK_EN();
		}
	}

	else{
		if(pADCx == ADC1){
			ADC1_PCLK_DI();
		} else if(pADCx == ADC3){
			ADC2_PCLK_DI();
		} else if(pADCx == ADC3){
			ADC3_PCLK_DI();
		}
	}
}

void ADC_Init(ADC_Handle_t *pADCHandle){
	// Enable the peripheral clock
	ADC_PeriClockControl(pADCHandle->pADCx, ENABLE);

	// Especify the resolution
	pADCHandle->pADCx->CR1 |= (0 << ADC_CR1_RES);

	// SCAN activ
	//pADCHandle->pADCx->CR1 |= (1 << ADC_CR1_SCAN);


	//  Discontinuous mode channel count
	pADCHandle->pADCx->CR1 |= (0b011 << ADC_CR1_DISCNUM);

	//Conversion sequence start
	if(pADCHandle->pADCx == ADC1){
		pADCHandle->pC_ADC->CCR |= (1 << ADC_CCR_TSVREFE);
		pADCHandle->pADCx->SQR3 |= (0b10010 << ADC_SQR3_SQ1);
		pADCHandle->pADCx->SMPR1 |= (0b110 << ADC_SMPR1_SPM18);

	}else if(pADCHandle->pADCx == ADC2){
		pADCHandle->pADCx->SQR3 |= (0b00000 << ADC_SQR3_SQ1);
		pADCHandle->pADCx->SMPR2 |= (0b110 << ADC_SMPR2_SPM0);

	}else if(pADCHandle->pADCx == ADC3){
		pADCHandle->pADCx->SQR3 |= (0b00001 << ADC_SQR3_SQ1);
		pADCHandle->pADCx->SMPR2 |= (0b110 << ADC_SMPR2_SPM1);
	}

	//Specify conversion sequence length
	pADCHandle->pADCx->SQR1 |= (0 << ADC_SQR1_L);

	// Enable the ADC
	pADCHandle->pADCx->CR2 |= (1 << ADC_CR2_ADON);

}


void ADC_DeInit(ADC_RegDef_t *pADCx){
	if(pADCx == ADC1){
		ADC_REG_RESET();
	} else if(pADCx == ADC2){
		ADC_REG_RESET();
	} else if(pADCx == ADC3){
		ADC_REG_RESET();
	}
}

void ADC_StartConversion(ADC_RegDef_t *pADCx){
	// Continius conversion
	pADCx->CR2 |= (1<<ADC_CR2_CONT);

	//Start ADC conversion
	pADCx->CR2 |= (1 << ADC_CR2_SWSTART);
}

uint32_t ADC_ReadValue(ADC_RegDef_t *pADCx){
	//Wait for the conversion to be complete
	while(!(pADCx->SR & (1 << ADC_SR_EOC)));

	//Read converted result
	return(pADCx->DR);
}
