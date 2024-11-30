#include "custom_callbacks.h"

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
		if((current_halfcycle == FIRST_HALFCYCLE) && (current_quadrant == FIRST_QUADRANT)){
			//do nothing
		}
		else if((current_halfcycle == FIRST_HALFCYCLE) && (current_quadrant == SECOND_QUADRANT)){
			current_halfcycle = SECOND_HALFCYCLE;
			current_quadrant = FIRST_QUADRANT;
		}
		else if((current_halfcycle == SECOND_HALFCYCLE) && (current_quadrant == FIRST_QUADRANT)){
			//do nothing
		}
		else if((current_halfcycle == SECOND_HALFCYCLE) && (current_quadrant == SECOND_QUADRANT)){
			current_halfcycle = FIRST_HALFCYCLE;
			current_quadrant = FIRST_QUADRANT;
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
		Multiply_Duty_By_Current_Depth_and_Divide_By_256();
	}
	else{
		duty = 1023; //if depth is 0, just output 1023
	}

#endif

	//Write Duty
	__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, duty); //updates the CCR register of TIM14, which sets duty, i.e. the ON time relative to the total period which is set by the ARR.
}

uint8_t Multiply_Duty_By_Current_Depth_and_Divide_By_256(void)
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

	Stop_OC_TIM(htim, TIM_CHANNEL_1); //disable TIM17
}

void ADC_DMA_conversion_complete_callback(ADC_HandleTypeDef *hadc)
{
	HAL_ADC_Stop_DMA(hadc); //disable ADC DMA

	//GET WAVESHAPE
	uint16_t ADC_result = ADCResultsDMA[0]; //set ADC_Result to waveshape index value

	if(ADC_result <= TRIANGLE_MODE_ADC_THRESHOLD){
		current_waveshape = TRIANGLE_MODE; //triangle wave
	}
	else if (ADC_result <= SINE_MODE_ADC_THRESHOLD){
		current_waveshape = SINE_MODE; //sine wave
	}
	else if (ADC_result <= SQUARE_MODE_ADC_THRESHOLD){
		current_waveshape = SQUARE_MODE; //square wave
	}
	else{
		current_waveshape = SINE_MODE; //if error, return sine
	}

	//GET SPEED
	current_speed_linear = ADCResultsDMA[1] >> 2; //convert to 10-bit

	//GET DEPTH
	#if DEPTH_ON_OR_OFF == ON

		current_depth = ADCResultsDMA[2] >> 4; //convert to 8-bit

	#endif

	//GET SYMMETRY
	#if SYMMETRY_ON_OR_OFF == ON

		#if SYMMETRY_ADC_RESOLUTION == 10

			current_symmetry = ADCResultsDMA[3] >> 2;

		#endif

		#if SYMMETRY_ADC_RESOLUTION == 8

			current_symmetry = ADCResultsDMA[3] >> 4;

		#endif

	#endif

	//after initial conversion is complete, set the conversion complete flag
	if(initial_ADC_conversion_complete == 0){

		initial_ADC_conversion_complete = 1;
	}

	__HAL_TIM_SET_COUNTER(&htim17, 0); //set counter to 0
	Start_OC_TIM(&htim17, TIM_CHANNEL_1);
}
