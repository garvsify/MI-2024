#include "TIMx_callbacks.h"

void TIM16_callback(TIM_HandleTypeDef *htim)
{
	__HAL_TIM_SET_COUNTER(&htim16, (uint16_t)TIM16_final_start_value); //this line must go here, or at least very near the beginning!

	//interrupt flag is already cleared by stm32g0xx_it.c

	if(current_waveshape == TRIANGLE_MODE){
		duty = tri_table_one_quadrant[current_one_quadrant_index];
	}
	else if(current_waveshape == SINE_MODE){
		duty = sine_table_one_quadrant[current_one_quadrant_index];
	}
	else if(current_waveshape == SQUARE_MODE){
		duty = 1023;
	}
	if(current_one_quadrant_index == MAX_QUADRANT_INDEX){
		current_quadrant = SECOND_QUADRANT;
	}
	else if(current_one_quadrant_index == MIN_QUADRANT_INDEX){
		current_quadrant = FIRST_QUADRANT;
		if(current_halfcycle == FIRST_HALFCYCLE){
			current_halfcycle = SECOND_HALFCYCLE;
		}
		else{
			current_halfcycle = FIRST_HALFCYCLE;
		}
	}
	if(current_quadrant == FIRST_QUADRANT){
		current_one_quadrant_index++;
	}
	else{
		current_one_quadrant_index--;
	}
	if(current_halfcycle == SECOND_HALFCYCLE){
		duty = 1023 - duty;
	}

#if DEPTH_ON_OR_OFF == 1
	//Apply Depth
	if(current_depth == 255){
		duty = 1023 - duty;
	}
	else if(current_depth != 0){
		//duty = 1023 - duty*(current_depth >> 8);
		multiply_duty_by_current_depth_and_divide_by_256();
	}
	else{
		duty = 1023; //if depth is 0, just output 1023
	}
#endif

	//Write Duty
	__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, duty); //updates the CCR register of TIM14, which sets duty
}

uint8_t multiply_duty_by_current_depth_and_divide_by_256(void)
{
	volatile uint32_t multiply_product = 0;

	//Perform: (duty*current_depth) / 256
	multiply_product = duty * current_depth; //compiler should compile this as a hardware multiplication, but need to check
    duty = 1023 - (uint16_t)(multiply_product >> 8);

    return 1;
}

void TIM17_callback(TIM_HandleTypeDef *htim)
{
	//Start ADC (in scan mode) conversion
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions);

	//TMR1_Stop(); //-to be sorted

	//size_t tmr3_value = TMR3_OVERFLOW_COUNT; //-to be sorted

	//TMR3_Write(tmr3_value); //-to be sorted
	//TMR3_Start(); //time delay to allow ADCC conversion to complete, DMA is triggered on overflow. //-to be sorted
}
