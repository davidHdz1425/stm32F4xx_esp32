/*
 * stm32f446xx_gpio_driver.h
 *
 *      Author: David Alejandro Hernandez Rios
 */

#ifndef INC_STM32F446XX_GPIO_DRIVER_H_
#define INC_STM32F446XX_GPIO_DRIVER_H_

#include "stm32f446xx.h"




/*
 * This is a configuration structure for a GPIO pin
 */
typedef struct
{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;					/* possible values from @GPIO_PIN_MODES */
	uint8_t GPIO_PinSpeed;					/* possible values from @GPIO_PIN_SPEED */
	uint8_t GPIO_PinPuPdControl;			/* possible values from @GPIO_PIN_PUPD */
	uint8_t GPIO_PinOPType;					/* possible values from @GPIO_PIN_OTYPE */
	uint8_t GPIO_PinAltFunMode;

}GPIO_PinConfig_t;

/*
 * This is a handle structure for GPIO pin
 */

typedef struct
{
	// POINTER TO THE BASE ADDRESS OF THE GPIO PERIPHERAL
	GPIO_RegDef_t *pGPIOx;				//Contiene la direccion base del puerto GPIO al que pin pertenece
	GPIO_PinConfig_t GPIO_PinConfig;	//Contiene GPIO pin configuration settings

}GPIO_Handle_t;

/*
 * @GPIO_PIN_NUMBERS
 * GPIO pin numbers
 */
#define GPIO_PIN_NO_0		0
#define GPIO_PIN_NO_1		1
#define GPIO_PIN_NO_2		2
#define GPIO_PIN_NO_3		3
#define GPIO_PIN_NO_4		4
#define GPIO_PIN_NO_5		5
#define GPIO_PIN_NO_6		6
#define GPIO_PIN_NO_7		7
#define GPIO_PIN_NO_8		8
#define GPIO_PIN_NO_9		9
#define GPIO_PIN_NO_10		10
#define GPIO_PIN_NO_11		11
#define GPIO_PIN_NO_12		12
#define GPIO_PIN_NO_13		13
#define GPIO_PIN_NO_14		14
#define GPIO_PIN_NO_15		15

/*
 * @GPIO_PIN_MODES
 * GPIO pin posible modes
 */
#define GPIO_MODE_IN			0
#define GPIO_MODE_OUT			1
#define GPIO_MODE_ALTFN			2
#define GPIO_MODE_ANALOG		3
#define GPIO_MODE_IT_FT			4 	//MODO DE ENTRADA (IT) FALLING EDGE TRIGGER(FT)
#define GPIO_MODE_IT_RT			5 	//MODO DE ENTRADA (IT) RISING EDGE TRIGGER (RT)
#define GPIO_MODE_IT_RFT		4 	//MODO DE ENTRADA (IT) RISING FALLING EDGE TRIGGER(FT)


/*
 * @GPIO_PIN_OTYPE
 * GPIO pin possible output types
 */
#define GPIO_OP_TYPE_PP			0	//PUSH PULL
#define GPIO_OP_TYPE_OD			1 	//OPEN DRAIN

/*
 * @GPIO_PIN_SPEED
 * GPIO pin possible output speed register
 */
#define GPIO_SPEED_LOW			0	//SPEED LOW
#define GPIO_SPEED_MEDIUM		1 	//SPEED MED
#define GPIO_SPEED_FAST			2	//SPEED FAST
#define GPIO_SPEED_HIGH			3 	//SPEED HIGH

/*
 * @GPIO_PIN_PUPD
 * GPIO pin possible Pull-up Pull-down confi
 */
#define GPIO_NO_PUPD			0	//NO PULL UP NO PULL DOWN
#define GPIO_PIN_PU				1 	//PULL UP
#define GPIO_PIN_PD				2	//PULL DOWN


/*******************************************************************
 * 				APIs supported by this driver
 * 	For more info about the APIs check the function definitions
 ******************************************************************/

/*
 * Periphercal clock setup
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/*
 * Data read and write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOuputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOuputPort(GPIO_RegDef_t *pGPIOx,uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
 * IRQ Configuration and IRQ Handling
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* INC_STM32F446XX_GPIO_DRIVER_H_ */
