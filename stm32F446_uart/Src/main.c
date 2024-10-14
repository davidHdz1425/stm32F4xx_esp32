#include "stm32f446xx.h"
#include <string.h>
#include <stdio.h>

char msg[35];
char msg_Temp[10];
char msg_Voltaje[10];
char msg_Corriente[70];

USART_Handle_t usart2_handle;
ADC_Handle_t adc1_handle, adc2_handle, adc3_handle;


#define VREFINIT	1.21
#define ADCMAX		4095
#define V25			.76
#define AVG_SLOPE	.0025
#define RESISTOR 	10

void delay(){
	for(uint32_t i=0; i<5000000/2; i++);
}

void USART2_Init(void){
	usart2_handle.pUSARTx = USART2;
	usart2_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
	usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart2_handle.USART_Config.USART_Mode = USART_MODE_ONLY_TX;
	usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
	usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	USART_Init(&usart2_handle);

}

void USART2_GPIOInit(void){
	GPIO_Handle_t usart_gpios;

	usart_gpios.pGPIOx = GPIOA;
	usart_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	usart_gpios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	usart_gpios.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	usart_gpios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	usart_gpios.GPIO_PinConfig.GPIO_PinAltFunMode = 7;

	// USART TX
	usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_2;
	GPIO_Init(&usart_gpios);

	// USART RX
	usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
	GPIO_Init(&usart_gpios);

}

void ADC_GPIOInit(void){
	GPIO_Handle_t adc_gpios;
	adc_gpios.pGPIOx = GPIOA;
	adc_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ANALOG;


	// Sensor 1
	adc_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_1;
	GPIO_Init(&adc_gpios);

	// Sensor 0
	adc_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIO_Init(&adc_gpios);
}

void ADCs_Init(){
	// SENSOR DE TEMPERATURA
	adc1_handle.pADCx = ADC1;
	adc1_handle.pC_ADC = ADC_C;
	ADC_Init(&adc1_handle);


	//
	adc2_handle.pADCx = ADC2;
	ADC_Init(&adc2_handle);

	adc3_handle.pADCx = ADC3;
	ADC_Init(&adc3_handle);

}


int main (void){
	float temperature=0, voltaje_temp_sens=0, voltaje_pila_AA=0, battery_current = 0, baterry_voltage = 0;
	uint32_t adc_value_s1=0, adc_value_s2=0;

	// UART2 2 Init
	USART2_GPIOInit();
	USART2_Init();
	USART_PeripheralControl(USART2, ENABLE);

	// ADCs Init
	ADC_GPIOInit();
	ADCs_Init();

	ADC_StartConversion(ADC1);
	ADC_StartConversion(ADC2);
	ADC_StartConversion(ADC3);

	while(1){
		// Temperature sensor
		adc_value_s1 = ADC_ReadValue(ADC1);
		voltaje_temp_sens = (3.3*((float)adc_value_s1))/4095;
		temperature = ((voltaje_temp_sens - V25)/AVG_SLOPE) + 25;


		// Voltage meter  (Nivel de bateria)
		adc_value_s2 = ADC_ReadValue(ADC2);
		voltaje_pila_AA = (((float)adc_value_s2)*3.3)/4095;
		baterry_voltage = voltaje_pila_AA;

		// Current meter
		battery_current=(voltaje_pila_AA/RESISTOR)*1000;

		// Concatenate string
		sprintf(msg, "%.2f %.2f %.2f",temperature, baterry_voltage, battery_current);

		// Send Data
		USART_SendData(&usart2_handle, (uint8_t *)msg, strlen(msg));
		delay();

	}
}
