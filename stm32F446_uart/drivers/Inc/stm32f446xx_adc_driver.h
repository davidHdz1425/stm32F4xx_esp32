/*
 * stm32f446xx_adc_driver.h
 *
 *      Author: David Alejandro Hernandez Rios
 */

#ifndef INC_STM32F446XX_ADC_DRIVER_H_
#define INC_STM32F446XX_ADC_DRIVER_H_

#include "stm32f446xx.h"

typedef struct
{
	uint8_t ADC_Channel;
	uint8_t ADC_SamplingCycle;				/* possible values from @GPIO_PIN_MODES */

}ADC_Config_t;

typedef struct{
	ADC_RegDef_t *pADCx;
	ADC_C_RegDef_t *pC_ADC;
	ADC_Config_t ADC_Config;
}ADC_Handle_t;


/*
 * @GADC_Channel
 * ADC channel posible modes
 */
#define ADC_CHANNEL_0			0
#define ADC_CHANNEL_1			1
#define ADC_CHANNEL_2			2
#define ADC_CHANNEL_3			3
#define ADC_CHANNEL_4			4
#define ADC_CHANNEL_5			5
#define ADC_CHANNEL_6			6
#define ADC_CHANNEL_7			7
#define ADC_CHANNEL_8			8
#define ADC_CHANNEL_9			9
#define ADC_CHANNEL_10			10
#define ADC_CHANNEL_11			11
#define ADC_CHANNEL_12			12
#define ADC_CHANNEL_13			13
#define ADC_CHANNEL_14			14
#define ADC_CHANNEL_15			15
#define ADC_CHANNEL_16			16
#define ADC_CHANNEL_17			17
#define ADC_CHANNEL_18			18

/*
 * ADC_SamplingCycle
 * ADC Sampling Cycles possible types
 */
#define ADC_SAMPLING_CYCLE_3	0	// 3 cycles
#define ADC_SAMPLING_CYCLE_15	1 	// 15 cycles
#define ADC_SAMPLING_CYCLE_28	2	// 28 cycles
#define ADC_SAMPLING_CYCLE_56	3 	// 56 cycles
#define ADC_SAMPLING_CYCLE_84	4	// 84 cycles
#define ADC_SAMPLING_CYCLE_112	5 	// 112 cycles
#define ADC_SAMPLING_CYCLE_144	6	// 144 cycles
#define ADC_SAMPLING_CYCLE_480	7 	// 480 cycles

/*******************************************************************
 * 				APIs supported by this driver
 * 	For more info about the APIs check the function definitions
 ******************************************************************/

/*
 * Periphercal clock setup
 */
void ADC_PeriClockControl(ADC_RegDef_t *pADCx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void ADC_Init(ADC_Handle_t *pADCHandle);
void ADC_DeInit(ADC_RegDef_t *pADCx);

/*
 *
 */
void ADC_StartConversion(ADC_RegDef_t *pADCx);
uint32_t ADC_ReadValue(ADC_RegDef_t *pADCx);


#endif /* INC_STM32F446XX_ADC_DRIVER_H_ */
