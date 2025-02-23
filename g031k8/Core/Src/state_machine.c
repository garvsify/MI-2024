//INCLUDES
#include <state_machine.h>

//VARIABLE DEFINITIONS
volatile struct Speed_FSM speed_fsm = {.current_state.shared_state = MANUAL_MODE, .prev_state.shared_state = NONE};
volatile struct Normal_FSM depth_fsm = {.current_state = MANUAL_MODE, .prev_state = NONE};
volatile struct Normal_FSM waveshape_fsm = {.current_state = MANUAL_MODE, .prev_state = NONE};
volatile struct Normal_FSM symmetry_fsm = {.current_state = MANUAL_MODE, .prev_state = NONE};
volatile struct Normal_FSM phase_fsm = {.current_state = MANUAL_MODE, .prev_state = NONE};
volatile struct IP_CAP_FSM IP_CAP_fsm = {.current_state = IDLE, .prev_state = IP_CAP_NONE};
volatile enum MIDI_CLK_FSM_State MIDI_CLK_fsm = NOT_COMPILING;

//FUNCTION DEFINITIONS
uint8_t Advance_Pending_States(void){

	//UPDATE SPEED FSM
	union Speed_FSM_States current_state = speed_fsm.current_state;

	if(speed_fsm.current_state.speed_exclusive_state == TAP_PENDING_MODE){

		speed_fsm.current_state.speed_exclusive_state = TAP_MODE;
	}
	else if(speed_fsm.current_state.speed_exclusive_state == CLK_IN_PENDING_MODE){

		speed_fsm.current_state.speed_exclusive_state = CLK_IN_MODE;
	}
	else if(speed_fsm.current_state.speed_exclusive_state == TAP_RESYNC_MODE){

		speed_fsm.current_state.speed_exclusive_state = TAP_MODE;
	}
	else if(speed_fsm.current_state.speed_exclusive_state == CLK_IN_RESYNC_MODE){

		speed_fsm.current_state.speed_exclusive_state = CLK_IN_MODE;
	}

	//DON'T ADD MIDI CLK CODE


	if(!((current_state.speed_exclusive_state == CLK_IN_MODE) || (current_state.speed_exclusive_state == TAP_MODE))){

		speed_fsm.prev_state = current_state;
	}

	return 1;
}

