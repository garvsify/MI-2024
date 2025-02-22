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

uint8_t Reset_Tap_Tempo_Timer(void){

	Set_Status_Bit(&statuses, Tap_Tempo_Checking_Disabled);
	Clear_Status_Bit(&statuses, Software_Tap_Tempo_Checking_Timer_Has_Timed_Out);
	Set_Status_Bit(&statuses, Software_Tap_Tempo_Checking_Timer_Is_Running);

	return 1;
}
