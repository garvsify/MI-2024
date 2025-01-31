#ifndef INC_TAP_TEMPO_H_
#define INC_TAP_TEMPO_H_

//MACROS
//#define LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK 4001 - 1 //4ms overflow with prescaler of 128
#define LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK 126 - 1 //0.25ms overflow with prescaler of 128
#define TAP_TEMPO_SWITCH_CONFIDENCE_COUNT 5
#define COUNT_TO_DELAY_RISING_TAP_TEMPO_EDGE 5

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
