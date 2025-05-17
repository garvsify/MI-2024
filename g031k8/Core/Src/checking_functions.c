#include "checking_functions.h"
#include "main.h"

//VARIABLE DEFINITIONS
volatile uint8_t waveshape_pot_adc_measurement_num = 0;
volatile uint8_t speed_pot_adc_measurement_num = 0;
volatile uint8_t depth_pot_adc_measurement_num = 0;
volatile uint8_t symmetry_pot_adc_measurement_num = 0;
volatile uint8_t phase_pot_adc_measurement_num = 0;

//FUNCTION DEFINITIONS
uint8_t Pot_Check(struct Params* params_ptr, enum Pot_Type pot_type){

	static uint16_t first_waveshape_measurement;
	static uint16_t second_waveshape_measurement;

	static uint16_t first_speed_measurement;
	static uint16_t second_speed_measurement;

	static uint16_t first_depth_measurement;
	static uint16_t second_depth_measurement;

	static uint16_t first_symmetry_measurement;
	static uint16_t second_symmetry_measurement;

	static uint16_t first_phase_measurement;
	static uint16_t second_phase_measurement;

	uint16_t *first_measurement_ptr = NULL;
	uint16_t *second_measurement_ptr = NULL;
	volatile uint8_t *measurement_num_ptr = NULL;
	void *measurement_ptr = NULL;
	uint8_t tolerance = 0;
	volatile struct Normal_FSM* normal_fsm_ptr = NULL;
	volatile struct Speed_FSM* speed_fsm_ptr = NULL;

	if(pot_type == WAVESHAPE_POT){

		first_measurement_ptr = &first_waveshape_measurement;
		second_measurement_ptr = &second_waveshape_measurement;
		measurement_num_ptr = &waveshape_pot_adc_measurement_num;
		measurement_ptr = (uint16_t*)&params_ptr->waveshape;
		tolerance = WAVESHAPE_TOLERANCE;
		normal_fsm_ptr = &waveshape_fsm;
	}
	else if(pot_type == SPEED_POT){

		first_measurement_ptr = &first_speed_measurement;
		second_measurement_ptr = &second_speed_measurement;
		measurement_num_ptr = &speed_pot_adc_measurement_num;
		measurement_ptr = (uint16_t*)&params_ptr->speed;
		tolerance = SPEED_TOLERANCE;
		speed_fsm_ptr = &speed_fsm;
	}
	else if(pot_type == DEPTH_POT){

		first_measurement_ptr = &first_depth_measurement;
		second_measurement_ptr = &second_depth_measurement;
		measurement_num_ptr = &depth_pot_adc_measurement_num;
		measurement_ptr = (uint16_t*)&params_ptr->depth;
		tolerance = DEPTH_TOLERANCE;
		normal_fsm_ptr = &depth_fsm;
	}
	else if(pot_type == SYMMETRY_POT){

		first_measurement_ptr = &first_symmetry_measurement;
		second_measurement_ptr = &second_symmetry_measurement;
		measurement_num_ptr = &symmetry_pot_adc_measurement_num;
		measurement_ptr = (uint16_t*)&params_ptr->symmetry;
		tolerance = SYMMETRY_TOLERANCE;
		normal_fsm_ptr = &symmetry_fsm;
	}
	else if(pot_type == PHASE_POT){

		first_measurement_ptr = &first_phase_measurement;
		second_measurement_ptr = &second_phase_measurement;
		measurement_num_ptr = &phase_pot_adc_measurement_num;
		measurement_ptr = (uint16_t*)&params_ptr->duty_delay_line_read_pointer_offset;
		tolerance = PHASE_TOLERANCE;
		normal_fsm_ptr = &phase_fsm;
	}

	if(*measurement_num_ptr == 0){

		*first_measurement_ptr = *((uint16_t*)measurement_ptr);

			(*measurement_num_ptr)++;
	}
	else if(*measurement_num_ptr == 1){

		*second_measurement_ptr = *((uint16_t*)measurement_ptr);

		(*measurement_num_ptr)++;
	}
	else if(*measurement_num_ptr == 2){

		*measurement_num_ptr = 0;

		uint16_t diff = 0;

		if(*first_measurement_ptr > *second_measurement_ptr){

			diff = *first_measurement_ptr - *second_measurement_ptr;

		}
		else if(*second_measurement_ptr > *first_measurement_ptr){

			diff = *second_measurement_ptr - *first_measurement_ptr;
		}
		/*else{

			diff = 0
		}*/

		if(diff > tolerance){

			if(normal_fsm_ptr != NULL){

				normal_fsm_ptr->prev_state = normal_fsm_ptr->current_state;
				normal_fsm_ptr->current_state = MANUAL_MODE;

			}
			else if(speed_fsm_ptr != NULL){

				speed_fsm_ptr->prev_state = speed_fsm_ptr->current_state;
				speed_fsm_ptr->current_state.shared_state = MANUAL_MODE;
				Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out);
				Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);
				Clear_Status_Bit(&statuses, First_Sync_Complete);
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


