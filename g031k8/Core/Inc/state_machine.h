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
volatile extern enum MIDI_CLK_FSM_State MIDI_CLK_fsm;
volatile extern struct MIDI_FSM MIDI_fsm;
volatile extern struct MIDI_Running_Status midi_running_status;

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
	MIDI_CLK_PENDING_A0_MODE = 9,
	MIDI_CLK_PENDING_A1_MODE = 10,
	MIDI_CLK_PENDING_B0_MODE = 11,
	MIDI_CLK_PENDING_B1_MODE = 12,
	MIDI_CLK_PENDING_B2_MODE = 13,
	MIDI_CLK_RESYNC_A0_MODE = 14,
	MIDI_CLK_RESYNC_A1_MODE = 15,
	MIDI_CLK_RESYNC_B0_MODE = 16,
	MIDI_CLK_RESYNC_B1_MODE = 17,
	MIDI_CLK_RESYNC_B2_MODE = 18
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

enum MIDI_FSM_States{

	MIDI_NONE, //startup previous only

	MIDI_IDLE, //(dispatch status)

	RECEIVED_CC_STATUS_BYTE,
	RECEIVED_CH_MODE_DATA_BYTE_AFTER_CC_STATUS_BYTE,

	RECEIVED_CONTROLLER_DATA_BYTE_AFTER_CC_STATUS_BYTE,
	RECEIVED_CONTROLLER_VALUE_DATA_BYTE_AFTER_CONTROLLER_DATA_BYTE,

	RECEIVED_PC_STATUS_BYTE,
	RECEIVED_DATA_BYTE_AFTER_PC_STATUS_BYTE,
};

struct MIDI_FSM{

	enum MIDI_FSM_States current_state;
	enum MIDI_FSM_States prev_state;
};

struct MIDI_Running_Status{

	enum Midi_Status_Byte midi_status_byte;
	enum Validate status_byte_received;
};

//FUNCTION DECLARATIONS
uint8_t Advance_Pending_States(void);

#endif /* INC_STATE_MACHINE_H_ */
