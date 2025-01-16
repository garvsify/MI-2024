#include "custom_callbacks.h"

void TIM16_callback(TIM_HandleTypeDef *htim)
{
	//TIM16 interrupt flag is already cleared by stm32g0xx_it.c

	TIM16_callback_active = YES;

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

	////////////////////////////////////////////////////////
	//SET THE CURRENT(prev) VALUES FOR THE MAIN OSCILLATOR//
	////////////////////////////////////////////////////////
	TIM16->EGR |= TIM_EGR_UG; //DO NOT DELETE THIS LINE, IT LITERALLY MAKES OR BREAKS THE BASTARD - It triggers an 'update' event
	__HAL_TIM_SET_COUNTER(&htim16, TIM16_final_start_value); //this line must go here, or at least very near the beginning!
	__HAL_TIM_SET_PRESCALER(&htim16, (TIM16_final_prescaler - 1)); //have to take one off the divisor
	TIM1->EGR |= TIM_EGR_UG; //not sure if we really need this line but gonna keep it here because it worked wonders for TIM16
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, prev_duty); //updates the CCR register of TIM14, which sets duty, i.e. the ON time relative to the total period which is set by the ARR.

	/////////////////////////////////////////////////////////////
	//SET THE CURRENT(prev) VALUES FOR THE SECONDARY OSCILLATOR//
	/////////////////////////////////////////////////////////////
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, duty_delayed); //updates the CCR register of TIM14, which sets duty, i.e. the ON time relative to the total period which is set by the ARR.

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

	//SET THE NEXT VALUE FOR THE MAIN OSCILLATOR
	prev_duty = duty;

	//STORE THE VALUES IN THE APPROPRIATE '0TH - 1' INDEX RELATIVE TO THE START POINTER
	if(duty_delay_line_start_offset != 0){
		duty_delay_line_storage_array[duty_delay_line_start_offset - 1] = duty;
	}
	else{
		duty_delay_line_storage_array[FINAL_INDEX + 1] = duty;
	}

	//DECREMENT THE START AND FINISH POINTERS
	if(duty_delay_line_start_offset == 0){
		duty_delay_line_start_offset = FINAL_INDEX + 1;
		duty_delay_line_finish_offset = duty_delay_line_finish_offset - 1;
	}
	else if(duty_delay_line_finish_offset == 0){
		duty_delay_line_finish_offset = FINAL_INDEX + 1;
		duty_delay_line_start_offset = duty_delay_line_start_offset - 1;
	}
	else{
		duty_delay_line_start_offset = duty_delay_line_start_offset - 1;
		duty_delay_line_finish_offset = duty_delay_line_finish_offset - 1;
	}

	//DETERMINE THE DELAYED WAVE'S VALUES
	if(duty_delay_line_start_offset + duty_delay_line_read_pointer_offset > FINAL_INDEX + 1){ //if the desired starting index falls off the end of the array
		duty_delayed = *(duty_delay_line_storage_array + (duty_delay_line_start_offset + duty_delay_line_read_pointer_offset - (FINAL_INDEX + 1)));
	}
	else{
		duty_delayed = *(duty_delay_line_storage_array + duty_delay_line_start_offset + duty_delay_line_read_pointer_offset);
	}

	TIM16_callback_active = NO;

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions); //this function takes ages to execute!
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
		//current_depth = ADCResultsDMA[2] >> 4; //convert to 8-bit
		duty_delay_line_read_pointer_offset = ADCResultsDMA[2] >> 3;

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

	if(speed_pot_is_disabled == NO){

		Process_TIM16_Raw_Start_Value_and_Raw_Prescaler(current_speed, SPEED_ADC_RESOLUTION, NUMBER_OF_FREQUENCY_STEPS, &TIM16_raw_start_value, &TIM16_raw_prescaler);
		Process_TIM16_Final_Start_Value_and_Final_Prescaler(TIM16_raw_start_value, &TIM16_final_start_value, TIM16_raw_prescaler, &TIM16_final_prescaler, current_symmetry, current_waveshape, current_halfcycle, current_quadrant, current_index);
	}
}

void TIM2_ch1_IP_capture_callback(TIM_HandleTypeDef *htim){

	TIM2_ch1_input_capture_value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

	interrupt_period = TIM2_ch1_input_capture_value >> 9; //divided by 512

	//since the input capture measurement is z, and this is 512x the interrupt period, we just use the interrupt
	//period = z/512 as the 'elapse period value' if we also set the elapse timer prescaler to 512x less than the
	//input capture measurement timer

	if(input_capture_event == FIRST){ //edge detected is the first

		__HAL_TIM_SET_COUNTER(&htim2, 0); //begin measurement
		speed_pot_is_disabled = YES;
		tap_tempo_mode_is_active = YES;
		input_capture_measurement_is_ongoing = YES;

		input_capture_event = SECOND;
	}
	else{ //is second

		input_capture_event = FIRST; //reset event name
		input_capture_measurement_is_ongoing = NO;

		if(input_capture_measurement_reelapse_is_ongoing == YES){

			//second edge was received when the measurement reelapse was ongoing
			//This should restart the measurement reelapse (discarding the previous measurement)
			__HAL_TIM_SET_COUNTER(&htim3, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, interrupt_period);
			Start_OC_TIM(&htim3, TIM_CHANNEL_1);
		}
		else{

			if(interrupt_period < HIGHEST_PRESCALER_TOP_SPEED_PERIOD){ //if the captured value/512 is less than 129, then the desired speed is not reproducable, so just set the absolute top speed (i.e. highest prescaler and shortest period)

				interrupt_period = HIGHEST_PRESCALER_TOP_SPEED_PERIOD;
				TIM16_raw_prescaler_to_be_loaded = 64;

				//start TIM3 from 0 with CCR loaded with TIM2_ch1_input_capture_value
				//CCR shouldn't be preloaded so *should* update instantaneously
				__HAL_TIM_SET_COUNTER(&htim3, 0);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, HIGHEST_PRESCALER_TOP_SPEED_PERIOD);
				Start_OC_TIM(&htim3, TIM_CHANNEL_1);

				//set I/P capture measurement re-elapse 1 is ongoing flag
				input_capture_measurement_reelapse_is_ongoing = YES;
			}
			else{

				HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

				//start TIM3 from 0 with CCR loaded with TIM2_ch1_input_capture_value
				//CCR shouldn't be preloaded so *should* update instantaneously
				__HAL_TIM_SET_COUNTER(&htim3, 0);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, interrupt_period); //measured value divided by 512
				Start_OC_TIM(&htim3, TIM_CHANNEL_1);

				//set I/P capture measurement re-elapse is ongoing flag
				input_capture_measurement_reelapse_is_ongoing = YES;
			}

			HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);

			input_capture_processing_can_be_started = YES;
		}
	}
}

void TIM2_ch1_overflow_callback(TIM_HandleTypeDef *htim){

	if(input_capture_measurement_is_ongoing == YES && input_capture_event == SECOND){

		//overflow has occurred at a time when an input capture measurement is occurring, which means input capture event would have been the second if it it did occur.
		//This means that the user has depressed the switch once, and hasn't pressed again within the timeout period.
		//Thus, reset everything requiring the user to press the switch again to start another capture

		input_capture_measurement_is_ongoing = NO;
		input_capture_event = FIRST;
	}
}

void TIM3_ch1_IP_capture_measurement_reelapse_callback(TIM_HandleTypeDef *htim){

	//force update of timers to sync the wave to the TIM3 reelapse interrupt

	HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

	TIM16->EGR |= TIM_EGR_UG; //DO NOT DELETE THIS LINE, IT LITERALLY MAKES OR BREAKS THE BASTARD - It triggers an 'update' event
	__HAL_TIM_SET_COUNTER(&htim16, TIM16_final_start_value_to_be_loaded); //this line must go here, or at least very near the beginning!
	__HAL_TIM_SET_PRESCALER(&htim16, TIM16_final_prescaler_to_be_loaded - 1); //have to take one off the divisor
	TIM1->EGR |= TIM_EGR_UG; //not sure if we really need this line but gonna keep it here because it worked wonders for TIM16
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, duty_to_be_loaded); //updates the CCR register of TIM14, which sets duty, i.e. the ON time relative to the total period which is set by the ARR.

	//ADD CODE TO WORK OUT THE SECONDARY OSCILLATOR DELAYED VALUE SO BOTH CAN UPDATE SIMULTANEOUSLY, IT WILL BE MUCH BETTER THAT WAY

	Stop_OC_TIM(&htim3, TIM_CHANNEL_1);

	input_capture_measurement_reelapse_is_ongoing = NO;

	current_index = current_index_to_be_loaded;
	current_halfcycle = current_halfcycle_to_be_loaded;
	current_quadrant = current_quadrant_to_be_loaded;

	//set the values to the 'to be loaded' values such that the TIM16 continually loads these if the speed pot is disabled
	TIM16_final_start_value = TIM16_final_start_value_to_be_loaded;
	TIM16_final_prescaler = TIM16_final_prescaler_to_be_loaded;

	HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
}

/*void TIM17_callback_debounce(TIM_HandleTypeDef *htim){

	Stop_OC_TIM(&htim17, TIM_CHANNEL_1);

	if(tap_tempo_switch_state == NOT_DEPRESSED){ //switch released before timer elapsed

		HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1); //reset
	}

	TIM17_debounce_is_elapsing = NO;
	TAP_TEMPO_EXTI4_15_IRQ_is_disabled = NO;
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn); //enable EXTI10 interrupts

	DeInit- alternative to the above
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = SW_IN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(SW_IN_GPIO_Port, &GPIO_InitStruct);
}*/

/*void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin){

	if(tap_tempo_switch_state == NOT_DEPRESSED){ //previous state is not depressed - will eliminate switch release bouncing causing falling edges

		HAL_NVIC_DisableIRQ(EXTI4_15_IRQn); //disable further EXTI10 interrupts
		HAL_GPIO_DeInit(SW_IN_GPIO_Port, SW_IN_Pin); //apparently disables EXTI interrupt - alternative to the above

		HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0); //latch low until timer elapses

		tap_tempo_mode_is_active = YES;
		TAP_TEMPO_EXTI4_15_IRQ_is_disabled = YES;
		TIM17_debounce_is_elapsing = YES;

		__HAL_TIM_SET_COUNTER(&htim17, 0);
		Start_OC_TIM(&htim17, TIM_CHANNEL_1);
	}
}*/

/*void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){

	if(TIM17_debounce_is_elapsing == NO){ //check if this rising edge has occurred during debounce

		HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1); //reset
	}
}*/

void UART2_TX_transfer_complete_callback(UART_HandleTypeDef *huart){

	UART_DMA_TX_is_complete = YES;
}

void UART2_RX_transfer_complete_callback(UART_HandleTypeDef *huart){

	if(rx_buffer[0] == 'y'){

		speed_pot_is_disabled = YES;
		TIM16_final_prescaler = 64;
		TIM16_final_start_value = 127;
		rx_buffer[0] = 0;
	}
	HAL_UART_Receive_DMA(&huart2, (uint8_t*)rx_buffer, sizeof(rx_buffer));
}

void LPTIM1_callback(LPTIM_HandleTypeDef *hlptim){

	Check_Tap_Tempo_Switch_State(&tap_tempo_switch_state);

	if(tap_tempo_switch_state == DEPRESSED){

		//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
		HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
		tap_tempo_mode_is_active = YES;

	}
	else{
		//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);
		HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1); //reset
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
	}

	HAL_LPTIM_SetOnce_Start_IT(&hlptim1, LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK, LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK);
}

void TIM17_callback_speed_pot_check(TIM_HandleTypeDef *htim){

	Stop_OC_TIM(&htim17, TIM_CHANNEL_1);

	Speed_Pot_Check();

	__HAL_TIM_SET_COUNTER(&htim17, 0);
	Start_OC_TIM(&htim17, TIM_CHANNEL_1);
}
