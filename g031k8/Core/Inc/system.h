#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

//INCLUDES
#include "config.h"
#include <stdint.h>

//MACROS
#define FIRST_QUADRANT_START_INDEX 0
#define SECOND_QUADRANT_START_INDEX 128
#define THIRD_QUADRANT_START_INDEX 256
#define FOURTH_QUADRANT_START_INDEX 384
#define FINAL_INDEX 511
#define TRIANGLE_MODE 0
#define SINE_MODE 1
#define SQUARE_MODE 2
#define NUMBER_OF_FREQUENCY_STEPS 700; //883 in calcs, seems to be wrong, but 650 gives 15Hz max freq.
#define FIRST_HALFCYCLE 0
#define SECOND_HALFCYCLE 1
#define FIRST_QUADRANT 0
#define SECOND_QUADRANT 1
#define TRIANGLE_MODE_ADC_THRESHOLD 1365
#define SINE_MODE_ADC_THRESHOLD 2730
#define SQUARE_MODE_ADC_THRESHOLD 4095
#define CW 1
#define CCW 0
#define MAX_SYMMETRY_TOTAL 361
#define SHORTEN_PERIOD_FRACTION_16_BIT_NUMBER 47926
#define LENGTHEN_PERIOD_FRACTION_16_BIT_NUMBER 17609
#define RESOLUTION_OF_TOTAL_SYMMETRY_FRACTION 16
#define SHORTEN_POWER_OF_TWO_CONSTANT_8_BIT_SYM 1024
#define SHORTEN_POWER_OF_TWO_DIVISOR_8_BIT_SYM 12
#define SHORTEN_POWER_OF_TWO_CONSTANT_10_BIT_SYM 4096
#define SHORTEN_POWER_OF_TWO_DIVISOR_10_BIT_SYM 14
#define LENGTHEN_CONSTANT_8_BIT_SYM 896
#define LENGTHEN_POWER_OF_TWO_DIVISOR_8_BIT_SYM 9
#define LENGTHEN_CONSTANT_10_BIT_SYM 3584
#define LENGTHEN_POWER_OF_TWO_DIVISOR_10_BIT_SYM 11
#define TWELVEBITMINUSONE 4095
#define TENBITMINUSONE 1023
#define EIGHTBITMINUSONE 255
#define HALFCYCLE_WINDOW 2
#define DELAY 0
#define DELAY_2 0
#define ON 1
#define OFF 0

		//TURN ON/OFF SYMMETRY and SET RESOLUTION
		//set SYMMETRY_ADC_RESOLUTION to either 8, 10, or 12. 8 and 10 use the old equations, 12 uses the new equations.
		#define SYMMETRY_ADC_RESOLUTION 12
		#define SYMMETRY_ON_OR_OFF ON

		#if SYMMETRY_ADC_RESOLUTION == 10
			#define SYMMETRY_ADC_HALF_SCALE_NO_BITS 9
			#define SYMMETRY_ADC_FULL_SCALE 1023
			#define SYMMETRY_ADC_HALF_SCALE 511
		#endif

		#if SYMMETRY_ADC_RESOLUTION == 8
			#define SYMMETRY_ADC_HALF_SCALE_NO_BITS 7
			#define SYMMETRY_ADC_FULL_SCALE 255
			#define SYMMETRY_ADC_HALF_SCALE 128
		#endif

		#if SYMMETRY_ADC_RESOLUTION == 12
			#define SYMMETRY_ADC_FULL_SCALE 4095
			#define SYMMETRY_ADC_HALF_SCALE_NO_BITS 11
			#define SYMMETRY_ADC_HALF_SCALE 2048
		#endif

		//TURN ON/OFF DEPTH
		#define DEPTH_ON_OR_OFF ON

//CONSTANTS
extern const uint16_t sine_wavetable[512];
extern const uint16_t tri_wavetable[512];
extern const uint16_t TIM16_prescaler_divisors[9];

//VARIABLES
volatile extern uint8_t TIM16_prescaler_adjust;
volatile extern uint32_t TIM16_raw_start_value;
volatile extern uint32_t TIM16_final_start_value;
volatile extern uint8_t TIM16_base_prescaler_divisors_index;
volatile extern uint16_t duty;
volatile extern uint8_t current_waveshape;
volatile extern uint16_t current_speed_linear;
volatile extern uint32_t current_speed_linear_32;
volatile extern uint16_t current_depth;
volatile extern uint32_t current_symmetry;
volatile extern uint16_t current_index;
volatile extern uint8_t current_halfcycle;
volatile extern uint8_t current_quadrant;
volatile extern uint8_t TIM16_prescaler_overflow_flag;
volatile extern uint16_t ADCResultsDMA[4];
const extern uint8_t num_ADC_conversions;
volatile extern uint8_t initial_ADC_conversion_complete;
volatile extern uint8_t processing_TIM16_final_start_value_and_prescaler;
volatile extern uint8_t TIM16_callback_active;
volatile extern uint32_t exit_TIM16_final_start_value_locked;
volatile extern uint8_t exit_TIM16_prescaler_adjust_locked;
volatile extern uint8_t halfcycle_has_changed;
volatile extern uint8_t halfcycle_is_about_to_change;
volatile extern uint32_t TIM16_final_start_value_locked;
volatile extern uint8_t TIM16_prescaler_adjust_locked;
volatile extern uint16_t prev_duty;
volatile extern uint8_t values_locked;

//CUSTOM TYPES
enum Polarity{
	NEGATIVE,
	POSITIVE
};

enum Adjust_Prescaler_Action{
	DO_NOTHING,
	DIVIDE_BY_TWO,
	MULTIPLY_BY_TWO,
	DIVIDE_BY_FOUR,
	DONT_CARE
};

enum Validate{
	NO,
	YES
};

//FUNCTION DECLARATIONS
uint8_t Global_Interrupt_Enable(void);
uint8_t Global_Interrupt_Disable(void);
uint8_t Start_ADC_Trig_Timer(void);
uint8_t Start_PWM_Gen_Timer(void);
uint8_t Start_Freq_Gen_Timer(void);
uint8_t Start_PWM_TIM(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel);
uint8_t Start_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel);
uint8_t Stop_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel);
uint8_t Process_TIM16_Raw_Start_Value_and_Raw_Prescaler(void);
uint8_t Process_TIM16_Final_Start_Value_and_Prescaler_Adjust(void);
uint8_t Adjust_and_Set_TIM16_Prescaler(uint8_t TIM16_prescaler_adjust_arg);
uint8_t Process_TIM16_Final_Start_Value_and_Prescaler_Adjust_Slow_Speeds(void);
uint8_t Shorten_Period(void);
uint8_t Lengthen_Period(void);

#endif
