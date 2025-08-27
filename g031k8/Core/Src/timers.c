//INCLUDES
#include "timers.h"

//FUNCTION DEFINITIONS
uint8_t Start_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t OC_TIM_channel){

	uint8_t ok = HAL_TIM_OC_Start_IT(TIM, OC_TIM_channel); //_IT variant of function
	//means the timer will generate an interrupt on delay_elapsed (CNT = CCR) condition

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Stop_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t OC_TIM_channel){

	uint8_t ok = HAL_TIM_OC_Stop_IT(TIM, OC_TIM_channel);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Start_DMA_M2M_Timer(void){

	Stop_OC_TIM(&htim17, TIM_CHANNEL_1);
	__HAL_TIM_SET_COUNTER(&htim17, 0);
	__HAL_TIM_SET_COMPARE(&htim17, TIM_CHANNEL_1, HTIM17_DMA_M2M_DELAY);
	Start_OC_TIM(&htim17, TIM_CHANNEL_1);

	return 1;
}
