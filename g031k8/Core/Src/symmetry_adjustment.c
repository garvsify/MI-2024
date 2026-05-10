#include "symmetry_adjustment.h"

// Process_Phase_Accumulator_Symmetry_Increments
//
// Computes phase_increment_A and phase_increment_B such that the overall LFO
// frequency is EXACTLY preserved regardless of the symmetry pot position.
//
// ── Why the previous additive formula was wrong ───────────────────────────────
//
// In the original timer-overflow design each quadrant group always had exactly
// 256 samples, so:
//
//   T_total = 256 × (base_period + delta) + 256 × (base_period - delta)
//           = 512 × base_period      ← delta cancels, frequency unchanged ✓
//
// In the phase accumulator the time spent in each group is INVERSELY
// proportional to its increment (T = 0x80000000 / inc), so the additive
// approach expands the total period by 1/(1 - (delta/base)²) — which makes
// the LFO run slightly slower whenever symmetry is applied.
//
// ── The correct formula: harmonic mean constraint ─────────────────────────────
//
// Each group spans exactly half the 32-bit phase space (0x80000000).
// For T_total = T_nominal = 2^32 / base_increment we need:
//
//   T_A + T_B = 0x80000000/inc_A + 0x80000000/inc_B = 0x100000000/base
//   →  1/inc_A + 1/inc_B = 2/base          (harmonic mean condition)
//
// Let f_A = fraction of the cycle spent in Group A, f_B = 1 - f_A.
//   T_A = T_nominal × f_A  →  inc_A = base / (2 × f_A) = base × 128 / (256 × f_A)
//
// Mapping the symmetry ADC value to f_A (128 = centre = f_A 0.5):
//   f_A = (256 - symmetry) / 256
//
//   → inc_A = base × 128 / (256 - symmetry)
//     inc_B = base × 128 / symmetry
//
// Frequency preservation check:
//   T_A + T_B = 0x80000000 × (256-sym + sym) / (128 × base)
//             = 0x80000000 × 256 / (128 × base)
//             = 0x100000000 / base  ✓   (exact for every symmetry value)
//
// ── Quadrant group assignment ─────────────────────────────────────────────────
//
// The 32-bit phase accumulator's top two bits encode the quadrant:
//
//   bits [31:30] = 00  →  indices   0-127  (Q=FIRST,  HC=FIRST)   \  Group A
//   bits [31:30] = 11  →  indices 384-511  (Q=SECOND, HC=SECOND)  /
//   bits [31:30] = 01  →  indices 128-255  (Q=SECOND, HC=FIRST)   \  Group B
//   bits [31:30] = 10  →  indices 256-383  (Q=FIRST,  HC=SECOND)  /
//
// For SQUARE mode: Group A = first halfcycle (bit31=0), Group B = second.
//
// Convention (matching original code): Group A is SLOWER when pot is CW.
//   CW (sym < 128):  anti_sym > sym  →  inc_A < inc_B  →  Group A slower  ✓
//   CCW (sym > 128): anti_sym < sym  →  inc_A > inc_B  →  Group A faster  ✓
//   Centre (sym=128): inc_A = inc_B = base                              ✓
//
// ── Overflow note ─────────────────────────────────────────────────────────────
// base × 128: max base ≈ 4.23 M, × 128 ≈ 541 M < 2^32 — fits in uint32_t.
// Guard: sym=0 (pot fully CW) would cause division by zero for inc_B;
// the ADC produces even values (0, 2, 4 … 254) so clamp to 1 as the floor.

uint8_t Process_Phase_Accumulator_Symmetry_Increments(struct Params* params_ptr){

    #if SYMMETRY_ON_OR_OFF == ON

        // Clamp symmetry away from 0 to avoid division by zero on inc_B.
        uint32_t sym      = (params_ptr->symmetry == 0U) ? 1UL : (uint32_t)params_ptr->symmetry;
        uint32_t anti_sym = 256UL - sym;

        uint32_t base_x128 = params_ptr->phase_increment * 128UL;

        params_ptr->phase_increment_A = base_x128 / anti_sym; // slower when sym < 128 (CW)
        params_ptr->phase_increment_B = base_x128 / sym;      // faster when sym < 128 (CW)

    #endif

    #if SYMMETRY_ON_OR_OFF == OFF

        params_ptr->phase_increment_A = params_ptr->phase_increment;
        params_ptr->phase_increment_B = params_ptr->phase_increment;

    #endif

    return 1;
}
