//INCLUDES
#include <state_machine.h>

//VARIABLE DEFINITIONS
volatile struct Speed_FSM speed_fsm = {.current_state.shared_state = MANUAL_MODE, .prev_state.shared_state = NONE};
volatile struct Normal_FSM depth_fsm = {.current_state = MANUAL_MODE, .prev_state = NONE};
volatile struct Normal_FSM waveshape_fsm = {.current_state = MANUAL_MODE, .prev_state = NONE};
volatile struct Normal_FSM symmetry_fsm = {.current_state = MANUAL_MODE, .prev_state = NONE};
volatile struct Normal_FSM phase_fsm = {.current_state = MANUAL_MODE, .prev_state = NONE};
volatile struct IP_CAP_FSM IP_CAP_fsm = {.current_state = MANUAL_MODE, .prev_state = NONE};
volatile extern enum MIDI_CLK_FSM_State MIDI_CLK_FSM_state = NOT_COMPILING;

