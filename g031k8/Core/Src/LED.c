//INCLUDES
#include "LED.h"

//VARIABLE DEFINITIONS
volatile uint8_t LED_counter = 0;
volatile uint32_t led_blink_period = LED_BLINK_PERIOD;

uint8_t set_LED_to_state(volatile struct LED_FSM *LED_FSM_ptr, enum LED_States desired_state){

	if(desired_state == LED_CONFIRM){

		if(LED_fsm.current_state != LED_CONFIRM){

			__HAL_TIM_SET_COUNTER(&htim14, 0);
			led_blink_period = LED_BLINK_PERIOD >> 1;
			LED_counter = 0;
			__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, led_blink_period);
		}
	}
	else{

		if(LED_FSM_ptr->current_state != desired_state){

			__HAL_TIM_SET_COUNTER(&htim14, 0);
			led_blink_period = LED_BLINK_PERIOD;
			LED_counter = 0;
			__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, led_blink_period);
		}
	}

	LED_FSM_ptr->prev_state = LED_FSM_ptr->current_state;
	LED_FSM_ptr->current_state = desired_state;

	return 1;
}

//FUNCTION DEFINITIONS
