#ifndef INC_CHECKING_FUNCTIONS_H_
#define INC_CHECKING_FUNCTIONS_H_

//MACROS
//#define LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK 4001 - 1 //4ms overflow with prescaler of 128
#define LPTIM1_CCR_CHECK 251 - 1
#define LPTIM2_COUNT_MAX 24

#define POT_COUNTER_COUNT 64

/*#define SPEED_TOLERANCE 10
#define WAVESHAPE_TOLERANCE 1
#define DEPTH_TOLERANCE 2
#define SYMMETRY_TOLERANCE 2
#define PHASE_TOLERANCE 3*/

#define SPEED_TOLERANCE 70
#define WAVESHAPE_TOLERANCE 70
#define DEPTH_TOLERANCE 70
#define SYMMETRY_TOLERANCE 70
#define PHASE_TOLERANCE 70

#define IDLE_COUNT 32000 //roughly 300ms
#define MIDI_COUNT 200000 //roughly 1s
#define PRESET_SAVE_IDLE_COUNT 2000000 //roughly 10s

#define POTS_COUNTER_HAS_TIMED_OUT (0b1 << 11)

#define TAP_TEMPO_SWITCH_PRESET_SAVE_MODE_ADVANCE_COUNT 2500 //with LPTIM1_CCR_CHECK == 250
#define TAP_TEMPO_SWITCH_PRESET_SAVE_COUNT 250

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
extern volatile uint8_t pots_counter;

//FUNCTION DECLARATIONS
uint8_t Pot_Check(volatile uint16_t* ADCResults_arr, enum Pot_Type pot_type);
uint8_t Check_Tap_Tempo_Switch_State(volatile struct Tap_Tempo_Switch_States *tap_tempo_switch_states_ptr);

#endif /* INC_CHECKING_FUNCTIONS_H_ */
