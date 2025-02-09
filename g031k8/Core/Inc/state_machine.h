#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_


//INCLUDES
#include <stdint.h>

//VARIABLES
volatile extern struct Speed_FSM speed_fsm;
volatile extern struct Normal_FSM depth_fsm;
volatile extern struct Normal_FSM waveshape_fsm;
volatile extern struct Normal_FSM symmetry_fsm;
volatile extern struct Normal_FSM phase_fsm;
volatile extern struct IP_CAP_FSM IP_CAP_fsm;

enum Speed_FSM_States{

	SPEED_MANUAL,
	SPEED_CC,
	SPEED_PC,
	SPEED_TAP,
	SPEED_TAP_PENDING,
	SPEED_CLK_IN,
	SPEED_CLK_IN_PENDING,
	SPEED_MIDI_CLK,
	SPEED_MIDI_CLK_PENDING,
	SPEED_NONE //startup only

};

enum Normal_FSM_States{

	NORMAL_MANUAL,
	NORMAL_CC,
	NORMAL_PC,
	NORMAL_NONE //startup only
};

enum IP_CAP_FSM_States{

	IDLE,
	MEASUREMENT_PENDING,
	MEASUREMENT_REELAPSE,
	MEASUREMENT_REELAPSE_AND_MEASUREMENT_PENDING,
	IP_CAP_NONE
};

struct Speed_FSM{

	enum Speed_FSM_States current_state;
	enum Speed_FSM_States prev_state;
};

struct Normal_FSM{

	enum Normal_FSM_States current_state;
	enum Normal_FSM_States prev_state;
};

struct IP_CAP_FSM{

	enum IP_CAP_FSM_States current_state;
	enum IP_CAP_FSM_States prev_state;
};

#endif /* INC_STATE_MACHINE_H_ */
