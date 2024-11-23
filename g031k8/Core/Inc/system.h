#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

//INCLUDES
#include "config.h"
#include <stdint.h>

//MACROS
#define MAX_QUADRANT_INDEX 128
#define MIN_QUADRANT_INDEX 0
#define TRIANGLE_MODE 0
#define SINE_MODE 1
#define SQUARE_MODE 2
#define NUMBER_OF_FREQUENCY_STEPS 600; //883 in calcs, seems to be wrong, but 650 gives 15Hz max freq.
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
#define ON 1
#define OFF 0

		//TURN ON/OFF SYMMETRY and SET RESOLUTION
		#define SYMMETRY_ADC_RESOLUTION 8
		#define SYMMETRY_ON_OR_OFF OFF

		#if SYMMETRY_ADC_RESOLUTION == 12
			#define SYMMETRY_ADC_HALF_SCALE_NO_BITS 11
			#define SYMMETRY_ADC_FULL_SCALE 4095
			#define SYMMETRY_ADC_HALF_SCALE 2047
		#endif

		#if SYMMETRY_ADC_RESOLUTION == 8
			#define SYMMETRY_ADC_HALF_SCALE_NO_BITS 7
			#define SYMMETRY_ADC_FULL_SCALE 255
			#define SYMMETRY_ADC_HALF_SCALE 128
		#endif

		//TURN ON/OFF DEPTH
		#define DEPTH_ON_OR_OFF OFF

//CONSTANTS
extern const uint16_t sine_table_one_quadrant[129];
extern const uint16_t tri_table_one_quadrant[129];
extern const uint16_t TIM16_prescaler_divisors[9];

//VARIABLES
volatile extern uint32_t final_TMR0;
volatile extern uint8_t TMR0_prescaler_adjust;
volatile extern uint32_t raw_TMR0;
volatile extern uint8_t base_prescaler_bits_index;
volatile extern uint8_t symmetry_status;
volatile extern uint16_t speed_control;
volatile extern uint32_t speed_control_32;
volatile extern uint8_t how_many_128;
volatile extern uint16_t duty;
volatile extern uint8_t duty_low_byte;
volatile extern uint8_t duty_high_byte;
volatile extern uint8_t current_waveshape;
volatile extern uint16_t current_speed_linear;
volatile extern uint32_t current_speed_linear_32;
volatile extern uint16_t current_depth;
volatile extern uint32_t current_symmetry;
volatile extern uint8_t current_one_quadrant_index;
volatile extern uint8_t current_halfcycle;
volatile extern uint8_t current_quadrant;
volatile extern uint8_t prescaler_overflow_flag;
volatile extern uint8_t prescaler_final_index;
volatile extern uint8_t ADC_type_flag;
volatile extern uint8_t dutyL;
volatile extern uint8_t dutyH;
volatile extern uint8_t current_depthL;

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
uint8_t process_TMR0_raw_and_prescaler(void);
uint8_t turn_TMR0_prescaler_OFF(void);
uint8_t turn_TMR0_prescaler_ON(void);
uint8_t adjust_TMR0(void);
uint8_t adjust_and_set_TMR0_prescaler(void);
uint8_t shorten_period(void);
uint8_t lengthen_period(void);
uint8_t process_TMR0_final_and_prescaler_adjust(void);

#endif
