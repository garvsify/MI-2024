#ifndef INC_SYMMETRY_ADJUSTMENT_TYPES_H_
#define INC_SYMMETRY_ADJUSTMENT_TYPES_H_

// The oscillation-mode, prescaler-adjust, and SHORTEN/LENGTHEN types from the
// timer-overflow implementation have been removed.  Symmetry is now a warp of
// the uniform master phase, described by a pair of Q24 slopes
// (warp_slope_A / warp_slope_B) computed once per ADC update.

enum Symmetry_Status{
	CW,
	CCW
};

#endif /* INC_SYMMETRY_ADJUSTMENT_TYPES_H_ */
