#ifndef INC_CHECKING_FUNCTIONS_H_
#define INC_CHECKING_FUNCTIONS_H_

//MACROS
//#define LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK 4001 - 1 //4ms overflow with prescaler of 128
#define LPTIM1_CCR_CHECK 251 - 1
#define LPTIM2_COUNT_MAX 24
#define TIM14_OVERFLOW_LENGTH 2050 // 4096/2 is min osc frequency period

//INCLUDES
#include <stdint.h>

#include "utility.h"
#include "oscillator_params_type.h"
#include "speed_pot_check.h"
#include "oscillator.h"
#include "timers.h"
#include "tap_tempo.h"
#include "CLK_IN.h"
#include "speed_control_state_machine.h"

//CUSTOM TYPES

//VARIABLE DECLARATIONS

//FUNCTION DECLARATIONS
uint8_t Speed_Pot_Check(struct Params* params_ptr);
uint8_t Check_Tap_Tempo_Switch_State(volatile struct Tap_Tempo_Switch_States *tap_tempo_switch_states_ptr);
uint8_t Start_Timeout_Timer(void);
uint8_t Reset_Timeout_Timer(void);
uint8_t Stop_Timeout_Timer(void);

#endif /* INC_CHECKING_FUNCTIONS_H_ */
