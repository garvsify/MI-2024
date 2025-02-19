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
volatile extern enum MIDI_CLK_FSM_State MIDI_CLK_FSM_state;

enum MIDI_CLK_FSM_State{

	NOT_COMPILING,
	COMPILING
};

enum Speed_Exclusive_FSM_States{

	TAP_MODE = 4,
	TAP_PENDING_MODE = 5,
	CLK_IN_MODE = 6,
	CLK_IN_PENDING_MODE = 7,
	MIDI_CLK_MODE = 8,
	MIDI_CLK_PENDING_MODE = 9
};

enum Shared_FSM_States{

	MANUAL_MODE = 1,
	CC_MODE = 2,
	PC_MODE = 3,
	NONE = 0 //startup only
};

union Speed_FSM_States{

	enum Speed_Exclusive_FSM_States speed_exclusive_state;
	enum Shared_FSM_States shared_state;
};

enum IP_CAP_FSM_States{

	IDLE,
	MEASUREMENT_PENDING,
	MEASUREMENT_REELAPSE,
	MEASUREMENT_REELAPSE_AND_MEASUREMENT_PENDING,
	IP_CAP_NONE
};

struct Speed_FSM{

	union Speed_FSM_States current_state;
	union Speed_FSM_States prev_state;
};

struct Normal_FSM{

	enum Shared_FSM_States current_state;
	enum Shared_FSM_States prev_state;
};

struct IP_CAP_FSM{

	enum IP_CAP_FSM_States current_state;
	enum IP_CAP_FSM_States prev_state;
};

//FUNCTION DECLARATIONS
uint8_t Advance_Pending_States(void);

#endif /* INC_STATE_MACHINE_H_ */
