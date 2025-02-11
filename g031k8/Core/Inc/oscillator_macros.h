#ifndef OSCILLATOR_MACROS_H_
#define OSCILLATOR_MACROS_H_

#define PWM_DUTY_VALUE_MAX 1023
#define PWM_DUTY_VALUE_MIN 0

#define FIRST_QUADRANT_START_INDEX 0
#define SECOND_QUADRANT_START_INDEX 128
#define THIRD_QUADRANT_START_INDEX 256
#define FOURTH_QUADRANT_START_INDEX 384
#define FINAL_INDEX 511
#define TRIANGLE_MODE 0
#define SINE_MODE 1
#define SQUARE_MODE 2
#define NUMBER_OF_FREQUENCY_STEPS 643
#define FIRST_HALFCYCLE 0
#define SECOND_HALFCYCLE 1
#define FIRST_QUADRANT 0
#define SECOND_QUADRANT 1

#define TRIANGLE_MODE_ADC_THRESHOLD 1365 >> 5
#define SINE_MODE_ADC_THRESHOLD 2730 >> 5
#define SQUARE_MODE_ADC_THRESHOLD 4095 >> 5
#define WAVESHAPE_ADC_RESULT_INDEX 0
#define SPEED_ADC_RESULT_INDEX 1
#define DEPTH_ADC_RESULT_INDEX 2
#define SYMMETRY_ADC_RESULT_INDEX 3
#define DUTY_DELAY_LINE_READ_POINTER_OFFSET_ADC_RESULT_INDEX 4

#define SINE_OR_TRIANGLE_WAVE_PREEMPTIVE_DELAY 50
#define SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX 384 - SINE_OR_TRIANGLE_WAVE_PREEMPTIVE_DELAY
#define SQUARE_WAVE_TEMPO_APEX_INDEX 256
#define TEMPO_PULSE_LENGTH 32
#define SINE_OR_TRIANGLE_WAVE_TEMPO_PULSE_OFF_INDEX SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX + TEMPO_PULSE_LENGTH
#define SQUARE_WAVE_TEMPO_PULSE_OFF_INDEX SQUARE_WAVE_TEMPO_APEX_INDEX + TEMPO_PULSE_LENGTH

#define HIGHEST_PRESCALER_TOP_SPEED_PERIOD 129
#define PRESCALER_DIVISORS_MAX_INDEX 5
#define SLOWEST_SPEED_PRESCALER 1024


#if SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX < SECOND_QUADRANT_START_INDEX
	#define CURRENT_QUADRANT_SINE_OR_TRI_SYNCED = FIRST_QUADRANT
	#define CURRENT_HALFCYCLE_SINE_OR_TRI_SYNCED = FIRST_HALFCYCLE

#elif SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX < THIRD_QUADRANT_START_INDEX
	#define CURRENT_QUADRANT_SINE_OR_TRI_SYNCED = SECOND_QUADRANT
	#define CURRENT_HALFCYCLE_SINE_OR_TRI_SYNCED = FIRST_HALFCYCLE

#elif SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX < FOURTH_QUADRANT_START_INDEX
	#define CURRENT_QUADRANT_SINE_OR_TRI_SYNCED FIRST_QUADRANT
	#define CURRENT_HALFCYCLE_SINE_OR_TRI_SYNCED SECOND_HALFCYCLE

#elif SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX <= FINAL_INDEX
	#define CURRENT_QUADRANT_SINE_OR_TRI_SYNCED = SECOND_QUADRANT
	#define CURRENT_HALFCYCLE_SINE_OR_TRI_SYNCED = SECOND_HALFCYCLE
#endif


#if SQUARE_WAVE_TEMPO_APEX_INDEX < SECOND_QUADRANT_START_INDEX
	#define CURRENT_QUADRANT_SQUARE_SYNCED = FIRST_QUADRANT
	#define CURRENT_HALFCYCLE_SQUARE_SYNCED = FIRST_HALFCYCLE

#elif SQUARE_WAVE_TEMPO_APEX_INDEX < THIRD_QUADRANT_START_INDEX
	#define CURRENT_QUADRANT_SQUARE_SYNCED = SECOND_QUADRANT
	#define CURRENT_HALFCYCLE_SQUARE_SYNCED = FIRST_HALFCYCLE

#elif SQUARE_WAVE_TEMPO_APEX_INDEX < FOURTH_QUADRANT_START_INDEX
	#define CURRENT_QUADRANT_SQUARE_SYNCED FIRST_QUADRANT
	#define CURRENT_HALFCYCLE_SQUARE_SYNCED SECOND_HALFCYCLE

#elif SQUARE_WAVE_TEMPO_APEX_INDEX <= FINAL_INDEX
	#define CURRENT_QUADRANT_SQUARE_SYNCED = SECOND_QUADRANT
	#define CURRENT_HALFCYCLE_SQUARE_SYNCED = SECOND_HALFCYCLE

#endif

#define FIRST_SYNC_COMPLETE (0b1 << 3)
#define INITIAL_ADC_CONVERSION_COMPLETE (0b1 << 4)

#endif /* OSCILLATOR_MACROS_H_ */
