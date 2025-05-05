/*NOTES ON THIS BUILD:


Issues:

- When the MIDI CLKs are ceased, the device goes back into manual mode when the speed pot isn't wiggled

- PC not working properly

Potential Issues:


*/

//INCLUDES
#include "main.h"
#include "main_vars.h"

int main(void)
{
	//SYSTEM INIT
	System_Init();

	//STARTUP
	Startup();

	while (1)
	{
		if(Get_Status_Bit(&statuses, Input_Capture_Processing_Can_Be_Started) == YES){

			//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

			Input_Capture_Processing(interrupt_period, &params_to_be_loaded);

			//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
		}

		if(Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running) == YES){

			if(idle_counter < IDLE_COUNT){

				if(IP_CAP_fsm.current_state != IDLE){

					Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);
					Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out);
					idle_counter = 0;
				}
				else{

					idle_counter++;
				}
			}
			else{

				Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);
				Set_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out);
				idle_counter = 0;
			}
		}
		if(Get_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running) == YES){

			if(midi_counter < MIDI_COUNT){

				midi_counter++;
			}
			else{

				Clear_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);
				Set_Status_Bit(&statuses, Software_MIDI_Timer_Has_Timed_Out);
				midi_counter = 0;
			}
		}
	}
	return 1;
}



