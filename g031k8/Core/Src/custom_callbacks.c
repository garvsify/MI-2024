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

	enum Validate first_sync_complete = Get_Status_Bit(&statuses, First_Sync_Complete);

	if(first_sync_complete == YES){

		params.raw_start_value = params_working.raw_start_value;
		params.raw_prescaler = params_working.raw_prescaler;
	}
	else{

		Process_TIM16_Raw_Start_Value_and_Raw_Prescaler(&params);
	}

	Process_TIM16_Final_Start_Value_and_Final_Prescaler(&params);

	//after initial conversion is complete, set the conversion complete flag - leave this after raw/final value processing rather than actually when ADC values are converted for startup routine reasons.
	if(Get_Status_Bit(&statuses, Initial_ADC_Conversion_Complete) == NO){
		Set_Status_Bit(&statuses, Initial_ADC_Conversion_Complete);
	}

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
}

void TIM2_ch1_IP_capture_callback(TIM_HandleTypeDef *htim){

	TIM2_ch1_input_capture_value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

	interrupt_period = TIM2_ch1_input_capture_value >> 9; //divided by 512

	//since the input capture measurement is z, and this is 512x the interrupt period, we just use the interrupt
	//period = z/512 as the 'elapse period value' if we also set the elapse timer prescaler to 512x less than the
	//input capture measurement timer

	if(IP_CAP_fsm.current_state == IDLE){

		Begin_Input_Capture_Measurement();
		IP_CAP_fsm.current_state = MEASUREMENT_PENDING;
		IP_CAP_fsm.prev_state = IDLE;
	}

	else if(IP_CAP_fsm.current_state == MEASUREMENT_PENDING){ //second edge

		if(interrupt_period >= HIGHEST_PRESCALER_TOP_SPEED_PERIOD){ //if the captured value/512 is >= than 129

			//No need to check longest period as that is tested inherently by the TIM2 overflow

			if(!((speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_PENDING_B1_MODE)
					|| (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_RESYNC_B1_MODE))){

				Start_Measurement_Reelapse_Timer();
			}

			IP_CAP_fsm.current_state = MEASUREMENT_REELAPSE;
			IP_CAP_fsm.prev_state = MEASUREMENT_PENDING;

			Copy_Params_Structs(&params, &params_to_be_loaded);

			Advance_Pending_States();

			//BEGIN PROCESSING
			Set_Status_Bit(&statuses, Input_Capture_Processing_Can_Be_Started);
		}
		//DO NOT EVER COMMENT THIS BACK IN - causes a bug at 15Hz sig. gen
		/*else{

			IP_CAP_fsm.current_state = IDLE;
			IP_CAP_fsm.prev_state = MEASUREMENT_PENDING;
		}*/
	}

	else if(IP_CAP_fsm.current_state == MEASUREMENT_REELAPSE){ //first edge

		Begin_Input_Capture_Measurement();

		IP_CAP_fsm.current_state = MEASUREMENT_REELAPSE_AND_MEASUREMENT_PENDING;
		IP_CAP_fsm.prev_state = MEASUREMENT_REELAPSE;
	}

	else if(IP_CAP_fsm.current_state == MEASUREMENT_REELAPSE_AND_MEASUREMENT_PENDING){ //second edge

		if(!((speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_PENDING_B1_MODE)
			|| (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_RESYNC_B1_MODE))){

			Start_Measurement_Reelapse_Timer();
		}

		IP_CAP_fsm.current_state = MEASUREMENT_REELAPSE;
		IP_CAP_fsm.prev_state = MEASUREMENT_REELAPSE_AND_MEASUREMENT_PENDING;

		Copy_Params_Structs(&params, &params_to_be_loaded);

		Advance_Pending_States();

		//BEGIN PROCESSING
		Set_Status_Bit(&statuses, Input_Capture_Processing_Can_Be_Started);
	}

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
}


void TIM2_ch1_overflow_callback(TIM_HandleTypeDef *htim){

	union Speed_FSM_States previous = speed_fsm.prev_state;

	if(IP_CAP_fsm.current_state == MEASUREMENT_PENDING){

		IP_CAP_fsm.current_state = IDLE;
		IP_CAP_fsm.prev_state = MEASUREMENT_PENDING;

		MIDI_CLK_fsm = NOT_COMPILING;
		MIDI_CLK_tag = 0;

		HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1);
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);

		speed_fsm.prev_state = speed_fsm.current_state;
		speed_fsm.current_state = previous;
	}
	else if(IP_CAP_fsm.current_state == MEASUREMENT_REELAPSE_AND_MEASUREMENT_PENDING){

		IP_CAP_fsm.current_state = MEASUREMENT_REELAPSE;
		IP_CAP_fsm.prev_state = MEASUREMENT_REELAPSE_AND_MEASUREMENT_PENDING;

		MIDI_CLK_fsm = NOT_COMPILING;
		MIDI_CLK_tag = 0;

		HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1);
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);

		speed_fsm.prev_state = speed_fsm.current_state;
		speed_fsm.current_state = previous;
	}
}

void TIM3_ch1_IP_capture_measurement_reelapse_callback(TIM_HandleTypeDef *htim){

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

	// @TODO //WRITE CODE TO LOAD CORRECT DUTY DELAYED VALUE TO SECONDARY OSCILLATOR
	Set_Oscillator_Values(&params_to_be_loaded);

	Stop_OC_TIM(&htim3, TIM_CHANNEL_1);

	if(IP_CAP_fsm.current_state == MEASUREMENT_REELAPSE){

		//UPDATE IP CAP FSM
		IP_CAP_fsm.current_state = IDLE;
		IP_CAP_fsm.prev_state = MEASUREMENT_REELAPSE;
	}
	else if(IP_CAP_fsm.current_state == MEASUREMENT_REELAPSE_AND_MEASUREMENT_PENDING){

		//UPDATE IP CAP FSM
		IP_CAP_fsm.current_state = MEASUREMENT_PENDING;
		IP_CAP_fsm.prev_state = MEASUREMENT_REELAPSE_AND_MEASUREMENT_PENDING;
	}

	Copy_Params_Structs(&params_to_be_loaded, &params_working);
	Copy_Params_Structs(&params_to_be_loaded, &params);

	Set_Status_Bit(&statuses, First_Sync_Complete);

	Calculate_Next_Main_Oscillator_Values(&params, (enum Next_Values_Processing_Mode)REGULAR_MODE);
	Write_Next_Main_Oscillator_Values_to_Delay_Line(&params, &delay_line);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions); //this function takes ages to execute!

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
}

void UART2_TX_transfer_complete_callback(UART_HandleTypeDef *huart){

	//UART_DMA_TX_is_complete = YES;
}

void UART2_RX_transfer_complete_callback(UART_HandleTypeDef *huart){

	if(Get_Status_Bit(&statuses, Start_Required_Before_Sync_Mode) == YES){

		if((MIDI_CLK_fsm == NOT_COMPILING) && (IP_CAP_fsm.current_state == IDLE)
				&& ((speed_fsm.current_state.speed_exclusive_state == TAP_MODE)
				|| (speed_fsm.current_state.shared_state == MANUAL_MODE)
				|| (speed_fsm.current_state.speed_exclusive_state == CLK_IN_MODE)
				|| (speed_fsm.current_state.shared_state == PC_MODE)
				|| (speed_fsm.current_state.shared_state == CC_MODE))){

			if(*rx_buffer == SYSTEM_REAL_TIME_START){

				speed_fsm.prev_state = speed_fsm.current_state;
				speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_PENDING_A0_MODE;

				MIDI_CLK_tag = 0; //just in case
			}
			else if(*rx_buffer == SYSTEM_REAL_TIME_MIDI_CLOCK){

				speed_fsm.prev_state = speed_fsm.current_state;
				speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_PENDING_B0_MODE;

				MIDI_CLK_tag = 0; //just in case

				HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
				HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

				MIDI_CLK_fsm = COMPILING;
				MIDI_CLK_tag++;
			}
		}
		else if(speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_PENDING_A0_MODE){

			if(*rx_buffer == SYSTEM_REAL_TIME_MIDI_CLOCK){

				HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
				HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

				MIDI_CLK_fsm = COMPILING;
				MIDI_CLK_tag++;

				speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_PENDING_A1_MODE;
				//DO NOT UPDATE PREV STATE - we need to keep track of what the state was prior to any pending state
			}
		}
		else if(speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_PENDING_A1_MODE){

			if(*rx_buffer == SYSTEM_REAL_TIME_MIDI_CLOCK){

				MIDI_CLK_tag++;

				if(MIDI_CLK_tag < 12){

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
				}
				else if(MIDI_CLK_tag < 25){

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
				}
				else{

					speed_fsm.prev_state.speed_exclusive_state = MIDI_CLK_PENDING_A1_MODE;
					speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_MODE;
					MIDI_CLK_tag = 1;
				}
			}
		}
		else if(speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_PENDING_B0_MODE){

			if(*rx_buffer == SYSTEM_REAL_TIME_MIDI_CLOCK){

				MIDI_CLK_tag++;

				if(MIDI_CLK_tag < 12){

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
				}
				else if(MIDI_CLK_tag < 25){

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
				}
				else{

					MIDI_CLK_tag = 1;
					//DO NOT UPDATE PREV STATE - we need to keep track of what the state was prior to any pending state
					speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_PENDING_B1_MODE;
				}
			}
		}
		else if(speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_PENDING_B1_MODE){

			if(*rx_buffer == SYSTEM_REAL_TIME_MIDI_CLOCK){

				MIDI_CLK_tag++;

				if(MIDI_CLK_tag < 12){

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
				}
				else if(MIDI_CLK_tag < 25){

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
				}
				else{

					MIDI_CLK_tag = 1;
				}
			}
			else if(*rx_buffer == SYSTEM_REAL_TIME_START){

				//DO NOT UPDATE PREV STATE - we need to keep track of what the state was prior to any pending state
				speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_PENDING_B2_MODE;
			}
		}
		else if(speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_PENDING_B2_MODE){

			if(*rx_buffer == SYSTEM_REAL_TIME_MIDI_CLOCK){

				HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
				HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

				//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

				// @TODO //WRITE CODE TO LOAD CORRECT DUTY DELAYED VALUE TO SECONDARY OSCILLATOR
				Set_Oscillator_Values(&params_to_be_loaded);

				speed_fsm.prev_state.speed_exclusive_state = MIDI_CLK_PENDING_B2_MODE;
				speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_MODE;

				MIDI_CLK_tag = 1;

				Stop_OC_TIM(&htim3, TIM_CHANNEL_1); //do we need?

				Copy_Params_Structs(&params_to_be_loaded, &params_working);
				Copy_Params_Structs(&params_to_be_loaded, &params);

				Set_Status_Bit(&statuses, First_Sync_Complete);

				Calculate_Next_Main_Oscillator_Values(&params, (enum Next_Values_Processing_Mode)REGULAR_MODE);
				Write_Next_Main_Oscillator_Values_to_Delay_Line(&params, &delay_line);
				HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions); //this function takes ages to execute!

				//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
			}
		}
		else if((speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_MODE) && (Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out) == NO)){

			if(*rx_buffer == SYSTEM_REAL_TIME_MIDI_CLOCK){

				MIDI_CLK_tag++;

				if(MIDI_CLK_tag < 12){

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
				}
				else if(MIDI_CLK_tag < 25){

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
				}
				else{

					MIDI_CLK_tag = 1;
				}
			}
		}
		else if((speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_MODE) && (IP_CAP_fsm.current_state == IDLE) && (Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out) == YES)){

			if(*rx_buffer == SYSTEM_REAL_TIME_MIDI_CLOCK){

				MIDI_CLK_tag = 0; //just in case
				MIDI_CLK_tag++;
				MIDI_CLK_fsm = COMPILING;

				speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_RESYNC_B0_MODE;
				speed_fsm.prev_state.speed_exclusive_state = MIDI_CLK_MODE;

				Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out);
			}
			else if(*rx_buffer == SYSTEM_REAL_TIME_START){

				MIDI_CLK_tag = 0; //just in case

				speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_RESYNC_A0_MODE;
				speed_fsm.prev_state.speed_exclusive_state = MIDI_CLK_MODE;

				Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out);
			}
		}
		else if(speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_RESYNC_A0_MODE){

			if(*rx_buffer == SYSTEM_REAL_TIME_MIDI_CLOCK){

				HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
				HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

				MIDI_CLK_fsm = COMPILING;
				MIDI_CLK_tag++;

				speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_RESYNC_A1_MODE;
				//DO NOT UPDATE PREV STATE - we need to keep track of what the state was prior to any pending state
			}
		}
		else if(speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_RESYNC_A1_MODE){

			if(*rx_buffer == SYSTEM_REAL_TIME_MIDI_CLOCK){

				MIDI_CLK_tag++;

				if(MIDI_CLK_tag < 12){

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
				}
				else if(MIDI_CLK_tag < 25){

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
				}
				else{

					speed_fsm.prev_state.speed_exclusive_state = MIDI_CLK_RESYNC_A1_MODE;
					speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_MODE;
					MIDI_CLK_tag = 1;
				}
			}
		}
		else if(speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_RESYNC_B0_MODE){

			if(*rx_buffer == SYSTEM_REAL_TIME_MIDI_CLOCK){

				//when sftwre timer times out the midi clk tag is reset to 0
				MIDI_CLK_tag++;

				if(MIDI_CLK_tag < 12){

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
				}
				else if(MIDI_CLK_tag < 25){

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
				}
				else{

					speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_RESYNC_B1_MODE;
					//DO NOT UPDATE PREV STATE - we need to keep track of what the state was prior to any pending state
					MIDI_CLK_tag = 1;
				}
			}
		}
		else if(speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_RESYNC_B1_MODE){

			if(*rx_buffer == SYSTEM_REAL_TIME_MIDI_CLOCK){

				MIDI_CLK_tag++;

				if(MIDI_CLK_tag < 12){

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
				}
				else if(MIDI_CLK_tag < 25){

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
				}
				else{

					MIDI_CLK_tag = 1;
				}
			}
			else if(*rx_buffer == SYSTEM_REAL_TIME_START){

				//DO NOT UPDATE PREV STATE - we need to keep track of what the state was prior to any pending state
				speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_RESYNC_B2_MODE;
			}
		}
		else if(speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_RESYNC_B2_MODE){

			if(*rx_buffer == SYSTEM_REAL_TIME_MIDI_CLOCK){

				HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
				HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

				//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

				// @TODO //WRITE CODE TO LOAD CORRECT DUTY DELAYED VALUE TO SECONDARY OSCILLATOR
				Set_Oscillator_Values(&params_to_be_loaded);

				speed_fsm.prev_state.speed_exclusive_state = MIDI_CLK_RESYNC_B2_MODE;
				speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_MODE;

				MIDI_CLK_tag = 1;

				Stop_OC_TIM(&htim3, TIM_CHANNEL_1); //do we need?

				Copy_Params_Structs(&params_to_be_loaded, &params_working);
				Copy_Params_Structs(&params_to_be_loaded, &params);

				Set_Status_Bit(&statuses, First_Sync_Complete);

				Calculate_Next_Main_Oscillator_Values(&params, (enum Next_Values_Processing_Mode)REGULAR_MODE);
				Write_Next_Main_Oscillator_Values_to_Delay_Line(&params, &delay_line);
				HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions); //this function takes ages to execute!

				//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
			}
		}
	}
	*rx_buffer = 0;

	HAL_UART_Receive_DMA(&huart2, (uint8_t*)rx_buffer, sizeof(rx_buffer));
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin){

	if((GPIO_Pin == CLK_IN_Pin)){ //if specifically CLK IN pin with falling interrupt

		if((speed_fsm.current_state.speed_exclusive_state == CLK_IN_MODE)
			|| (speed_fsm.current_state.speed_exclusive_state == CLK_IN_PENDING_MODE)
			|| (speed_fsm.current_state.speed_exclusive_state == CLK_IN_RESYNC_MODE)){

			HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
		}
	}
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

	if((GPIO_Pin == CLK_IN_Pin)){ //if specifically CLK IN pin with rising interrupt

		if(speed_fsm.current_state.shared_state == MANUAL_MODE){

			//Set SW OUT
			HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			speed_fsm.prev_state.shared_state = MANUAL_MODE;
			speed_fsm.current_state.speed_exclusive_state = CLK_IN_PENDING_MODE;
		}
		else if(speed_fsm.current_state.shared_state == PC_MODE){

			//Set SW OUT
			HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			speed_fsm.prev_state.shared_state = PC_MODE;
			speed_fsm.current_state.speed_exclusive_state = CLK_IN_PENDING_MODE;
		}
		else if(speed_fsm.current_state.shared_state == CC_MODE){

			//Set SW OUT
			HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			speed_fsm.prev_state.shared_state = CC_MODE;
			speed_fsm.current_state.speed_exclusive_state = CLK_IN_PENDING_MODE;
		}
		else if((speed_fsm.current_state.speed_exclusive_state == TAP_MODE) && (IP_CAP_fsm.current_state == IDLE)){

			//Set SW OUT
			HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			speed_fsm.prev_state.speed_exclusive_state = TAP_MODE;
			speed_fsm.current_state.speed_exclusive_state = CLK_IN_PENDING_MODE;
		}
		else if((speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_MODE) && (IP_CAP_fsm.current_state == IDLE)){

			//Set SW OUT
			HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			speed_fsm.prev_state.speed_exclusive_state = MIDI_CLK_MODE;
			speed_fsm.current_state.speed_exclusive_state = CLK_IN_PENDING_MODE;
		}

		//IF ALREADY IN PENDING MODE, CHECK FOR SECOND EDGE
		else if(speed_fsm.current_state.speed_exclusive_state == CLK_IN_PENDING_MODE){ //second edge

			//Set SW OUT
			HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			//ALREADY DONE IN IP CAP CALLBACK
			/*speed_fsm.prev_state.speed_exclusive_state = CLK_IN_PENDING_MODE;
			speed_fsm.current_state.speed_exclusive_state = CLK_IN_MODE;*/
		}

		//IF ALREADY IN CLK MODE
		else if(speed_fsm.current_state.speed_exclusive_state == CLK_IN_MODE){

			//Set SW OUT
			HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			speed_fsm.prev_state.speed_exclusive_state = CLK_IN_MODE;
			speed_fsm.current_state.speed_exclusive_state = CLK_IN_RESYNC_MODE;
		}

		//IF ALREADY IN RESYNC MODE, CHECK FOR SECOND EDGE
		else if(speed_fsm.current_state.speed_exclusive_state == CLK_IN_RESYNC_MODE){ //second edge

			//Set SW OUT
			HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			//ALREADY DONE IN IP CAP CALLBACK
			/*speed_fsm.prev_state.speed_exclusive_state = CLK_IN_RESYNC_MODE;
			speed_fsm.current_state.speed_exclusive_state = CLK_IN_MODE;*/
		}
	}

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
}

void LPTIM1_callback(LPTIM_HandleTypeDef *hlptim){

	static volatile struct Tap_Tempo_Switch_States tap_tempo_switch_states = {0};

	//CHECK IF NEED TO TAP_PENDING TRANSITION

	uint8_t pin_state = (uint8_t)HAL_GPIO_ReadPin(SW_IN_GPIO_Port, SW_IN_Pin);

	if((speed_fsm.current_state.shared_state == MANUAL_MODE) && (pin_state == 0)){

		speed_fsm.current_state.speed_exclusive_state = TAP_PENDING_MODE;
		speed_fsm.prev_state.shared_state = MANUAL_MODE;
	}
	else if((speed_fsm.current_state.shared_state == CC_MODE) && (pin_state == 0)){

		speed_fsm.current_state.speed_exclusive_state = TAP_PENDING_MODE;
		speed_fsm.prev_state.shared_state = CC_MODE;
	}
	else if((speed_fsm.current_state.shared_state == PC_MODE) && (pin_state == 0)){

		speed_fsm.current_state.speed_exclusive_state = TAP_PENDING_MODE;
		speed_fsm.prev_state.shared_state = PC_MODE;
	}
	else if((speed_fsm.current_state.speed_exclusive_state == CLK_IN_MODE) && (pin_state == 0) && IP_CAP_fsm.current_state == IDLE){

		speed_fsm.current_state.speed_exclusive_state = TAP_PENDING_MODE;
		speed_fsm.prev_state.speed_exclusive_state = CLK_IN_MODE;
	}
	else if((speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_MODE) && (pin_state == 0) && IP_CAP_fsm.current_state == IDLE){

		speed_fsm.current_state.speed_exclusive_state = TAP_PENDING_MODE;
		speed_fsm.prev_state.speed_exclusive_state = MIDI_CLK_MODE;
	}
	else if((speed_fsm.current_state.speed_exclusive_state == TAP_MODE) && (pin_state == 0) && IP_CAP_fsm.current_state == IDLE){

		speed_fsm.current_state.speed_exclusive_state = TAP_RESYNC_MODE;
		speed_fsm.prev_state.speed_exclusive_state = TAP_MODE;
	}

	//CHECK TAP TEMPO STATE
	if((speed_fsm.current_state.speed_exclusive_state == TAP_PENDING_MODE)
		|| (speed_fsm.current_state.speed_exclusive_state == TAP_RESYNC_MODE)
		|| (speed_fsm.current_state.speed_exclusive_state == TAP_MODE)){

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


	//PERFORM SPEED POT CHECKING
	if((speed_fsm.current_state.shared_state == PC_MODE) || (speed_fsm.current_state.shared_state == CC_MODE)){

		Speed_Pot_Check(&params);
	}
	else if(((speed_fsm.current_state.speed_exclusive_state == CLK_IN_MODE)) && (IP_CAP_fsm.current_state == IDLE)){

		Set_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);

		if(Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out) == YES){

			Speed_Pot_Check(&params);
		}
	}
	else if((speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_MODE) && (IP_CAP_fsm.current_state == IDLE)){

		Set_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);

		if(Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out) == YES){

			Speed_Pot_Check(&params);
		}
	}
	else if((speed_fsm.current_state.speed_exclusive_state == TAP_MODE) && (IP_CAP_fsm.current_state == IDLE)){

		Set_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);

		if(Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out) == YES){

			Speed_Pot_Check(&params);
		}
	}

	//SET TIMER TRIGGER
	HAL_LPTIM_SetOnce_Start_IT(&hlptim1, LPTIM1_CCR_CHECK, LPTIM1_CCR_CHECK);

}

void TIM17_callback(TIM_HandleTypeDef *htim){


}

void TIM14_callback(TIM_HandleTypeDef *htim){


}
