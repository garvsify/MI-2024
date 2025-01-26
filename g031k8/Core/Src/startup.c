#include <stdint.h>
#include "startup.h"

uint8_t Startup(void){

	// re-initialise all values in delay line storage array to 512 (mid scale) as they are initialised to 0 on startup
	for(uint16_t i = 0; i < FINAL_INDEX + 1; i++){

		delay_line.duty_delay_line_storage_array[i] = INITIAL_PWM_VALUE;
	}

	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE); //make sure the overflow (update) interrupt is enabled for TIM2
	__HAL_TIM_ENABLE_IT(&htim16, TIM_IT_UPDATE); //make sure the overflow (update) interrupt is enabled for TIM16

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions);

	//WAIT
	while(initial_ADC_conversion_complete == NO){}; //wait while first ADC conversion is ongoing

	HAL_ADC_Stop_DMA(&hadc1);

	//SET DEFAULT PIN STATES
	HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1); //latch high the debounced o/p
	HAL_GPIO_WritePin(HACK_POT_HIGH_GPIO_Port, HACK_POT_HIGH_Pin, 1);
	HAL_GPIO_WritePin(HACK_POT_LOW_GPIO_Port, HACK_POT_LOW_Pin, 0);

	return 1;
}

