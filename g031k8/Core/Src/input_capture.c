#include "input_capture.h"

//VARIABLE DEFINITIONS
volatile uint32_t TIM2_ch1_input_capture_value;
volatile uint16_t interrupt_period = 0;
volatile uint8_t MIDI_CLK_tag = 0;

//FUNCTION DEFINITIONS
// Input_Capture_Processing
//
// Converts the measured input-capture period to a phase_increment value and
// snaps the phase accumulator to the sync point.
//
// Derivation of phase_increment from interrupt_period_value:
//
//   TIM2 counts at  16 MHz / 64 (prescaler) = 250 kHz.
//   interrupt_period = TIM2_capture >> 9  (divides by 512).
//   N = interrupt_period << 6  =  TIM2_capture * 64 / 512
//     = TIM2_capture / 8
//     = (input_period_sec * 250000) / 8
//     = input_period_sec * 16e6 / 512        [TIM16-clock ticks per LFO sample]
//
//   With fixed TIM16 at 16 MHz / 32 / 128 = 3906.25 Hz:
//     fixed_ticks_per_interrupt = 32 * 128 = 4096
//
//   phase_increment = (fixed_ticks / N) * 2^32
//                   = (4096 / N) * 2^32
//                   = 2^44 / N
//                   = 2^35 / interrupt_period_value  (since N = interrupt_period_value << 6)
//                   = (1<<29) / interrupt_period_value  [after dividing numerator and denominator by 64]
//
//   No primality check or integer factorisation loop needed — the phase
//   accumulator handles any period value exactly.
uint8_t Input_Capture_Processing(volatile uint16_t interrupt_period_value, struct Params *params_ptr){

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

	Clear_Status_Bit(&statuses, Input_Capture_Processing_Can_Be_Started); //reset flag

	// Compute phase_increment directly from the measured period.
	// (1ULL<<29) keeps the intermediate 64-bit before the division.
	params_ptr->phase_increment = (uint32_t)((1ULL << 29) / (uint32_t)interrupt_period_value);

	Calculate_Next_Main_Oscillator_Values(params_ptr, (enum Next_Values_Processing_Mode)IP_CAPTURE_MODE);
	Process_Phase_Accumulator_Symmetry_Increments(params_ptr);

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);

	return 1;
}

uint8_t Start_Input_Capture_Timer(void){

	uint8_t ok = Start_IC_TIM(&htim2, TIM_CHANNEL_1);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Start_IC_TIM(TIM_HandleTypeDef *TIM, uint32_t IC_TIM_channel){

	uint8_t ok_AND = HAL_TIM_Base_Start_IT(&htim2);
	ok_AND &= HAL_TIM_IC_Start_IT(&htim2, IC_TIM_channel);

	if(ok_AND != HAL_OK){

		Error_Handler();
	}

	return ok_AND;
}

uint8_t Copy_Params_Structs(struct Params *src_ptr, struct Params *dst_ptr){

	*dst_ptr = *src_ptr;

	return 1;
}

uint8_t Start_Measurement_Reelapse_Timer(void){

	Stop_OC_TIM(&htim3, TIM_CHANNEL_1);
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, interrupt_period);
	Start_OC_TIM(&htim3, TIM_CHANNEL_1);

	return 1;
}

uint8_t Begin_Input_Capture_Measurement(void){

	__HAL_TIM_SET_COUNTER(&htim2, 0); //begin measurement

	return 1;
}

