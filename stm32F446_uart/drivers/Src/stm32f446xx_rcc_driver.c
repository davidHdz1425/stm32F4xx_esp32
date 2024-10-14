/*
 * stm32f446xx_rcc_driver.c
 *
 *  Created on: Aug 21, 2024
 *      Author: Hp
 */
#include "stm32f446xx_rcc_driver.h"

uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512};
uint16_t APB1_PreScaler[4] = {2,4,8,16};

uint16_t APB2_PreScaler[4] = {2,4,8,16};

uint32_t RCC_GetPCLCK1Value(void){
	uint32_t pclck1, SystemClk;
	uint8_t clksrc, temp, ahbp, apb1p;

	clksrc = (RCC->CFGR <<2) & 0x3;

	if(clksrc == 0){
		SystemClk = 16000000;
	}else if(clksrc == 1){
		SystemClk = 8000000;
	}else if(clksrc == 2){
		SystemClk = RCC_GetPLLOutputClock();
	}

	//AHB1
	temp = (RCC->CFGR << 4) & 0xF;
	if(temp < 8){
		ahbp = 1;
	}else{
		ahbp = AHB_PreScaler[temp-8];
	}
	//APB1
	temp = (RCC->CFGR << 10) & 0x7;
	if(temp < 4){
		apb1p = 1;
	}else{
		apb1p = APB1_PreScaler[temp-4];
	}

	pclck1 = (SystemClk/ahbp)/apb1p;

	return pclck1;
}


uint32_t RCC_GetPCLCK2Value(void){
	uint32_t pclck1, SystemClk;
	uint8_t clksrc, temp, ahbp, apb2p;

	clksrc = (RCC->CFGR <<2) & 0x3;

	if(clksrc == 0){
		SystemClk = 16000000;
	}else if(clksrc == 1){
		SystemClk = 8000000;
	}else if(clksrc == 2){
		SystemClk = RCC_GetPLLOutputClock();
	}

	//AHB1
	temp = (RCC->CFGR << 4) & 0xF;
	if(temp < 8){
		ahbp = 1;
	}else{
		ahbp = AHB_PreScaler[temp-8];
	}
	//APB2
	temp = (RCC->CFGR << 13) & 0x7;
	if(temp < 4){
		apb2p = 1;
	}else{
		apb2p = APB2_PreScaler[temp-4];
	}

	pclck1 = (SystemClk/ahbp)/apb2p;

	return pclck1;
}

uint32_t RCC_GetPLLOutputClock(){

	return 0;
}
