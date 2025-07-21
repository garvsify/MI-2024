#ifndef INC_LED_H_
#define INC_LED_H_

//DEFINES
#define LED_BLINK_PERIOD 300
#define LED_COUNT_OFF_TIME 4

//INCLUDES
#include <stdint.h>
#include "config.h"
#include "state_machine.h"

//CUSTOM TYPES

//VARIABLES
volatile extern uint8_t LED_counter;
volatile extern uint32_t led_blink_period;

//FUNCTION DECLARATIONS
uint8_t Set_LED_to_State(volatile struct LED_FSM *LED_FSM_ptr, enum LED_States desired_state);

#endif /* INC_LED_H_ */
