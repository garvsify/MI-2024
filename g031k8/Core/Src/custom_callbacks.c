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

	if((state == STATE_0) || ((state != STATE_0) && (first_sync_complete == NO))){

		Process_TIM16_Raw_Start_Value_and_Raw_Prescaler(&params);
	}

	else if((state != STATE_0) && (first_sync_complete == YES)){

		params.raw_start_value = params_working.raw_start_value;
		params.raw_prescaler = params_working.raw_prescaler;
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

			if(interrupt_period < HIGHEST_PRESCALER_TOP_SPEED_PERIOD){ //if the captured value/512 is less than 129, then the desired speed is not reproducable, do not proceed witj ip capture

				/*interrupt_period = HIGHEST_PRESCALER_TOP_SPEED_PERIOD;

				//start TIM3 from 0 with CCR loaded with TIM2_ch1_input_capture_value
				//CCR shouldn't be preloaded so *should* update instantaneously
				__HAL_TIM_SET_COUNTER(&htim3, 0);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, HIGHEST_PRESCALER_TOP_SPEED_PERIOD);
				Start_OC_TIM(&htim3, TIM_CHANNEL_1);

				//set I/P capture measurement re-elapse 1 is ongoing flag
				input_capture_measurement_reelapse_is_ongoing = YES;*/

				input_capture_processing_can_be_started = NO;
			}

			//No need to check longest period as that is tested inherently by the TIM2 overflow

			else{

				//start TIM3 from 0 with CCR loaded with TIM2_ch1_input_capture_value
				//CCR shouldn't be preloaded so *should* update instantaneously
				__HAL_TIM_SET_COUNTER(&htim3, 0);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, interrupt_period); //measured value divided by 512
				Start_OC_TIM(&htim3, TIM_CHANNEL_1);

				//set I/P capture measurement re-elapse is ongoing flag
				input_capture_measurement_reelapse_is_ongoing = YES;

				//begin processing
				input_capture_processing_can_be_started = YES;
			}
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

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

	// @TODO //WRITE CODE TO LOAD CORRECT DUTY DELAYED VALUE TO SECONDARY OSCILLATOR
	Set_Oscillator_Values(&params_to_be_loaded);

	Stop_OC_TIM(&htim3, TIM_CHANNEL_1);

	input_capture_measurement_reelapse_is_ongoing = NO;

	Copy_Params_Structs(&params_to_be_loaded, &params_working);
	Copy_Params_Structs(&params_to_be_loaded, &params);
	first_sync_complete = YES;

	Calculate_Next_Main_Oscillator_Values(&params, (enum Next_Values_Processing_Mode)REGULAR_MODE);
	Write_Next_Main_Oscillator_Values_to_Delay_Line(&params, &delay_line);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions); //this function takes ages to execute!

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
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

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin){

	//DISABLE EXTI INTERRUPTS - in EXTI Callback before

	if((GPIO_Pin == CLK_IN_Pin)){ //if specifically CLK IN pin with falling interrupt

		flags = 1 << 15;

		HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1);

		flags = 1 << 16;

		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);

		flags = 1 << 17;

		HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

		flags = 1 << 19;
	}
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){

	//DISABLE EXTI INTERRUPTS - in EXTI Callback before

	if((GPIO_Pin == CLK_IN_Pin)){ //if specifically CLK IN pin with rising interrupt

		flags = 1 << 0;

		//Set SW OUT
		HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);

		flags = 1 << 1;

		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

		flags = 1 << 2;

		if(state == STATE_2){

			flags = 1 << 5;

			LPTIM2_overflow_count = 0;

			flags = 1 << 6;

			HAL_LPTIM_SetOnce_Stop_IT(&hlptim2);

			flags = 1 << 7;

			HAL_LPTIM_SetOnce_Start_IT(&hlptim2, LPTIM2_LENGTH, LPTIM2_LENGTH);

			flags = 1 << 8;

			IP_CAP_events_detection_timeout = NO;

			flags = 1 << 9;
		}
		else if(state == STATE_0){

			flags = 1 << 10;

			LPTIM2_overflow_count = 0;

			flags = 1 << 11;

			HAL_LPTIM_SetOnce_Start_IT(&hlptim2, LPTIM2_LENGTH, LPTIM2_LENGTH);

			flags = 1 << 12;

			state = STATE_2;

			flags = 1 << 13;

			IP_CAP_events_detection_timeout = NO;

			flags = 1 << 14;
		}

		HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

		flags = 1 << 18;
	}
}

void LPTIM1_callback(LPTIM_HandleTypeDef *hlptim){

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

	static volatile struct Tap_Tempo_Switch_States tap_tempo_switch_states = {0};
	//volatile enum CLK_IN_State clk_in_state = LOW;

	if((uint8_t)HAL_GPIO_ReadPin(SW_IN_GPIO_Port, SW_IN_Pin) == 0){

		state = STATE_1;
	}
	/*else if((uint8_t)HAL_GPIO_ReadPin(CLK_IN_GPIO_Port, CLK_IN_Pin) == 1){

		state = STATE_2;
	}*/

	//don't add conditional for STATE_0

	if(IP_CAP_events_detection_timeout == YES){

		Speed_Pot_Check(&params);
	}


	if(state == STATE_1){

		Check_Tap_Tempo_Switch_State(&tap_tempo_switch_states);

		if(tap_tempo_switch_states.tap_tempo_switch_state == DEPRESSED){

			//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
			HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

		}
		else if(tap_tempo_switch_states.tap_tempo_switch_state == NOT_DEPRESSED){

			//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);
			HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1); //reset
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
		}

	}

	//SET PREVIOUS STATE TO CURRENT STATE
	//tap_tempo_switch_states.tap_tempo_switch_prev_state = tap_tempo_switch_states.tap_tempo_switch_state;


	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);

	//SET TIMER TRIGGER
	HAL_LPTIM_SetOnce_Start_IT(&hlptim1, LPTIM1_CCR_CHECK, LPTIM1_CCR_CHECK);

}

void LPTIM2_callback(LPTIM_HandleTypeDef *hlptim){

	/*if(LPTIM2_overflow_count != LPTIM2_COUNT_MAX - 1){

		LPTIM2_overflow_count++;
		HAL_LPTIM_SetOnce_Start_IT(&hlptim2, LPTIM2_LENGTH, LPTIM2_LENGTH);
	}
	else{

		IP_CAP_events_detection_timeout = YES;
		HAL_LPTIM_SetOnce_Stop_IT(&hlptim2);
	}*/


	HAL_GPIO_TogglePin(MONITOR_GPIO_Port, MONITOR_Pin);

	HAL_LPTIM_SetOnce_Start_IT(&hlptim2, LPTIM2_LENGTH, LPTIM2_LENGTH);
}

void TIM17_overflow_callback(TIM_HandleTypeDef *htim){

	/*Stop_OC_TIM(&htim17, TIM_CHANNEL_1);

	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);*/

	Stop_OC_TIM(&htim17, TIM_CHANNEL_1);

	HAL_GPIO_TogglePin(MONITOR_GPIO_Port, MONITOR_Pin);

	__HAL_TIM_SET_COUNTER(&htim17, 0);

	Start_OC_TIM(&htim17, TIM_CHANNEL_1);

}
