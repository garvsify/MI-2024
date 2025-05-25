#ifndef INC_CHECKING_FUNCTIONS_H_
#define INC_CHECKING_FUNCTIONS_H_

//MACROS
//#define LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK 4001 - 1 //4ms overflow with prescaler of 128
#define LPTIM1_CCR_CHECK 251 - 1
#define LPTIM2_COUNT_MAX 24

#define SPEED_POT_CHECK_COUNTER_LENGTH 50
#define SPEED_TOLERANCE 6
#define WAVESHAPE_TOLERANCE 6
#define DEPTH_TOLERANCE 6
#define SYMMETRY_TOLERANCE 6
#define PHASE_TOLERANCE 6

#define IDLE_COUNT 32000
#define MIDI_COUNT 32000 //@TODO

//INCLUDES
#include <stdint.h>

#include "utility.h"
#include "oscillator_params_type.h"
#include "oscillator.h"
#include "timers.h"
#include "tap_tempo.h"
#include "CLK_IN.h"
#include <state_machine.h>

//CUSTOM TYPES
enum Pot_Type{
	WAVESHAPE_POT,
	SPEED_POT,
	DEPTH_POT,
	SYMMETRY_POT,
	PHASE_POT
};

//VARIABLE DECLARATIONS
extern volatile uint8_t waveshape_pot_adc_measurement_num;
extern volatile uint8_t speed_pot_adc_measurement_num;
extern volatile uint8_t depth_pot_adc_measurement_num;
extern volatile uint8_t symmetry_pot_adc_measurement_num;
extern volatile uint8_t phase_pot_adc_measurement_num;

//FUNCTION DECLARATIONS
uint8_t Pot_Check(volatile struct Params* params_ptr, enum Pot_Type pot_type);
uint8_t Check_Tap_Tempo_Switch_State(volatile struct Tap_Tempo_Switch_States *tap_tempo_switch_states_ptr);

#endif /* INC_CHECKING_FUNCTIONS_H_ */
