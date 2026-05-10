#ifndef INC_SYMMETRY_ADJUSTMENT_TYPES_H_
#define INC_SYMMETRY_ADJUSTMENT_TYPES_H_

// The oscillation-mode, prescaler-adjust, and SHORTEN/LENGTHEN types from the
// timer-overflow implementation have been removed.  The phase accumulator
// approach handles all of these cases through a single pair of increments
// (phase_increment_A / phase_increment_B) computed once per ADC update.

enum Symmetry_Status{
	CW,
	CCW
};

#endif /* INC_SYMMETRY_ADJUSTMENT_TYPES_H_ */
