#ifndef INC_OSCILLATOR_PARAMS_H_
#define INC_OSCILLATOR_PARAMS_H_

//INCLUDES
#include <stdint.h>

#include "symmetry_adjustment_types.h"

//CUSTOM TYPES

struct Params{

	volatile uint16_t duty;
	volatile uint16_t prev_duty;
	volatile uint16_t duty_delayed;
	volatile uint8_t waveshape;
	volatile uint16_t speed;
	volatile uint8_t depth;
	volatile uint16_t symmetry;
	volatile uint16_t index;
	volatile uint8_t halfcycle;
	volatile uint8_t quadrant;
	volatile uint16_t raw_start_value;
	volatile uint16_t final_start_value;
	volatile uint16_t final_period;
	volatile uint16_t final_ARR;
	volatile uint16_t raw_prescaler;
	volatile uint16_t final_prescaler;
	volatile uint16_t final_prescaler_minus_one;
	volatile enum Adjust_Prescaler_Action prescaler_adjust;
};


struct All_Params_Structs{

	struct Params *running_params;
	struct Params *pot_control_params;
	struct Params *to_be_loaded_params;
	struct Params *working_params;
};

#endif // - end INC_OSCILLATOR_PARAMS_H_
