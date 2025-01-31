#ifndef INC_SPEED_CONTROL_STATE_MACHINE_H_
#define INC_SPEED_CONTROL_STATE_MACHINE_H_


//INCLUDES
#include <stdint.h>

//VARIABLES
volatile extern enum Speed_State_Machine_State state;

enum Speed_State_Machine_State{

	//pot control mode - tap tempo control, external clock control, midi clock control off
	STATE_0,
	//tap tempo control mode - pot control, external clock control, midi clock control off
	STATE_1,
	//external clock control mode - pot control, tap tempo control, midi clock control off
	STATE_2,
	//midi clock control mode - pot control, external clock control, tap tempo control off
	STATE_3
};

#endif /* INC_SPEED_CONTROL_STATE_MACHINE_H_ */
