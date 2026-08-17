#include "symmetry_adjustment.h"

// ─────────────────────────────────────────────────────────────────────────────
// SYMMETRY AS A PHASE WARP
// ─────────────────────────────────────────────────────────────────────────────
//
// ── Why it is done this way ──────────────────────────────────────────────────
//
// The previous implementation gave the phase accumulator two different
// increments and picked between them according to which quadrant group the
// oscillator was in.  That produces the right waveform, but it destroys the
// accumulator as a measure of position in the cycle: the phase value no longer
// advances at a constant rate, so "half of 2^32" is no longer "half a cycle in
// time".  With nothing left that measures the cycle linearly, there was no way
// to place the secondary oscillator at a constant phase offset.
//
// So the two jobs are now separated:
//
//   phase_accumulator   uniform master phase.  Always advances by exactly
//                       phase_increment.  2^32 == one cycle, at every speed and
//                       every symmetry setting.  This is the linear ruler.
//
//   Symmetry_Warp()     maps that master phase onto the shaped output phase
//                       that actually indexes the wavetable.  All of the
//                       symmetry distortion lives here.
//
// The main oscillator reads Symmetry_Warp(master), the secondary reads
// Symmetry_Warp(master + offset).  Because the offset is applied on the linear
// side of the warp it is a constant fraction of the cycle - a true phase
// offset - and because both taps go through the SAME warp, both oscillators
// always carry exactly the same waveshape.
//
// ── The warp ─────────────────────────────────────────────────────────────────
//
// The output phase space is split into groups that symmetry speeds up or slows
// down:
//
//   SINE / TRIANGLE   Group A = phi in [0, 2^30) and [3*2^30, 2^32)
//                               (indices 0-127 and 384-511 - the rising halves)
//                     Group B = phi in [2^30, 3*2^30)
//                               (indices 128-383 - the falling halves)
//
//   SQUARE            Group A = phi in [0, 2^31)    (indices 0-255, high half)
//                     Group B = phi in [2^31, 2^32) (indices 256-511, low half)
//
// Group A is given a fraction (256 - sym)/256 of the cycle TIME and Group B the
// remaining sym/256.  Those two fractions add to exactly 1, which is why the
// LFO frequency is untouched by the symmetry pot - frequency preservation now
// falls out of the construction instead of relying on a cancellation.
//
// Writing kA = (256 - sym)/128 and kB = sym/128, each segment's master-phase
// width is its time fraction times 2^32, so for SINE / TRIANGLE:
//
//   theta in [0, b1)      ->  phi in [0, 2^30)        b1 = (256-sym) * 2^23
//   theta in [b1, b2)     ->  phi in [2^30, 3*2^30)   b2 = 2^31 + sym * 2^23
//   theta in [b2, 2^32)   ->  phi in [3*2^30, 2^32)
//
// and for SQUARE:
//
//   theta in [0, b1)      ->  phi in [0, 2^31)        b1 = (256-sym) * 2^24
//   theta in [b1, 2^32)   ->  phi in [2^31, 2^32)
//
// Within a segment the map is linear, with only two possible gradients:
//
//   slope_A = 128 / (256 - sym)      slope_B = 128 / sym
//
// both held Q24 in warp_slope_A / warp_slope_B.  A whole warp is therefore two
// compares, a subtract, one multiply and a shift - no division, and nothing
// that needs to run at interrupt rate except the multiply.
//
// Sanity checks:
//   sym = 128 (centre) -> slope_A = slope_B = 1.0 and b1/b2 land exactly on the
//                         quadrant boundaries, so the warp is the identity.
//   sym < 128          -> Group A gets more time than Group B.
//   sym > 128          -> Group B gets more time than Group A.
//
// ── Overflow ─────────────────────────────────────────────────────────────────
// sym is clamped to [SYMMETRY_MIN, SYMMETRY_MAX] = [1, 255], so:
//   slope_A, slope_B  <= 128 << 24 == 2^31            (fits uint32_t)
//   theta * slope     <  2^55                         (fits uint64_t)
// and the result of each segment stays strictly below the next segment's base,
// so the warp never wraps past 2^32.

//FUNCTION DEFINITIONS
uint8_t Process_Symmetry_Warp_Parameters(struct Params* params_ptr){

	#if SYMMETRY_ON_OR_OFF == ON

		uint32_t sym = (uint32_t)params_ptr->symmetry;

		// Both ends are degenerate - they ask one group to be crossed in zero
		// time - so hold the value one step inside each extreme.
		if(sym < SYMMETRY_MIN){
			sym = SYMMETRY_MIN;
		}
		else if(sym > SYMMETRY_MAX){
			sym = SYMMETRY_MAX;
		}

		params_ptr->warp_symmetry = (uint16_t)sym;
		params_ptr->warp_slope_A  = (128UL << WARP_SLOPE_FRAC_BITS) / (256UL - sym);
		params_ptr->warp_slope_B  = (128UL << WARP_SLOPE_FRAC_BITS) / sym;

	#else

		// Symmetry disabled: make the warp the identity map.
		params_ptr->warp_symmetry = SYMMETRY_ADC_HALF_SCALE;
		params_ptr->warp_slope_A  = (1UL << WARP_SLOPE_FRAC_BITS);
		params_ptr->warp_slope_B  = (1UL << WARP_SLOPE_FRAC_BITS);

	#endif

	return 1;
}

uint32_t Symmetry_Warp(uint32_t master_phase, const struct Params* params_ptr){

	#if SYMMETRY_ON_OR_OFF == ON

		uint32_t sym  = (uint32_t)params_ptr->warp_symmetry;
		uint32_t anti = 256UL - sym;

		if(params_ptr->waveshape == SQUARE_MODE){

			uint32_t b1 = anti << 24; //master phase at the halfcycle boundary

			if(master_phase < b1){

				return (uint32_t)(((uint64_t)master_phase * params_ptr->warp_slope_A) >> WARP_SLOPE_FRAC_BITS);
			}

			return 0x80000000UL + (uint32_t)(((uint64_t)(master_phase - b1) * params_ptr->warp_slope_B) >> WARP_SLOPE_FRAC_BITS);
		}
		else{

			uint32_t b1 = anti << 23;                   //master phase at the end of the 1st quadrant
			uint32_t b2 = 0x80000000UL + (sym << 23);   //master phase at the end of the 3rd quadrant

			if(master_phase < b1){

				return (uint32_t)(((uint64_t)master_phase * params_ptr->warp_slope_A) >> WARP_SLOPE_FRAC_BITS);
			}

			if(master_phase < b2){

				return 0x40000000UL + (uint32_t)(((uint64_t)(master_phase - b1) * params_ptr->warp_slope_B) >> WARP_SLOPE_FRAC_BITS);
			}

			return 0xC0000000UL + (uint32_t)(((uint64_t)(master_phase - b2) * params_ptr->warp_slope_A) >> WARP_SLOPE_FRAC_BITS);
		}

	#else

		(void)params_ptr;
		return master_phase;

	#endif
}

uint32_t Inverse_Symmetry_Warp(uint32_t shaped_phase, const struct Params* params_ptr){

	#if SYMMETRY_ON_OR_OFF == ON

		// The inverse gradients are exact integers scaled by 1/128, so this
		// direction needs no fixed-point reciprocal at all.
		uint32_t sym  = (uint32_t)params_ptr->warp_symmetry;
		uint32_t anti = 256UL - sym;

		if(params_ptr->waveshape == SQUARE_MODE){

			if(shaped_phase < 0x80000000UL){

				return (uint32_t)(((uint64_t)shaped_phase * anti) >> 7);
			}

			return (anti << 24) + (uint32_t)(((uint64_t)(shaped_phase - 0x80000000UL) * sym) >> 7);
		}
		else{

			if(shaped_phase < 0x40000000UL){

				return (uint32_t)(((uint64_t)shaped_phase * anti) >> 7);
			}

			if(shaped_phase < 0xC0000000UL){

				return (anti << 23) + (uint32_t)(((uint64_t)(shaped_phase - 0x40000000UL) * sym) >> 7);
			}

			return (0x80000000UL + (sym << 23)) + (uint32_t)(((uint64_t)(shaped_phase - 0xC0000000UL) * anti) >> 7);
		}

	#else

		(void)params_ptr;
		return shaped_phase;

	#endif
}
