#ifndef INC_CHECKING_FUNCTIONS_H_
#define INC_CHECKING_FUNCTIONS_H_

//MACROS

//INCLUDES
#include <stdint.h>
#include "utility.h"
#include <oscillator_params_type.h>
#include <speed_pot_check.h>
#include "oscillator.h"
#include "timers.h"
#include "tap_tempo.h"

//CUSTOM TYPES

//VARIABLE DECLARATIONS
volatile extern enum Validate external_clock_mode_is_active;
volatile extern enum Validate tap_tempo_mode_is_active;

//FUNCTION DECLARATIONS
uint8_t Speed_Pot_Check(struct Params* params_ptr);
uint8_t Check_Tap_Tempo_Switch_State(volatile struct Tap_Tempo_Switch_States *tap_tempo_switch_states_ptr);

#endif /* INC_CHECKING_FUNCTIONS_H_ */
