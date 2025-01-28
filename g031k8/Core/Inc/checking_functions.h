#ifndef INC_CHECKING_FUNCTIONS_H_
#define INC_CHECKING_FUNCTIONS_H_

//MACROS
//#define LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK 4001 - 1 //4ms overflow with prescaler of 128
#define LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK 1001 - 1 //2ms overflow with prescaler of 128
#define TAP_TEMPO_SWITCH_CONFIDENCE_COUNT 5
#define COUNT_TO_DELAY_RISING_TAP_TEMPO_EDGE 1
#define SPEED_POT_CHECK_COUNTER_LENGTH 50
#define SPEED_TOLERANCE 3

//INCLUDES
#include <stdint.h>
#include "utility.h"
#include <oscillator_params_type.h>
#include "oscillator.h"
#include "timers.h"

//CUSTOM TYPES
enum Tap_Tempo_Switch_State{
	NOT_DEPRESSED,
	DEPRESSED,
	INTERMEDIATE
};

//VARIABLE DECLARATIONS
volatile extern uint8_t tap_tempo_switch_state_counter;
volatile extern enum Tap_Tempo_Switch_State tap_tempo_switch_state;
volatile extern enum Validate external_clock_mode_is_active;
volatile extern enum Validate tap_tempo_mode_is_active;
volatile extern uint8_t speed_pot_adc_measurement_num;

//FUNCTION DECLARATIONS
uint8_t Speed_Pot_Check(struct Params* params_ptr);
uint8_t Check_Tap_Tempo_Switch_State(volatile enum Tap_Tempo_Switch_State *tap_tempo_switch_state_ptr);

#endif /* INC_CHECKING_FUNCTIONS_H_ */
