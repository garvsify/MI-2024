#include "checking.h"

//VARIABLE DEFINITIONS
volatile uint8_t tap_tempo_switch_state_counter = TAP_TEMPO_SWITCH_CONFIDENCE_COUNT;
volatile enum Tap_Tempo_Switch_State tap_tempo_switch_state = NOT_DEPRESSED;
volatile enum Validate external_clock_mode_is_active = NO;
volatile enum Validate tap_tempo_mode_is_active = NO;
volatile uint8_t speed_pot_adc_measurement_num = 0;

//FUNCTION DEFINITIONS
uint8_t Speed_Pot_Check(struct Params* params_ptr){

	if((tap_tempo_mode_is_active == YES) || (external_clock_mode_is_active == YES)){

		static uint16_t first_speed_measurement;

		static uint16_t second_speed_measurement;

		if(speed_pot_adc_measurement_num == 0){

			first_speed_measurement = params_ptr->speed;
			speed_pot_adc_measurement_num = 1;
		}
		else if(speed_pot_adc_measurement_num == 1){

			second_speed_measurement = params_ptr->speed;
			speed_pot_adc_measurement_num = 2;
		}
		else if(speed_pot_adc_measurement_num == 2){

			speed_pot_adc_measurement_num = 0;

			if(first_speed_measurement > second_speed_measurement){

				if(first_speed_measurement - second_speed_measurement > SPEED_TOLERANCE){

					tap_tempo_mode_is_active = NO;
					external_clock_mode_is_active = NO;

					//STOP SPEED POT CHECKING
					Stop_OC_TIM(&htim17, TIM_CHANNEL_1);
				}
			}
			else if(second_speed_measurement > first_speed_measurement){

				if(second_speed_measurement - first_speed_measurement > SPEED_TOLERANCE){

					tap_tempo_mode_is_active = NO;
					external_clock_mode_is_active = NO;

					//STOP SPEED POT CHECKING
					Stop_OC_TIM(&htim17, TIM_CHANNEL_1);
				}
			}
		}
	}
	return 1;
}

uint8_t Check_Tap_Tempo_Switch_State(volatile enum Tap_Tempo_Switch_State *tap_tempo_switch_state_ptr){

	static uint8_t extend_rising_edge = 0;

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

		*tap_tempo_switch_state_ptr = DEPRESSED;
	}
	else if(tap_tempo_switch_state_counter == TAP_TEMPO_SWITCH_CONFIDENCE_COUNT){

		*tap_tempo_switch_state_ptr = NOT_DEPRESSED;
	}

	return 1;
}
