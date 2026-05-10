#ifndef INC_SYMMETRY_ADJUSTMENT_H_
#define INC_SYMMETRY_ADJUSTMENT_H_

//INCLUDES
#include <stdint.h>

#include <oscillator_params_type.h>
#include "symmetry_adjustment_types.h"
#include "oscillator.h"

// Process_Phase_Accumulator_Symmetry_Increments
//
// Computes phase_increment_A and phase_increment_B from params_ptr->phase_increment
// and params_ptr->symmetry.
//
// These two increments are used by Calculate_Next_Main_Oscillator_Values to advance
// the phase accumulator at different rates depending on which quadrant group the
// oscillator is currently in, creating the waveform symmetry / duty-cycle effect.
uint8_t Process_Phase_Accumulator_Symmetry_Increments(struct Params* params_ptr);

#endif /* INC_SYMMETRY_ADJUSTMENT_H_ */
