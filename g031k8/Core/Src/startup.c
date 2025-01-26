#include <stdint.h>
#include "startup.h"

uint8_t Startup(void){

	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE); //make sure the overflow (update) interrupt is enabled for TIM2
	__HAL_TIM_ENABLE_IT(&htim16, TIM_IT_UPDATE); //make sure the overflow (update) interrupt is enabled for TIM16

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions);

	//WAIT
	while(initial_ADC_conversion_complete == NO){}; //wait while first ADC conversion is ongoing

	// re-initialise all values in delay line storage array to middle value of wave, as they are initialised to 0 on startup
		for(uint16_t i = 0; i < FINAL_INDEX + 2; i++){ //513

			if(params.waveshape == SQUARE_MODE){

				delay_line.duty_delay_line_storage_array[i] = PWM_DUTY_VALUE_MAX - ((params.depth * PWM_DUTY_VALUE_MAX) >> DEPTH_ADC_RESOLUTION);
			}
			else{

				delay_line.duty_delay_line_storage_array[i] = PWM_DUTY_VALUE_MAX - (((params.depth * PWM_DUTY_VALUE_MAX) >> DEPTH_ADC_RESOLUTION) >> 1);
			}
	}

	//SET DEFAULT PIN STATES
	HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1); //latch high the debounced o/p
	HAL_GPIO_WritePin(HACK_POT_HIGH_GPIO_Port, HACK_POT_HIGH_Pin, 1);
	HAL_GPIO_WritePin(HACK_POT_LOW_GPIO_Port, HACK_POT_LOW_Pin, 0);

	return 1;
}

