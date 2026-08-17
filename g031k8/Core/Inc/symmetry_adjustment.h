#ifndef INC_SYMMETRY_ADJUSTMENT_H_
#define INC_SYMMETRY_ADJUSTMENT_H_

//INCLUDES
#include <stdint.h>

#include <oscillator_params_type.h>
#include "symmetry_adjustment_types.h"
#include "oscillator.h"

// Process_Symmetry_Warp_Parameters
//
// Builds warp_slope_A / warp_slope_B / warp_symmetry in the params struct from
// params_ptr->symmetry.  Call it whenever the symmetry value may have changed.
// It does two divisions, so it lives in the ADC callback rather than in the
// oscillator interrupt.
uint8_t Process_Symmetry_Warp_Parameters(struct Params* params_ptr);

// Symmetry_Warp
//
// Maps the uniform master phase onto the shaped output phase.  This is where
// the symmetry effect actually happens - see the block comment in
// symmetry_adjustment.c for the derivation.  Multiplies and shifts only, so it
// is cheap enough to run twice per oscillator interrupt (main + secondary).
uint32_t Symmetry_Warp(uint32_t master_phase, const struct Params* params_ptr);

// Inverse_Symmetry_Warp
//
// The inverse of Symmetry_Warp: given a shaped output phase you want to be
// sitting at, returns the master phase that produces it.  Used when the
// oscillator is snapped to a sync point.
uint32_t Inverse_Symmetry_Warp(uint32_t shaped_phase, const struct Params* params_ptr);

#endif /* INC_SYMMETRY_ADJUSTMENT_H_ */
