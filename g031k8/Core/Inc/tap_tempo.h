#ifndef INC_TAP_TEMPO_H_
#define INC_TAP_TEMPO_H_

//MACROS
#define TAP_TEMPO_SWITCH_CONFIDENCE_COUNT 14
#define COUNT_TO_DELAY_RISING_TAP_TEMPO_EDGE 4

#define TAP_TEMPO_ADVANCE_IDLE_TIMER_IS_RUNNING (0b1 << 12)
#define TAP_TEMPO_ADVANCE_IDLE_TIMER_HAS_TIMED_OUT (0b1 << 13)
#define TAP_TEMPO_PRESET_SAVE_TIMER_IS_RUNNING (0b1 << 14)
#define TAP_TEMPO_PRESET_SAVE_TIMER_HAS_TIMED_OUT (0b1 << 15)
#define ADVANCE_IDLE_COUNT 200000 //roughly 1s

//CUSTOM TYPES
enum Tap_Tempo_Switch_State{
	NOT_DEPRESSED,
	DEPRESSED
};

struct Tap_Tempo_Switch_States{

	enum Tap_Tempo_Switch_State tap_tempo_switch_state;
	enum Tap_Tempo_Switch_State tap_tempo_switch_prev_state;
};

#endif /* INC_TAP_TEMPO_H_ */
