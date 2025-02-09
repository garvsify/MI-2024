//INCLUDES
#include <state_machine.h>

//VARIABLE DEFINITIONS
volatile struct Speed_FSM speed_fsm = {.current_state = SPEED_MANUAL, .prev_state = SPEED_NONE};
volatile struct Normal_FSM depth_fsm = {.current_state = NORMAL_MANUAL, .prev_state = NORMAL_NONE};
volatile struct Normal_FSM waveshape_fsm = {.current_state = NORMAL_MANUAL, .prev_state = NORMAL_NONE};
volatile struct Normal_FSM symmetry_fsm = {.current_state = NORMAL_MANUAL, .prev_state = NORMAL_NONE};
volatile struct Normal_FSM phase_fsm = {.current_state = NORMAL_MANUAL, .prev_state = NORMAL_NONE};
volatile extern struct IP_CAP_FSM IP_CAP_fsm = {.current_state = IDLE, .prev_state = IP_CAP_NONE};

