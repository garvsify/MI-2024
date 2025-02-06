#ifndef INC_TAP_TEMPO_H_
#define INC_TAP_TEMPO_H_

//MACROS
#define TAP_TEMPO_SWITCH_CONFIDENCE_COUNT 7
#define COUNT_TO_DELAY_RISING_TAP_TEMPO_EDGE 35

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
