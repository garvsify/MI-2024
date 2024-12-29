#include "custom_callbacks.h"

void TIM16_callback(TIM_HandleTypeDef *htim)
{
	//TIM16 interrupt flag is already cleared by stm32g0xx_it.c

	TIM16_callback_active = YES;
	Global_Interrupt_Disable();
	HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 1);

	/////////////////////////////////////////////////////////////
	//FLASH LED in case of wave reaching apex or perceived apex//
	/////////////////////////////////////////////////////////////
	/*if((current_waveshape == SINE_MODE || current_waveshape == TRIANGLE_MODE) && current_index >= SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX && current_index < SINE_OR_TRIANGLE_WAVE_TEMPO_PULSE_OFF_INDEX){
		HAL_GPIO_WritePin(TEMPO_GPIO_Port, TEMPO_Pin, 1);
	}
	else if(current_waveshape == SQUARE_MODE && current_index >= SQUARE_WAVE_TEMPO_APEX_INDEX && current_index < SQUARE_WAVE_TEMPO_PULSE_OFF_INDEX){
		HAL_GPIO_WritePin(TEMPO_GPIO_Port, TEMPO_Pin, 1);
	}
	else{
		HAL_GPIO_WritePin(TEMPO_GPIO_Port, TEMPO_Pin, 0);
	}*/

	////////////////////////////////
	//SET THE CURRENT(prev) VALUES//
	////////////////////////////////
	TIM16->EGR |= TIM_EGR_UG; //DO NOT DELETE THIS LINE, IT LITERALLY MAKES OR BREAKS THE BASTARD - It triggers an 'update' event
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
	else if((current_waveshape == SQUARE_MODE) && (current_index < THIRD_QUADRANT_START_INDEX)){
		duty = 1023;
	}
	else if((current_waveshape == SQUARE_MODE) && (current_index >= THIRD_QUADRANT_START_INDEX)){
			duty = 0;
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
	HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 0);
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

void ADC_DMA_conversion_complete_callback(ADC_HandleTypeDef *hadc)
{
	HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 1);
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
	current_speed = ADCResultsDMA[1] >> 2; //convert to 10-bit

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

	if(speed_pot_is_disabled == NO){

		Process_TIM16_Raw_Start_Value_and_Raw_Prescaler();
	}

	Process_TIM16_Final_Start_Value_and_Prescaler_Adjust();

	TIM16_final_start_value_locked = TIM16_final_start_value;
	TIM16_prescaler_divisors_final_index_locked = TIM16_prescaler_divisors_final_index;

	HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 0);
}

void TIM2_ch1_IP_capture_callback(TIM_HandleTypeDef *htim){

	TIM2_ch1_input_capture_value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

	interrupt_period = TIM2_ch1_input_capture_value >> 9; //divided by 512

	//since the input capture measurement is z, and this is 512x the interrupt period, we just use the interrupt
	//period = z/512 as the 'elapse period value' if we also set the elapse timer prescaler to 512 less than the
	//input capture measurement timer

	if(input_capture_event == FIRST){ //edge detected is the first

		speed_pot_is_disabled = YES;
		input_capture_measurement_is_ongoing = YES;
		__HAL_TIM_SET_COUNTER(&htim2, 0); //begin measurement

		input_capture_event = SECOND;
	}
	else{ //is second

		input_capture_event = FIRST; //reset event name
		input_capture_measurement_is_ongoing = NO;

		if(input_capture_measurement_reelapse_1_is_ongoing == YES){

			//second edge was received when the measurement reelapse 1 was ongoing
			//This should restart the measurement reelapse (discarding the previous measurement)
			Stop_OC_TIM(&htim3, TIM_CHANNEL_1);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, interrupt_period);
			__HAL_TIM_SET_COUNTER(&htim3, 0);
			Start_OC_TIM(&htim3, TIM_CHANNEL_1);
		}

		if(interrupt_period < HIGHEST_PRESCALER_TOP_SPEED_PERIOD){ //if the captured value/512 is less than 129, then the desired speed is not reproducable, so just set the absolute top speed (i.e. highest prescaler and shortest period)

			interrupt_period = HIGHEST_PRESCALER_TOP_SPEED_PERIOD;

			//start TIM3 from 0 with CCR loaded with TIM2_ch1_input_capture_value
			//CCR shouldn't be preloaded so *should* update instantaneously
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, HIGHEST_PRESCALER_TOP_SPEED_PERIOD);
			__HAL_TIM_SET_COUNTER(&htim3, 0);
			Start_OC_TIM(&htim3, TIM_CHANNEL_1);

			//set I/P capture measurement re-elapse 1 is ongoing flag
			input_capture_measurement_reelapse_1_is_ongoing = YES;
		}
		else{

			//start TIM3 from 0 with CCR loaded with TIM2_ch1_input_capture_value
			//CCR shouldn't be preloaded so *should* update instantaneously
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, interrupt_period); //measured value divided by 512
			__HAL_TIM_SET_COUNTER(&htim3, 0);
			Start_OC_TIM(&htim3, TIM_CHANNEL_1);

			//set I/P capture measurement re-elapse 1 is ongoing flag
			input_capture_measurement_reelapse_1_is_ongoing = YES;
		}

		//DETERMINE WHAT TO SET THE RAW_START_VALUE AND BASE_PRESCALER TO BASED ON THE I/P CAPTURE VALUE
		if(interrupt_period <= SIXTY_FOUR_PRESCALER_LARGEST_INTERRUPT_PERIOD){

			TIM16_raw_start_value = 256 - interrupt_period;
			TIM16_base_prescaler_divisors_index = FASTEST_SPEED_PRESCALER_DIVISORS_ARRAY_INDEX;
		}
		else if(interrupt_period <= ONE_HUNDRED_TWENTY_EIGHT_PRESCALER_LARGEST_INTERRUPT_PERIOD){

			interrupt_period = interrupt_period >> 1;
			TIM16_raw_start_value = 256 - interrupt_period;
			TIM16_base_prescaler_divisors_index = FASTEST_SPEED_PRESCALER_DIVISORS_ARRAY_INDEX - 1;
		}
		else if(interrupt_period <= TWO_HUNDRED_FIFTY_SIX_PRESCALER_LARGEST_INTERRUPT_PERIOD){

			interrupt_period = interrupt_period >> 2;
			TIM16_raw_start_value = 256 - interrupt_period;
			TIM16_base_prescaler_divisors_index = FASTEST_SPEED_PRESCALER_DIVISORS_ARRAY_INDEX - 2;
		}
		else if(interrupt_period <= FIVE_HUNDRED_TWELVE_PRESCALER_LARGEST_INTERRUPT_PERIOD){

			interrupt_period = interrupt_period >> 3;
			TIM16_raw_start_value = 256 - interrupt_period;
			TIM16_base_prescaler_divisors_index = FASTEST_SPEED_PRESCALER_DIVISORS_ARRAY_INDEX - 3;
		}
		else if(interrupt_period <= ONE_THOUSAND_TWENTY_FOUR_PRESCALER_LARGEST_INTERRUPT_PERIOD){

			interrupt_period = interrupt_period >> 4;
			TIM16_raw_start_value = 256 - interrupt_period;
			TIM16_base_prescaler_divisors_index = FASTEST_SPEED_PRESCALER_DIVISORS_ARRAY_INDEX - 4;
		}

		//WHEN THE MEASUREMENT REELAPSE TIMER INTERRUPTS, WE WANT TO 'RESTART' THE WAVE TO A SPECIFIC INDEX,
		//AS SUCH WE HAVE TO WORK OUT THE INDEX WE WANT TO RESTART THE WAVE AT, BASED ON THE TYPE OF WAVE SELECTED,
		//(WE'LL HAVE TO SYNCHRONISE THE TIM16 INTERRUPTS UPON THIS REELAPSE INTERRUPT, AND ALSO
		//BECAUSE OF HOW THE TIM16 CALLBACK WORKS, WE'LL HAVE TO WORK OUT HERE WHAT THE DUTY, FINAL_START_VALUE, AND PRESCALER_ADJUST ARE

		if(current_waveshape == SINE_MODE || current_waveshape == TRIANGLE_MODE){

			current_index = SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX;
			current_quadrant = CURRENT_QUADRANT_SINE_OR_TRI_SYNCED;
			current_halfcycle = CURRENT_HALFCYCLE_SINE_OR_TRI_SYNCED;
		}
		else{
			current_index = SQUARE_WAVE_TEMPO_APEX_INDEX;
			current_quadrant = CURRENT_QUADRANT_SQUARE_SYNCED;
			current_halfcycle = CURRENT_HALFCYCLE_SQUARE_SYNCED;
		}

		//ONCE INDEX IS SET, FIND THE DUTY VALUE
		if(current_waveshape == TRIANGLE_MODE){
			duty = tri_wavetable[current_index];
		}
		else if(current_waveshape == SINE_MODE){
			duty = sine_wavetable[current_index];
		}
		else if((current_waveshape == SQUARE_MODE) && (current_index < THIRD_QUADRANT_START_INDEX)){
			duty = 1023;
		}
		else if((current_waveshape == SQUARE_MODE) && (current_index >= THIRD_QUADRANT_START_INDEX)){
			duty = 0;
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
	}
}

void TIM2_ch1_overflow_callback(TIM_HandleTypeDef *htim){

	if(input_capture_measurement_is_ongoing == YES && input_capture_event == SECOND){

		/*overflow has occurred at a time when an input capture measurement is occurring, which means input capture event would have been the second if it it did occur.
		This means that the user has depressed the switch once, and hasn't pressed again within the timeout period.
		Thus, reset everything requiring the user to press the switch again to start another capture*/

		input_capture_measurement_is_ongoing = NO;
		input_capture_event = FIRST;

		//write code to set the lowest speed!
	}
}

void TIM3_ch1_IP_capture_measurement_reelapse_1_callback(TIM_HandleTypeDef *htim){

	input_capture_measurement_reelapse_1_is_ongoing = NO;

}

/*void TIM17_ch1_IP_capture_measurement_reelapse_2_callback(TIM_HandleTypeDef *htim){

	//I don't think this is actually needed but leaving it defined just in case

}*/
