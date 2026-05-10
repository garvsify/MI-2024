#ifndef INC_OSCILLATOR_PARAMS_TYPE_H_
#define INC_OSCILLATOR_PARAMS_TYPE_H_

//INCLUDES
#include <stdint.h>

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

	// Phase accumulator fields (replace timer-based speed/symmetry control)
	volatile uint32_t phase_accumulator;    // current phase (0 to 2^32-1 spans one full cycle)
	volatile uint32_t phase_increment;      // base phase step per interrupt, set from speed pot
	volatile uint32_t phase_increment_A;    // step for Group A quadrants (LENGTHEN-when-CW)
	volatile uint32_t phase_increment_B;    // step for Group B quadrants (SHORTEN-when-CW)

	volatile uint16_t duty_delay_line_read_pointer_offset;
};

#endif // - end INC_OSCILLATOR_PARAMS_TYPE_H_
