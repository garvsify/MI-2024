#ifndef INC_TAP_TEMPO_H_
#define INC_TAP_TEMPO_H_

//MACROS
#define TAP_TEMPO_SWITCH_CONFIDENCE_COUNT 14
#define COUNT_TO_DELAY_RISING_TAP_TEMPO_EDGE 4

#define SOFTWARE_TAP_TEMPO_CHECKING_TIMER_HAS_TIMED_OUT (1 << 8)
#define SOFTWARE_TAP_TEMPO_CHECKING_TIMER_IS_RUNNING (1 << 9)
#define TAP_TEMPO_CHECKING_DISABLED (1 << 10)

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
