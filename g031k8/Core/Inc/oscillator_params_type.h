#ifndef INC_OSCILLATOR_PARAMS_TYPE_H_
#define INC_OSCILLATOR_PARAMS_TYPE_H_

//INCLUDES
#include <stdint.h>

//CUSTOM TYPES

struct Params{

	volatile uint16_t duty;
	volatile uint16_t prev_duty;
	volatile uint16_t duty_secondary;
	volatile uint8_t waveshape;
	volatile uint16_t speed;
	volatile uint8_t depth;
	volatile uint16_t symmetry;
	volatile uint16_t index;
	volatile uint8_t halfcycle;
	volatile uint8_t quadrant;

	// ── PHASE ENGINE ─────────────────────────────────────────────────────────
	//
	// phase_accumulator is the UNIFORM ("master") phase.  It advances by exactly
	// phase_increment on every TIM16 interrupt and wraps at 2^32, so 2^32 always
	// means one whole LFO cycle no matter what the speed or symmetry pots are
	// doing.  It is therefore a true, linear measure of position within the
	// cycle - which is what makes a constant phase offset possible.
	//
	// The symmetry effect is applied AFTER this, as a warp of the master phase
	// into the shaped output phase (see Symmetry_Warp).  Both the main and the
	// secondary oscillator read through the same warp, so both always carry the
	// same waveshape.
	volatile uint32_t phase_accumulator;    // uniform master phase
	volatile uint32_t phase_increment;      // master step per interrupt, set from speed pot

	// Symmetry warp parameters - recomputed whenever symmetry or waveshape change.
	volatile uint32_t warp_slope_A;         // Q24 gain used inside the Group A segments
	volatile uint32_t warp_slope_B;         // Q24 gain used inside the Group B segment
	volatile uint16_t warp_symmetry;        // symmetry value the slopes were built from (clamped)

	// Phase pot / CC / preset value, 9-bit: 512 == one full cycle == 360 degrees.
	volatile uint16_t phase_control;
};

#endif // - end INC_OSCILLATOR_PARAMS_TYPE_H_
