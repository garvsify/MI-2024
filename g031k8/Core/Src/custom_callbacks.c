#include "custom_callbacks.h"

void TIM16_callback(TIM_HandleTypeDef *htim)
{
	//TIM16 interrupt flag is already cleared by stm32g0xx_it.c

	Global_Interrupt_Disable();
	TIM16_callback_active = YES;
	isr_done = NO;
	adc_done = NO;

	HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 1);

	//////////////////////////
	//SET THE CURRENT(prev) VALUES//
	//////////////////////////
	__HAL_TIM_SET_COUNTER(&htim16, TIM16_final_start_value_locked); //this line must go here, or at least very near the beginning!
	__HAL_TIM_SET_PRESCALER(&htim16, (TIM16_prescaler_divisors[TIM16_prescaler_divisors_final_index_locked]) - 1); //have to take one off the divisor
	__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, prev_duty); //updates the CCR register of TIM14, which sets duty, i.e. the ON time relative to the total period which is set by the ARR.

	/////////////////////////////
	//CALCULATE THE NEXT VALUES//
	/////////////////////////////
	current_index++;

	if(current_index == FINAL_INDEX + 1){
		current_quadrant = FIRST_QUADRANT;
		current_halfcycle = FIRST_HALFCYCLE;
		current_index = 0;
	}

	if(current_waveshape == TRIANGLE_MODE){
		duty = tri_wavetable[current_index];
	}
	else if(current_waveshape == SINE_MODE){
		duty = sine_wavetable[current_index];
	}
	else if((current_waveshape == SQUARE_MODE) && (current_index < SECOND_QUADRANT_START_INDEX)){
		duty = 1023;
	}
	else if((current_waveshape == SQUARE_MODE) && (current_index >= SECOND_QUADRANT_START_INDEX) && (current_index < FOURTH_QUADRANT_START_INDEX)){
			duty = 0;
	}
	else if((current_waveshape == SQUARE_MODE) && (current_index >= FOURTH_QUADRANT_START_INDEX)){
		duty = 1023;
	}

	if(current_index == FIRST_QUADRANT_START_INDEX){
		current_quadrant = FIRST_QUADRANT;
		current_halfcycle = FIRST_HALFCYCLE;
	}
	else if(current_index == SECOND_QUADRANT_START_INDEX){
		current_quadrant = SECOND_QUADRANT;
		current_halfcycle = FIRST_HALFCYCLE;
	}
	else if(current_index == THIRD_QUADRANT_START_INDEX){
		current_quadrant = FIRST_QUADRANT;
		current_halfcycle = SECOND_HALFCYCLE;
	}
	else if(current_index == FOURTH_QUADRANT_START_INDEX){
		current_quadrant = SECOND_QUADRANT;
		current_halfcycle = SECOND_HALFCYCLE;
	}

	//APPLY DEPTH
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

	prev_duty = duty;

	HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 0);

	TIM16_callback_active = NO;

	HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions); //this function takes ages to execute!

	isr_done = YES;

	Global_Interrupt_Enable();
}

uint8_t Multiply_Duty_By_Current_Depth_and_Divide_By_256(void)
{
	volatile uint32_t multiply_product = 0;

	//Perform: (duty*current_depth) / 256
	multiply_product = duty * current_depth; //compiler should compile this as a hardware multiplication, but need to check
    duty = 1023 - (uint16_t)(multiply_product >> 8);

    return 1;
}

void TIM17_callback(TIM_HandleTypeDef *htim) //Not used anymore
{
	//Start ADC (in scan mode) conversion
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions);

	Stop_OC_TIM(htim, TIM_CHANNEL_1); //disable TIM17
}

void ADC_DMA_conversion_complete_callback(ADC_HandleTypeDef *hadc)
{
	Global_Interrupt_Disable(); //DO NOT DELETE

	//HAL_ADC_Stop_DMA(hadc); //disable ADC DMA

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

		#if SYMMETRY_ADC_RESOLUTION == 12
			current_symmetry = ADCResultsDMA[3];

		#endif

	#endif

	//after initial conversion is complete, set the conversion complete flag
	if(initial_ADC_conversion_complete == NO){
		initial_ADC_conversion_complete = YES;
	}

	HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 0);

	HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 1);

	Process_TIM16_Raw_Start_Value_and_Raw_Prescaler();
	Process_TIM16_Final_Start_Value_and_Prescaler_Adjust();

	TIM16_final_start_value_locked = TIM16_final_start_value;
	TIM16_prescaler_divisors_final_index_locked = TIM16_prescaler_divisors_final_index;

	HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 0);

	adc_done = YES;

	Global_Interrupt_Enable(); //DO NOT DELETE
}
