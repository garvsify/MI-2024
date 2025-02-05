#include "checking_functions.h"

//VARIABLE DEFINITIONS

//FUNCTION DEFINITIONS
uint8_t Speed_Pot_Check(struct Params* params_ptr){

	static volatile uint8_t speed_pot_adc_measurement_num;

	static uint16_t first_speed_measurement;

	static uint16_t second_speed_measurement;

	if(speed_pot_adc_measurement_num == 0){

		first_speed_measurement = params_ptr->speed;

		speed_pot_adc_measurement_num++;
	}
	else if(speed_pot_adc_measurement_num == 1){

		second_speed_measurement = params_ptr->speed;

		speed_pot_adc_measurement_num++;
	}
	else if(speed_pot_adc_measurement_num == 2){

		speed_pot_adc_measurement_num = 0;

		if(first_speed_measurement > second_speed_measurement){

			if(first_speed_measurement - second_speed_measurement > SPEED_TOLERANCE){

				state = STATE_0;
				Clear_Status_Bit(&statuses, First_Sync_Complete);
				Clear_Status_Bit(&statuses, IP_CAP_Events_Detection_Timeout);
			}
		}
		else if(second_speed_measurement > first_speed_measurement){

			if(second_speed_measurement - first_speed_measurement > SPEED_TOLERANCE){

				state = STATE_0;
				Clear_Status_Bit(&statuses, First_Sync_Complete);
				Clear_Status_Bit(&statuses, IP_CAP_Events_Detection_Timeout);
			}
		}
	}
	return 1;
}

uint8_t Check_Tap_Tempo_Switch_State(volatile struct Tap_Tempo_Switch_States *tap_tempo_switch_states_ptr){

	static uint8_t extend_rising_edge = 0;
	static uint8_t tap_tempo_switch_state_counter = TAP_TEMPO_SWITCH_CONFIDENCE_COUNT;

	uint8_t switch_state = (uint8_t)HAL_GPIO_ReadPin(SW_IN_GPIO_Port, SW_IN_Pin);

	if(switch_state == 0){

		if(tap_tempo_switch_state_counter != 0){

			tap_tempo_switch_state_counter--;
		}
	}
	else{

		if(tap_tempo_switch_state_counter != TAP_TEMPO_SWITCH_CONFIDENCE_COUNT){

			if(extend_rising_edge == COUNT_TO_DELAY_RISING_TAP_TEMPO_EDGE){

				tap_tempo_switch_state_counter++;
				extend_rising_edge = 0;
			}
			else{
				extend_rising_edge++;
			}
		}
	}

	if(tap_tempo_switch_state_counter == 0){

		tap_tempo_switch_states_ptr->tap_tempo_switch_state = DEPRESSED;
	}
	else if(tap_tempo_switch_state_counter == TAP_TEMPO_SWITCH_CONFIDENCE_COUNT){

		tap_tempo_switch_states_ptr->tap_tempo_switch_state = NOT_DEPRESSED;
	}

	return 1;
}
