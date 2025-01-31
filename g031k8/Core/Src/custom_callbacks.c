#include "custom_callbacks.h"

void TIM16_callback(TIM_HandleTypeDef *htim)
{
	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

	Set_Oscillator_Values(&params);
	Calculate_Next_Main_Oscillator_Values(&params, (enum Next_Values_Processing_Mode)REGULAR_MODE);
	Write_Next_Main_Oscillator_Values_to_Delay_Line(&params, &delay_line);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions); //this function takes ages to execute!

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
}

void ADC_DMA_conversion_complete_callback(ADC_HandleTypeDef *hadc)
{
	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

	HAL_ADC_Stop_DMA(hadc); //disable ADC DMA
	Process_ADC_Conversion_Values(&params, &delay_line, ADCResultsDMA);

	if((tap_tempo_mode_is_active == YES) || (external_clock_mode_is_active == YES)){

		//COPY THE 'to be loaded' raw values into params such that the speed pot raw values are ignored in favour of the I/P cap processing raw values
		params.raw_start_value = params_to_be_loaded.raw_start_value;
		params.raw_prescaler = params_to_be_loaded.raw_prescaler;
	}
	else{ //pot control mode

		Process_TIM16_Raw_Start_Value_and_Raw_Prescaler(&params);
	}

	Process_TIM16_Final_Start_Value_and_Final_Prescaler(&params);

	//after initial conversion is complete, set the conversion complete flag - leave this after raw/final value processing rather than actually when ADC values are converted for startup routine reasons.
	if(initial_ADC_conversion_complete == NO){
		initial_ADC_conversion_complete = YES;
	}

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
}

void TIM2_ch1_IP_capture_callback(TIM_HandleTypeDef *htim){

	TIM2_ch1_input_capture_value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

	interrupt_period = TIM2_ch1_input_capture_value >> 9; //divided by 512

	//since the input capture measurement is z, and this is 512x the interrupt period, we just use the interrupt
	//period = z/512 as the 'elapse period value' if we also set the elapse timer prescaler to 512x less than the
	//input capture measurement timer

	if(input_capture_event == FIRST){ //edge detected is the first

		__HAL_TIM_SET_COUNTER(&htim2, 0); //begin measurement
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
				params_to_be_loaded.raw_prescaler = 64;

				//start TIM3 from 0 with CCR loaded with TIM2_ch1_input_capture_value
				//CCR shouldn't be preloaded so *should* update instantaneously
				__HAL_TIM_SET_COUNTER(&htim3, 0);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, HIGHEST_PRESCALER_TOP_SPEED_PERIOD);
				Start_OC_TIM(&htim3, TIM_CHANNEL_1);

				//set I/P capture measurement re-elapse 1 is ongoing flag
				input_capture_measurement_reelapse_is_ongoing = YES;
			}
			else{

				//start TIM3 from 0 with CCR loaded with TIM2_ch1_input_capture_value
				//CCR shouldn't be preloaded so *should* update instantaneously
				__HAL_TIM_SET_COUNTER(&htim3, 0);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, interrupt_period); //measured value divided by 512
				Start_OC_TIM(&htim3, TIM_CHANNEL_1);

				//set I/P capture measurement re-elapse is ongoing flag
				input_capture_measurement_reelapse_is_ongoing = YES;
			}

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

	// @TODO //WRITE CODE TO LOAD CORRECT DUTY DELAYED VALUE TO SECONDARY OSCILLATOR
	Set_Oscillator_Values(&params_to_be_loaded);

	Stop_OC_TIM(&htim3, TIM_CHANNEL_1);
	input_capture_measurement_reelapse_is_ongoing = NO;

	params.index = params_to_be_loaded.index;

	Calculate_Next_Main_Oscillator_Values(&params, (enum Next_Values_Processing_Mode)REGULAR_MODE);
	Write_Next_Main_Oscillator_Values_to_Delay_Line(&params, &delay_line);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions); //this function takes ages to execute!

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin){

	//DISABLE EXTI INTERRUPTS - in EXTI Callback before

	if((GPIO_Pin == CLK_IN_Pin) && (HAL_GPIO_ReadPin(CLK_IN_GPIO_Port, CLK_IN_Pin) == 0)){ //if specifically CLK IN pin with falling interrupt

		//Set SW OUT
		HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);

		//SET MODES
		external_clock_mode_is_active = YES;
		tap_tempo_mode_is_active = NO;

		/*//DISABLE TAP-TEMPO SWITCH CHECKING
		HAL_NVIC_DisableIRQ(LPTIM1_IRQn);
		HAL_LPTIM_SetOnce_Stop_IT(&hlptim1);*/

		//START SPEED POT CHECKING
		__HAL_TIM_SET_COUNTER(&htim17, 0);
		Start_OC_TIM(&htim17, TIM_CHANNEL_1);

		HAL_NVIC_EnableIRQ(EXTI4_15_IRQn); //allow clk to cause interrupts
	}
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){

	//DISABLE EXTI INTERRUPTS - in EXTI Callback before

	if((GPIO_Pin == CLK_IN_Pin) && (HAL_GPIO_ReadPin(CLK_IN_GPIO_Port, CLK_IN_Pin) == 1) && (external_clock_mode_is_active == YES)){ //if specifically CLK IN pin with rising interrupt

		HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1);
	}

	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn); //allow clk to cause interrupts
}

void UART2_TX_transfer_complete_callback(UART_HandleTypeDef *huart){

	UART_DMA_TX_is_complete = YES;
}

void UART2_RX_transfer_complete_callback(UART_HandleTypeDef *huart){

	if(rx_buffer[0] == 'y'){

		params.final_prescaler = 64;
		params.final_start_value = 127;
		rx_buffer[0] = 0;
	}
	HAL_UART_Receive_DMA(&huart2, (uint8_t*)rx_buffer, sizeof(rx_buffer));
}

void LPTIM1_callback(LPTIM_HandleTypeDef *hlptim){

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

	static volatile struct Tap_Tempo_Switch_States tap_tempo_switch_states = {0};

	Check_Tap_Tempo_Switch_State(&tap_tempo_switch_states);

	if(tap_tempo_switch_states.tap_tempo_switch_state == DEPRESSED){

		tap_tempo_mode_is_active = YES;
		external_clock_mode_is_active = NO;

		//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
		HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

	}
	else if(tap_tempo_switch_states.tap_tempo_switch_state == NOT_DEPRESSED){

		//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);
		HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1); //reset
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);

		HAL_NVIC_EnableIRQ(EXTI4_15_IRQn); //allow switch to cause interrupts again
	}

	//SET PREVIOUS STATE TO CURRENT STATE
	//tap_tempo_switch_states.tap_tempo_switch_prev_state = tap_tempo_switch_states.tap_tempo_switch_state;


	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);

	//SET TIMER TRIGGER
	HAL_LPTIM_SetOnce_Start_IT(&hlptim1, LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK, LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK);

}

void TIM17_callback_speed_pot_check(TIM_HandleTypeDef *htim){

	Stop_OC_TIM(&htim17, TIM_CHANNEL_1);

	Speed_Pot_Check(&params);

	__HAL_TIM_SET_COUNTER(&htim17, 0);
	Start_OC_TIM(&htim17, TIM_CHANNEL_1);
}
