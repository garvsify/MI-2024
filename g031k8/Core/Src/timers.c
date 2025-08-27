//INCLUDES
#include "timers.h"
#include "main_vars.h"

//FUNCTION DEFINITIONS
uint8_t __attribute__((optimize("O0")))Start_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t OC_TIM_channel){

	uint8_t ok = HAL_TIM_OC_Start_IT(TIM, OC_TIM_channel); //_IT variant of function
	//means the timer will generate an interrupt on delay_elapsed (CNT = CCR) condition

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t __attribute__((optimize("O0")))Stop_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t OC_TIM_channel){

	uint8_t ok = HAL_TIM_OC_Stop_IT(TIM, OC_TIM_channel);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t __attribute__((optimize("O0")))Check_Software_Timers(void){

	if(Get_Status_Bit(&statuses, Input_Capture_Processing_Can_Be_Started) == YES){

				//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

				Input_Capture_Processing(interrupt_period, &params_to_be_loaded);

				//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
			}

			if(Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running) == YES){

				if(idle_counter < IDLE_COUNT){

					if(IP_CAP_fsm.current_state != IDLE){

						Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);
						Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out);
						idle_counter = 0;
					}
					else{

						idle_counter++;
					}
				}
				else{

					Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);
					Set_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out);
					idle_counter = 0;
				}
			}
			if(Get_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running) == YES){

				if(midi_counter < MIDI_COUNT){

					midi_counter++;
				}
				else{

					Clear_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);
					Set_Status_Bit(&statuses, Software_MIDI_Timer_Has_Timed_Out);
					active_status_byte = 0;
					Clear_Data_Buffer(&MIDI_data);
					midi_counter = 0;
				}
			}
			if(Get_Status_Bit(&statuses, Tap_Tempo_Preset_Save_Timer_Is_Running) == YES){

				if(preset_save_idle_counter < PRESET_SAVE_IDLE_COUNT){

					preset_save_idle_counter++;
				}
				else{

					Clear_Status_Bit(&statuses, Tap_Tempo_Preset_Save_Timer_Is_Running);
					Set_Status_Bit(&statuses, Tap_Tempo_Preset_Save_Timer_Has_Timed_Out);
					preset_save_idle_counter = 0;
				}
			}

			return 1;
}

uint8_t Start_DMA_Timer(void){

	Stop_OC_TIM(&htim17, TIM_CHANNEL_1);
	__HAL_TIM_SET_COUNTER(&htim17, 0);
	__HAL_TIM_SET_COMPARE(&htim17, TIM_CHANNEL_1, TIM17_PERIOD_COUNT);
	Start_OC_TIM(&htim17, TIM_CHANNEL_1);

	return 1;
}
