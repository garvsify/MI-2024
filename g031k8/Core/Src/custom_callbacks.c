#include "custom_callbacks.h"

//debug
uint64_t depressed_num = 0;
enum Validate latched = NO;
//debug

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
	Process_ADC_Conversion_Values(&params_manual, ADCResultsDMA);

	//copies into running params based on mode
	Update_Params_Based_On_Mode_Selected();

	enum Validate first_sync_complete = Get_Status_Bit(&statuses, First_Sync_Complete);

	//overwrites raw speed values if a sync has completed
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

			/*if(!((speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_PENDING_B1_MODE)
					|| (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_RESYNC_B1_MODE))){*/

				Start_Measurement_Reelapse_Timer();
			//}

			IP_CAP_fsm.current_state = MEASUREMENT_REELAPSE;
			IP_CAP_fsm.prev_state = MEASUREMENT_PENDING;

			Copy_Params_Structs(&params, &params_to_be_loaded);

			if((speed_fsm.current_state.speed_exclusive_state == TAP_PENDING_MODE) || (speed_fsm.current_state.speed_exclusive_state == CLK_IN_PENDING_MODE)){

				Advance_Pending_States();
			}

			//BEGIN PROCESSING
			Set_Status_Bit(&statuses, Input_Capture_Processing_Can_Be_Started);
		}
		/*//DO NOT ADD BACK IN - causes issues at highest speeds
		else{

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

		/*if(!((speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_PENDING_B1_MODE)
			|| (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_RESYNC_B1_MODE))){*/

			Start_Measurement_Reelapse_Timer();
		//}

		IP_CAP_fsm.current_state = MEASUREMENT_REELAPSE;
		IP_CAP_fsm.prev_state = MEASUREMENT_REELAPSE_AND_MEASUREMENT_PENDING;

		Copy_Params_Structs(&params, &params_to_be_loaded);

		if((speed_fsm.current_state.speed_exclusive_state == TAP_PENDING_MODE) || (speed_fsm.current_state.speed_exclusive_state == CLK_IN_PENDING_MODE)){

			Advance_Pending_States();
		}

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

		if(!((speed_fsm.current_state.speed_exclusive_state == TAP_MODE)
			|| (speed_fsm.current_state.speed_exclusive_state == CLK_IN_MODE) || (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_MODE))){

			speed_fsm.prev_state = speed_fsm.current_state;
			speed_fsm.current_state = previous;
		}
	}
	else if(IP_CAP_fsm.current_state == MEASUREMENT_REELAPSE_AND_MEASUREMENT_PENDING){

		IP_CAP_fsm.current_state = MEASUREMENT_REELAPSE;
		IP_CAP_fsm.prev_state = MEASUREMENT_REELAPSE_AND_MEASUREMENT_PENDING;

		MIDI_CLK_fsm = NOT_COMPILING;
		MIDI_CLK_tag = 0;

		HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1);
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);

		if(!((speed_fsm.current_state.speed_exclusive_state == TAP_MODE)
			|| (speed_fsm.current_state.speed_exclusive_state == CLK_IN_MODE) || (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_MODE))){

			speed_fsm.prev_state = speed_fsm.current_state;
			speed_fsm.current_state = previous;
		}
	}
}

void TIM3_ch1_IP_capture_measurement_reelapse_callback(TIM_HandleTypeDef *htim){

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

	if(!((speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_PENDING_B0_MODE)
		|| (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_PENDING_B1_MODE)
		|| (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_PENDING_B2_MODE)
		|| (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_RESYNC_B0_MODE)
		|| (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_RESYNC_B1_MODE)
		|| (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_RESYNC_B2_MODE))){

		// @TODO //WRITE CODE TO LOAD CORRECT DUTY DELAYED VALUE TO SECONDARY OSCILLATOR
		Set_Oscillator_Values(&params_to_be_loaded);
	}

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

	if(!((speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_PENDING_B0_MODE)
		|| (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_PENDING_B1_MODE)
		|| (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_PENDING_B2_MODE)
		|| (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_RESYNC_B0_MODE)
		|| (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_RESYNC_B1_MODE)
		|| (speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_RESYNC_B2_MODE))){

		Copy_Params_Structs(&params_to_be_loaded, &params_working);
		Copy_Params_Structs(&params_to_be_loaded, &params);

		Set_Status_Bit(&statuses, First_Sync_Complete);

		Calculate_Next_Main_Oscillator_Values(&params, (enum Next_Values_Processing_Mode)REGULAR_MODE);
		Write_Next_Main_Oscillator_Values_to_Delay_Line(&params, &delay_line);
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions); //this function takes ages to execute!

	}

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
}

void UART2_TX_transfer_complete_callback(UART_HandleTypeDef *huart){

	//UART_DMA_TX_is_complete = YES;
}

void UART2_RX_transfer_complete_callback(UART_HandleTypeDef *huart){

	if(Is_System_Real_Time_Status_Byte(rx_buffer) == YES){

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

					MIDI_CLK_fsm = COMPILING; //just in case
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

					MIDI_CLK_fsm = COMPILING; //just in case
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

					MIDI_CLK_fsm = COMPILING; //just in case
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

					// @TODO //WRITE CODE TO LOAD CORRECT DUTY DELAYED VALUE TO SECONDARY OSCILLATOR
					Set_Oscillator_Values(&params_to_be_loaded);

					//Give it another IP CAP edge upon sync
					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

					MIDI_CLK_fsm = COMPILING; //just in case
					MIDI_CLK_tag = 1;

					speed_fsm.prev_state.speed_exclusive_state = MIDI_CLK_PENDING_B2_MODE;
					speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_MODE;

					Stop_OC_TIM(&htim3, TIM_CHANNEL_1); //do we need?
					IP_CAP_fsm.current_state = IDLE; //force idle so next edge is forced to be computed as first edge

					Copy_Params_Structs(&params_to_be_loaded, &params_working);
					Copy_Params_Structs(&params_to_be_loaded, &params);

					Set_Status_Bit(&statuses, First_Sync_Complete);

					Calculate_Next_Main_Oscillator_Values(&params, (enum Next_Values_Processing_Mode)REGULAR_MODE);
					Write_Next_Main_Oscillator_Values_to_Delay_Line(&params, &delay_line);
					HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions); //this function takes ages to execute!
				}
			}
			else if((speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_MODE) && (Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out) == NO)){

				if(*rx_buffer == SYSTEM_REAL_TIME_MIDI_CLOCK){

					MIDI_CLK_fsm = COMPILING; //just in case
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
					Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);
				}
				else if(*rx_buffer == SYSTEM_REAL_TIME_START){

					MIDI_CLK_tag = 0; //just in case

					speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_RESYNC_A0_MODE;
					speed_fsm.prev_state.speed_exclusive_state = MIDI_CLK_MODE;

					Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out);
					Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);
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

					// @TODO //WRITE CODE TO LOAD CORRECT DUTY DELAYED VALUE TO SECONDARY OSCILLATOR
					Set_Oscillator_Values(&params_to_be_loaded);

					//Give it another IP CAP edge upon sync
					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

					speed_fsm.prev_state.speed_exclusive_state = MIDI_CLK_RESYNC_B2_MODE;
					speed_fsm.current_state.speed_exclusive_state = MIDI_CLK_MODE;

					MIDI_CLK_tag = 1;

					Stop_OC_TIM(&htim3, TIM_CHANNEL_1); //do we need?
					IP_CAP_fsm.current_state = IDLE; //force idle so next edge is forced to be computed as first edge

					Copy_Params_Structs(&params_to_be_loaded, &params_working);
					Copy_Params_Structs(&params_to_be_loaded, &params);

					Set_Status_Bit(&statuses, First_Sync_Complete);

					Calculate_Next_Main_Oscillator_Values(&params, (enum Next_Values_Processing_Mode)REGULAR_MODE);
					Write_Next_Main_Oscillator_Values_to_Delay_Line(&params, &delay_line);
					HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions); //this function takes ages to execute!
				}
			}
		}
	}
	//not a realtime status byte
	else{

		if(active_status_byte == 0){

			if(running_status_byte == 0){

				if(Is_Data_Buffer_Empty(&MIDI_data) == YES){

					if(Is_Status_Byte(rx_buffer) == YES){

						if(Is_PC_Status_Byte(rx_buffer) == YES){

							running_status_byte = (uint8_t)*rx_buffer;

							if(Is_Channelised_Status_Byte_On_Basic_Channel(rx_buffer, MIDI_basic_channel) == YES){

								active_status_byte = (uint8_t)*rx_buffer;
								Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

							}
							else{

								if(Is_OMNI_On(&statuses) == YES){

									active_status_byte = (uint8_t)*rx_buffer;
									Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

								}
							}
						}
						else if(Is_CC_Status_Byte(rx_buffer) == YES){

							running_status_byte = (uint8_t)*rx_buffer;

							if(Is_Channelised_Status_Byte_On_Basic_Channel(rx_buffer, MIDI_basic_channel) == YES){

								active_status_byte = (uint8_t)*rx_buffer;
								Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

							}
							else{

								if(Is_OMNI_On(&statuses) == YES){

									active_status_byte = (uint8_t)*rx_buffer;
									Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

								}
							}
						}
						else if(Is_Sysex_Start_Status_Byte(rx_buffer) == YES){

							active_status_byte = (uint8_t)*rx_buffer;
							running_status_byte = 0;
							Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

						}
					}
				}
			}
			else if(running_status_byte != 0){

				if(Is_Data_Byte(rx_buffer) == YES){

					Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

					if(Is_PC_Status_Byte(&running_status_byte) == YES){

						if((Is_Channelised_Status_Byte_On_Basic_Channel(&running_status_byte, MIDI_basic_channel) == YES)
																|| (Is_OMNI_On(&statuses) == YES)){

							if(Is_Data_Buffer_Empty(&MIDI_data) == YES){

								//first data byte received
								if(Is_Program_Change_Data_Byte_In_Range(rx_buffer, NUM_PRESETS) == YES){

									Set_All_Pots_to_PC_Mode();
									preset_selected = (enum Preset_Selected)*rx_buffer + 1; //since 0 is no preset selected, we have to add 1
									Update_Params_Based_On_Mode_Selected(); // Update parameters immediately with preset values
									Clear_Status_Bit(&statuses, First_Sync_Complete); //important for where a synced state (via MIDI CLK, CLK IN, or TAP) is the prior state
								}

								//whether the program change data byte is in range or not, clear the data buffer and active status byte, and reset timer
								Clear_Data_Buffer(&MIDI_data);
								active_status_byte = 0;

								//not really required
								Reset_and_Stop_MIDI_Software_Timer(&midi_counter, &statuses);

							}
						}
					}
					else if(Is_CC_Status_Byte(&running_status_byte) == YES){

						if(Is_Data_Buffer_Empty(&MIDI_data) == YES){

							//first data byte received
							MIDI_data.MIDI_data_buffer[0] = *rx_buffer;
							midi_counter = 0; //reset timer

						}
						else{ //not empty

							//second data byte received
							MIDI_data.MIDI_data_buffer[1] = *rx_buffer;
							Reset_and_Stop_MIDI_Software_Timer(&midi_counter, &statuses);

							//if a CC byte is active, either it was received on the basic channel, or OMNI is on, so we can
							//simply just use it, although we will need to check that when a channel mode message is sent,
							//that it was received on the basic channel

							if(Is_Utilised_Channel_Mode_CC_First_Data_Byte(&MIDI_data.MIDI_data_buffer[0]) == YES){

								//check on basic channel
								if(Is_Channelised_Status_Byte_On_Basic_Channel(&running_status_byte, MIDI_basic_channel) == YES){

									if(Channel_Mode_CC_Second_Data_Byte_Is_Valid_Given_Utilised_First_Data_Byte(&MIDI_data.MIDI_data_buffer[0], &MIDI_data.MIDI_data_buffer[1]) == YES){

										//Implement new channel mode
										if(MIDI_data.MIDI_data_buffer[1] == RESET_ALL_CONTROLLERS){

											Reset_All_Controllers(&params, &delay_line);
											Clear_Status_Bit(&statuses, First_Sync_Complete); //important for where a synced state (via MIDI CLK, CLK IN, or TAP) is the prior state
										}
										else if(MIDI_data.MIDI_data_buffer[1] == LOCAL_CONTROL){

											Set_Local_Control();
											Clear_Status_Bit(&statuses, First_Sync_Complete); //important for where a synced state (via MIDI CLK, CLK IN, or TAP) is the prior state
										}
										else if(MIDI_data.MIDI_data_buffer[1] == OMNI_MODE_OFF){

											Set_OMNI_Off(&statuses);
										}
										else if(MIDI_data.MIDI_data_buffer[1] == OMNI_MODE_ON){

											Set_OMNI_On(&statuses);
										}
									}
								}

								//not required
								active_status_byte = 0;
								Clear_Data_Buffer(&MIDI_data);

							}
							else if(Is_Utilised_CC_First_Data_Byte(&MIDI_data.MIDI_data_buffer[0]) == YES){

								if((Is_Channelised_Status_Byte_On_Basic_Channel(&running_status_byte, MIDI_basic_channel) == YES)
										|| (Is_OMNI_On(&statuses) == YES)){

									if(MIDI_data.MIDI_data_buffer[0] == WAVESHAPE_CC){

										Set_Waveshape_to_CC_Mode_and_Value((uint8_t*)&MIDI_data.MIDI_data_buffer[1]);
									}
									else if(MIDI_data.MIDI_data_buffer[0] == SPEED_CC){

										Set_Speed_to_CC_Mode_and_Value((uint8_t*)&MIDI_data.MIDI_data_buffer[1]);
										Clear_Status_Bit(&statuses, First_Sync_Complete); //important for where a synced state (via MIDI CLK, CLK IN, or TAP) is the prior state
									}
									else if(MIDI_data.MIDI_data_buffer[0] == DEPTH_CC){

										Set_Depth_to_CC_Mode_and_Value((uint8_t*)&MIDI_data.MIDI_data_buffer[1]);
									}
									else if(MIDI_data.MIDI_data_buffer[0] == SYMMETRY_CC){

										Set_Symmetry_to_CC_Mode_and_Value((uint8_t*)&MIDI_data.MIDI_data_buffer[1]);
									}
									else if(MIDI_data.MIDI_data_buffer[0] == PHASE_CC){

										Set_Phase_to_CC_Mode_and_Value((uint8_t*)&MIDI_data.MIDI_data_buffer[1]);
									}
								}

								active_status_byte = 0;
								Clear_Data_Buffer(&MIDI_data);
							}
							else{

								//not a utilised Ch mode message on basic channel, or utilised CC message
								active_status_byte = 0;
								Clear_Data_Buffer(&MIDI_data);
							}
						}
					}
					//don't need to check Sysex with running status, as not supported by MIDI
				}
				else{ // is a status byte -> status byte interrupts an active status byte (when data bytes should be being received)

					Clear_Data_Buffer(&MIDI_data);
					Reset_and_Stop_MIDI_Software_Timer(&midi_counter, &statuses);

					if(Is_PC_Status_Byte(rx_buffer) == YES){

						running_status_byte = (uint8_t)*rx_buffer;

						if(Is_Channelised_Status_Byte_On_Basic_Channel(rx_buffer, MIDI_basic_channel) == YES){

							active_status_byte = (uint8_t)*rx_buffer;
							Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

						}
						else{

							if(Is_OMNI_On(&statuses) == YES){

								active_status_byte = (uint8_t)*rx_buffer;
								Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

							}
						}
					}
					else if(Is_CC_Status_Byte(rx_buffer) == YES){

						running_status_byte = (uint8_t)*rx_buffer;

						if(Is_Channelised_Status_Byte_On_Basic_Channel(rx_buffer, MIDI_basic_channel) == YES){

							active_status_byte = (uint8_t)*rx_buffer;
							Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

						}
						else{

							if(Is_OMNI_On(&statuses) == YES){

								active_status_byte = (uint8_t)*rx_buffer;
								Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

							}
						}
					}
					else if(Is_Sysex_Start_Status_Byte(rx_buffer) == YES){

						active_status_byte = (uint8_t)*rx_buffer;
						running_status_byte = 0;
						Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

					}
				}
			}
		}
		else if(active_status_byte != 0){ //CC/PC only called if on basic channel or omni ON

			if(Get_Status_Bit(&statuses, Software_MIDI_Timer_Has_Timed_Out) == YES){

				active_status_byte = 0;
				Clear_Data_Buffer(&MIDI_data);
				//running status is kept

				Reset_and_Stop_MIDI_Software_Timer(&midi_counter, &statuses);

				//In this condition, the data bytes haven't been received in enough time, so any subsequent data bytes
				//sent after this are simply ignored
			}
			else{ //not timed out

				if(Is_Data_Byte(rx_buffer) == YES){

					if(Is_PC_Status_Byte(&active_status_byte) == YES){

						if(Is_Data_Buffer_Empty(&MIDI_data) == YES){

							//first data byte received
							if(Is_Program_Change_Data_Byte_In_Range(rx_buffer, NUM_PRESETS) == YES){

								Set_All_Pots_to_PC_Mode();
								preset_selected = (enum Preset_Selected)*rx_buffer + 1; //since 0 is no preset selected, we have to add 1
								Update_Params_Based_On_Mode_Selected(); // Update parameters immediately with preset values
								Clear_Status_Bit(&statuses, First_Sync_Complete); //important for where a synced state (via MIDI CLK, CLK IN, or TAP) is the prior state
							}

							//whether the program change data byte is in range or not, clear the data buffer and active status byte, and reset timer
							Clear_Data_Buffer(&MIDI_data);
							active_status_byte = 0;

							Reset_and_Stop_MIDI_Software_Timer(&midi_counter, &statuses);

						}
					}
					else if(Is_CC_Status_Byte(&active_status_byte) == YES){

						if(Is_Data_Buffer_Empty(&MIDI_data) == YES){

							//first data byte received
							MIDI_data.MIDI_data_buffer[0] = *rx_buffer;
							midi_counter = 0; //reset timer

						}
						else{ //not empty

							//second data byte received
							MIDI_data.MIDI_data_buffer[1] = *rx_buffer;
							Reset_and_Stop_MIDI_Software_Timer(&midi_counter, &statuses);
							Clear_Status_Bit(&statuses, First_Sync_Complete); //important for where a synced state (via MIDI CLK, CLK IN, or TAP) is the prior state

							//if a CC byte is active, either it was received on the basic channel, or OMNI is on, so we can
							//simply just use it, although we will need to check that when a channel mode message is sent,
							//that it was received on the basic channel

							if(Is_Utilised_Channel_Mode_CC_First_Data_Byte(&MIDI_data.MIDI_data_buffer[0]) == YES){

								//check on basic channel
								if(Is_Channelised_Status_Byte_On_Basic_Channel(&active_status_byte, MIDI_basic_channel) == YES){

									if(Channel_Mode_CC_Second_Data_Byte_Is_Valid_Given_Utilised_First_Data_Byte(&MIDI_data.MIDI_data_buffer[0], &MIDI_data.MIDI_data_buffer[1]) == YES){

										//Implement new channel mode
										if(MIDI_data.MIDI_data_buffer[1] == RESET_ALL_CONTROLLERS){

											Reset_All_Controllers(&params, &delay_line);
											Clear_Status_Bit(&statuses, First_Sync_Complete); //important for where a synced state (via MIDI CLK, CLK IN, or TAP) is the prior state
										}
										else if(MIDI_data.MIDI_data_buffer[1] == LOCAL_CONTROL){

											Set_Local_Control();
											Clear_Status_Bit(&statuses, First_Sync_Complete); //important for where a synced state (via MIDI CLK, CLK IN, or TAP) is the prior state
										}
										else if(MIDI_data.MIDI_data_buffer[1] == OMNI_MODE_OFF){

											Set_OMNI_Off(&statuses);
										}
										else if(MIDI_data.MIDI_data_buffer[1] == OMNI_MODE_ON){

											Set_OMNI_On(&statuses);
										}
									}
								}

								active_status_byte = 0;
								Clear_Data_Buffer(&MIDI_data);

							}
							else if(Is_Utilised_CC_First_Data_Byte(&MIDI_data.MIDI_data_buffer[0]) == YES){

								if((Is_Channelised_Status_Byte_On_Basic_Channel(&active_status_byte, MIDI_basic_channel) == YES)
										|| (Is_OMNI_On(&statuses) == YES)){

									if(MIDI_data.MIDI_data_buffer[0] == WAVESHAPE_CC){

										Set_Waveshape_to_CC_Mode_and_Value((uint8_t*)&MIDI_data.MIDI_data_buffer[1]);
									}
									else if(MIDI_data.MIDI_data_buffer[0] == SPEED_CC){

										Set_Speed_to_CC_Mode_and_Value((uint8_t*)&MIDI_data.MIDI_data_buffer[1]);
										Clear_Status_Bit(&statuses, First_Sync_Complete); //important for where a synced state (via MIDI CLK, CLK IN, or TAP) is the prior state
									}
									else if(MIDI_data.MIDI_data_buffer[0] == DEPTH_CC){

										Set_Depth_to_CC_Mode_and_Value((uint8_t*)&MIDI_data.MIDI_data_buffer[1]);
									}
									else if(MIDI_data.MIDI_data_buffer[0] == SYMMETRY_CC){

										Set_Symmetry_to_CC_Mode_and_Value((uint8_t*)&MIDI_data.MIDI_data_buffer[1]);
									}
									else if(MIDI_data.MIDI_data_buffer[0] == PHASE_CC){

										Set_Phase_to_CC_Mode_and_Value((uint8_t*)&MIDI_data.MIDI_data_buffer[1]);
									}
								}

								active_status_byte = 0;
								Clear_Data_Buffer(&MIDI_data);
							}
							else{

								//not a utilised Ch mode message on basic channel, or utilised CC message
								active_status_byte = 0;
								Clear_Data_Buffer(&MIDI_data);
							}
						}
					}
					else if(Is_Sysex_Start_Status_Byte(&active_status_byte) == YES){

						//@TODO

						if(Is_Data_Buffer_Empty(&MIDI_data) == YES){

							//first data byte received
						}
						else{


						}

					}
				}
				else{ // is a status byte -> status byte interrupts an active status byte (when data bytes should be being received)

					Clear_Data_Buffer(&MIDI_data);
					Reset_and_Stop_MIDI_Software_Timer(&midi_counter, &statuses);

					if(Is_PC_Status_Byte(rx_buffer) == YES){

						running_status_byte = (uint8_t)*rx_buffer;

						if(Is_Channelised_Status_Byte_On_Basic_Channel(rx_buffer, MIDI_basic_channel) == YES){

							active_status_byte = (uint8_t)*rx_buffer;
							Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

						}
						else{

							if(Is_OMNI_On(&statuses) == YES){

								active_status_byte = (uint8_t)*rx_buffer;
								Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

							}
						}
					}
					else if(Is_CC_Status_Byte(rx_buffer) == YES){

						running_status_byte = (uint8_t)*rx_buffer;

						if(Is_Channelised_Status_Byte_On_Basic_Channel(rx_buffer, MIDI_basic_channel) == YES){

							active_status_byte = (uint8_t)*rx_buffer;
							Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

						}
						else{

							if(Is_OMNI_On(&statuses) == YES){

								active_status_byte = (uint8_t)*rx_buffer;
								Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

							}
						}
					}
					else if(Is_Sysex_Start_Status_Byte(rx_buffer) == YES){

						active_status_byte = (uint8_t)*rx_buffer;
						running_status_byte = 0;
						Set_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);

					}
				}
			}
		}
	}

	*rx_buffer = 0;

	HAL_UART_Receive_DMA(&huart2, (uint8_t*)rx_buffer, sizeof(rx_buffer));
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin){

	if((GPIO_Pin == CLK_IN_Pin)){ //if specifically CLK IN pin with falling interrupt

		if((speed_fsm.current_state.speed_exclusive_state == CLK_IN_MODE) || (speed_fsm.current_state.speed_exclusive_state == CLK_IN_PENDING_MODE)){

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
		else if((speed_fsm.current_state.speed_exclusive_state == TAP_MODE) && (IP_CAP_fsm.current_state == IDLE) && (Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out) == YES)){

			//Set SW OUT
			HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			speed_fsm.prev_state.speed_exclusive_state = TAP_MODE;
			speed_fsm.current_state.speed_exclusive_state = CLK_IN_PENDING_MODE;

			Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out);
			Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);
		}
		else if((speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_MODE) && (IP_CAP_fsm.current_state == IDLE) && (Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out) == YES)){

			//Set SW OUT
			HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			speed_fsm.prev_state.speed_exclusive_state = MIDI_CLK_MODE;
			speed_fsm.current_state.speed_exclusive_state = CLK_IN_PENDING_MODE;

			Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out);
			Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);
		}

		//IF ALREADY IN PENDING MODE, CHECK FOR SECOND EDGE
		else if(speed_fsm.current_state.speed_exclusive_state == CLK_IN_PENDING_MODE){ //second edge

			//Set SW OUT
			HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			speed_fsm.prev_state.speed_exclusive_state = CLK_IN_PENDING_MODE;
			speed_fsm.current_state.speed_exclusive_state = CLK_IN_MODE;
		}

		//IF ALREADY IN CLK MODE
		else if(speed_fsm.current_state.speed_exclusive_state == CLK_IN_MODE){

			//Set SW OUT
			HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
		}

	}

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
}

void LPTIM1_callback(LPTIM_HandleTypeDef *hlptim){

	static volatile struct Tap_Tempo_Switch_States tap_tempo_switch_states = {0};
	static volatile enum Validate preset_save_mode_is_inactive = YES;

	//CHECK IF NEED TAP_PENDING TRANSITION

	uint8_t pin_state = (uint8_t)HAL_GPIO_ReadPin(SW_IN_GPIO_Port, SW_IN_Pin);

	if(preset_save_mode_is_inactive == YES){

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
		else if((speed_fsm.current_state.speed_exclusive_state == CLK_IN_MODE) && (pin_state == 0) && (IP_CAP_fsm.current_state == IDLE) && (Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out) == YES)){

			speed_fsm.current_state.speed_exclusive_state = TAP_PENDING_MODE;
			speed_fsm.prev_state.speed_exclusive_state = CLK_IN_MODE;

			Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out);
			Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);
		}
		else if((speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_MODE) && (pin_state == 0) && (IP_CAP_fsm.current_state == IDLE) && (Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out) == YES)){

			speed_fsm.current_state.speed_exclusive_state = TAP_PENDING_MODE;
			speed_fsm.prev_state.speed_exclusive_state = MIDI_CLK_MODE;

			Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out);
			Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);
		}

		//CHECK TAP TEMPO STATE

		if((speed_fsm.current_state.speed_exclusive_state == TAP_PENDING_MODE) || (speed_fsm.current_state.speed_exclusive_state == TAP_MODE)){

			if(tap_tempo_switch_states.tap_tempo_switch_state == DEPRESSED){

				//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
				HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 0);
				//HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			}
			else if(tap_tempo_switch_states.tap_tempo_switch_state == NOT_DEPRESSED){

				//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);
				HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1); //reset
				//HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
			}
		}

		//CHECK FOR SPEED POT CHANGES
		if(Get_Status_Bit(&statuses, Pots_Counter_Has_Timed_Out) == YES){

			Clear_Status_Bit(&statuses, Pots_Counter_Has_Timed_Out);

			pots_counter = 0;

			//PERFORM SPEED POT CHECKING
			if((speed_fsm.current_state.shared_state == PC_MODE) || (speed_fsm.current_state.shared_state == CC_MODE)){

				Pot_Check(ADCResultsDMA, SPEED_POT);
			}
			else if((speed_fsm.current_state.speed_exclusive_state == CLK_IN_MODE) && (IP_CAP_fsm.current_state == IDLE)){

				Set_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);

				if(Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out) == YES){

					Pot_Check(ADCResultsDMA, SPEED_POT);
				}
			}
			else if((speed_fsm.current_state.speed_exclusive_state == MIDI_CLK_MODE) && (IP_CAP_fsm.current_state == IDLE)){

				Set_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);

				if(Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out) == YES){

					HAL_GPIO_WritePin(SW_OUT_GPIO_Port, SW_OUT_Pin, 1); //reset
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);

					MIDI_CLK_fsm = NOT_COMPILING;
					MIDI_CLK_tag = 0;

					Pot_Check(ADCResultsDMA, SPEED_POT);
				}
			}
			else if((speed_fsm.current_state.speed_exclusive_state == TAP_MODE) && (IP_CAP_fsm.current_state == IDLE)){

				Set_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);

				if(Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out) == YES){

					Pot_Check(ADCResultsDMA, SPEED_POT);
				}
			}

			if((waveshape_fsm.current_state == PC_MODE) || (waveshape_fsm.current_state == CC_MODE)){
				Pot_Check(ADCResultsDMA, WAVESHAPE_POT);
			}
			if((depth_fsm.current_state == PC_MODE) || (depth_fsm.current_state == CC_MODE)){
				Pot_Check(ADCResultsDMA, DEPTH_POT);
			}
			if((symmetry_fsm.current_state == PC_MODE) || (symmetry_fsm.current_state == CC_MODE)){
				Pot_Check(ADCResultsDMA, SYMMETRY_POT);
			}
			if((phase_fsm.current_state == PC_MODE) || (phase_fsm.current_state == CC_MODE)){
				Pot_Check(ADCResultsDMA, PHASE_POT);
			}

		}
		else{

			if(pots_counter == POT_COUNTER_COUNT){

				Set_Status_Bit(&statuses, Pots_Counter_Has_Timed_Out);
			}
			else{

				pots_counter++;
			}
		}
	}

	//check if tap tempo switch is depressed
	Check_Tap_Tempo_Switch_State(&tap_tempo_switch_states);

	//CHECK IF TAP TEMPO HELD DOWN - PRESET SAVE MODE

	static uint64_t depressed_num;
	static enum Validate latched;
	static enum Preset_Selected preset = PRESET_ONE;
	static enum Validate first_time = YES;
	enum LED_States led_state;
	static enum LED_States led_state_saved;

	led_state = LED_fsm.current_state;

	if(tap_tempo_switch_states.tap_tempo_switch_state == NOT_DEPRESSED){

		if(preset_save_mode_is_inactive == NO){

			//if preset save timer has timed out, come out of preset save mode
			if(Get_Status_Bit(&statuses, Tap_Tempo_Preset_Save_Timer_Has_Timed_Out) == YES){

				preset = PRESET_ONE;
				preset_save_mode_is_inactive = YES;
				first_time = YES;
				LED_fsm.prev_state = LED_fsm.current_state;
				LED_fsm.current_state = led_state_saved;

				Clear_Status_Bit(&statuses, Tap_Tempo_Preset_Save_Timer_Has_Timed_Out);
			}
			if((depressed_num >= TAP_TEMPO_SWITCH_PRESET_SAVE_COUNT) && (depressed_num < TAP_TEMPO_SWITCH_PRESET_SAVE_MODE_ADVANCE_COUNT)){

				//get correct preset, as it will be 'off by one'
				if(preset == PRESET_ONE){

					preset = PRESET_FOUR;
				}
				else if(preset == PRESET_TWO){

					preset = PRESET_ONE;
				}
				else if(preset == PRESET_THREE){

					preset = PRESET_TWO;
				}
				else if(preset == PRESET_FOUR){

					preset = PRESET_THREE;
				}

				//convert running params to preset, and update user preset and user preset used
				Store_Params_as_User_Preset(preset,
											&params,
											user_presets_used_array,
											user_presets_array,
											factory_presets_array,
											presets_converted_array);

				//set the current pot mode to PC_MODE and update current preset active
				Set_All_Pots_to_PC_Mode();
				preset_selected = preset;

				//store presets in flash
				Store_Single_Preset_In_Flash(user_presets_array[(uint8_t)(preset_selected - 1)], (uint8_t)(preset_selected - 1));

				//led confirm - overwrite prev state with saved state
				set_LED_to_state(&LED_fsm, LED_CONFIRM);
				LED_fsm.prev_state = led_state_saved;

				preset = PRESET_ONE;
				preset_save_mode_is_inactive = YES;
				first_time = YES;
			}

			if(speed_fsm.current_state.speed_exclusive_state == TAP_PENDING_MODE){

				union Speed_FSM_States curr_state = speed_fsm.current_state;
				speed_fsm.current_state = speed_fsm.prev_state;
				speed_fsm.prev_state = curr_state;
			}
		}
		else{

			if(speed_fsm.current_state.speed_exclusive_state == TAP_PENDING_MODE){

				union Speed_FSM_States curr_state = speed_fsm.current_state;
				speed_fsm.current_state = speed_fsm.prev_state;
				speed_fsm.prev_state = curr_state;
			}
		}

		depressed_num = 0;
		latched = NO;
	}
	else if(tap_tempo_switch_states.tap_tempo_switch_state == DEPRESSED){

		if(Get_Status_Bit(&statuses, Tap_Tempo_Preset_Save_Timer_Is_Running) == YES){

			preset_save_idle_counter = 0;
		}
		if((depressed_num < TAP_TEMPO_SWITCH_PRESET_SAVE_MODE_ADVANCE_COUNT) && (depressed_num < TAP_TEMPO_SWITCH_PRESET_SAVE_COUNT)){

			//save count will always be less than advance count
			depressed_num++;
		}
		else if((depressed_num < TAP_TEMPO_SWITCH_PRESET_SAVE_MODE_ADVANCE_COUNT) && (depressed_num >= TAP_TEMPO_SWITCH_PRESET_SAVE_COUNT)){

			//Clear_Status_Bit(&statuses, Short_Press);
			depressed_num++;
		}
		else{

			preset_save_mode_is_inactive = NO;

			//start counter
			Set_Status_Bit(&statuses, Tap_Tempo_Preset_Save_Timer_Is_Running);
			//reset counter
			preset_save_idle_counter = 0;

			depressed_num++; //may be an issue if overflow

			if(first_time == YES){

				first_time = NO;
				led_state_saved = led_state;
			}

			if(latched == NO){

				latched = YES;

				if(preset == PRESET_ONE){

					set_LED_to_state(&LED_fsm, LED_ONE_BLINK);
					preset = PRESET_TWO;
				}
				else if(preset == PRESET_TWO){

					set_LED_to_state(&LED_fsm, LED_TWO_BLINK);
					preset = PRESET_THREE;
				}
				else if(preset == PRESET_THREE){

					set_LED_to_state(&LED_fsm, LED_THREE_BLINK);
					preset = PRESET_FOUR;
				}
				else if(preset == PRESET_FOUR){

					set_LED_to_state(&LED_fsm, LED_FOUR_BLINK);
					preset = PRESET_ONE;
				}
			}

			if(speed_fsm.current_state.speed_exclusive_state == TAP_PENDING_MODE){

				union Speed_FSM_States curr_state = speed_fsm.current_state;
				speed_fsm.current_state = speed_fsm.prev_state;
				speed_fsm.prev_state = curr_state;
			}
		}
	}

	//SET TIMER TRIGGER
	HAL_LPTIM_SetOnce_Start_IT(&hlptim1, LPTIM1_CCR_CHECK, LPTIM1_CCR_CHECK);

}

void TIM17_callback(TIM_HandleTypeDef *htim){


}

void TIM14_callback(TIM_HandleTypeDef *htim){

	if(LED_fsm.current_state == LED_ON){

		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
	}
	else if(LED_fsm.current_state == LED_OFF){

		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
	}
	else if(LED_fsm.current_state == LED_ONE_BLINK){

		__HAL_TIM_SET_COUNTER(&htim14, 0);

		if(LED_counter == 0){

			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			LED_counter++;
		}
		else if(LED_counter < (LED_COUNT_OFF_TIME + 1)){

			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);

			LED_counter++;

			if(LED_counter == LED_COUNT_OFF_TIME + 1){

				LED_counter = 0;
			}
		}
	}
	else if(LED_fsm.current_state == LED_TWO_BLINK){

		__HAL_TIM_SET_COUNTER(&htim14, 0);

		if((LED_counter < 3) && (LED_counter % 2 == 0)){

			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			LED_counter++;
		}
		else if((LED_counter < 3) && (LED_counter % 2 == 1)){

			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
			LED_counter++;
		}
		else if(LED_counter < (LED_COUNT_OFF_TIME + 3)){

			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
			LED_counter++;

			if(LED_counter == LED_COUNT_OFF_TIME + 3){

				LED_counter = 0;
			}
		}
	}
	else if(LED_fsm.current_state == LED_THREE_BLINK){

		__HAL_TIM_SET_COUNTER(&htim14, 0);

		if((LED_counter < 5) && (LED_counter % 2 == 0)){

			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			LED_counter++;
		}
		else if((LED_counter < 5) && (LED_counter % 2 == 1)){

			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);

			LED_counter++;
		}
		else if(LED_counter < (LED_COUNT_OFF_TIME + 5)){

			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);

			LED_counter++;

			if(LED_counter == LED_COUNT_OFF_TIME + 5){

				LED_counter = 0;
			}
		}
	}
	else if(LED_fsm.current_state == LED_FOUR_BLINK){

		__HAL_TIM_SET_COUNTER(&htim14, 0);

		if((LED_counter < 7) && (LED_counter % 2 == 0)){

			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			LED_counter++;
		}
		else if((LED_counter < 7) && (LED_counter % 2 == 1)){

			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);

			LED_counter++;
		}
		else if(LED_counter < (LED_COUNT_OFF_TIME + 7)){

			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);

			LED_counter++;

			if(LED_counter == LED_COUNT_OFF_TIME + 7){

				LED_counter = 0;
			}
		}
	}
	else if(LED_fsm.current_state == LED_CONFIRM){

		__HAL_TIM_SET_COUNTER(&htim14, 0);

		if((LED_counter < 7) && (LED_counter % 2 == 0)){

			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);

			LED_counter++;
		}
		else if((LED_counter < 7) && (LED_counter % 2 == 1)){

			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);

			LED_counter++;
		}
		else if(LED_counter < (LED_COUNT_OFF_TIME + 7)){

			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);

			LED_counter++;

			if(LED_counter == LED_COUNT_OFF_TIME + 7){

				enum LED_States prev = LED_fsm.prev_state;

				set_LED_to_state(&LED_fsm, prev);
			}
		}
	}
}
