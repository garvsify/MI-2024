/*NOTES ON THIS BUILD:


Issues:

- Found a bug! If MIDI CLKs are received, oscillator syncs internally as it should, then when 'start' is received, the oscillator actually syncs
as it should. However if MIDI CLKs are then received and a start IS NOT sent, the oscillator actually syncs, when it should just internally sync again!

Potential Issues:

- What happens if 'Start' is received and no MIDI CLKs follow it???? Unlikely I know, but we'll have to deal with this rare case!

*/

//INCLUDES
#include "main.h"

//VARIABLE DEFINITIONS
volatile uint32_t statuses = 0; //set of all status bits (to reduce memory usage)
uint32_t idle_counter = 0;
uint32_t tap_tempo_checking_counter = 0;

int main(void)
{
	//SYSTEM INIT
	System_Init();

	//STARTUP
	Startup();

	while (1)
	{
		/*if(UART_DMA_TX_is_complete == YES){

			UART_DMA_TX_is_complete = NO;
			HAL_UART_Transmit_DMA(&huart2, (const uint8_t*)&one_byte_data, sizeof(one_byte_data));
		}*/

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

		if(Get_Status_Bit(&statuses, Software_Tap_Tempo_Checking_Timer_Is_Running) == YES){

			if(tap_tempo_checking_counter < TAP_TEMPO_CHECKING_COUNT){

				tap_tempo_checking_counter++;
			}
			else{

				Clear_Status_Bit(&statuses, Software_Tap_Tempo_Checking_Timer_Is_Running);
				Set_Status_Bit(&statuses, Software_Tap_Tempo_Checking_Timer_Has_Timed_Out);
				Clear_Status_Bit(&statuses, Tap_Tempo_Checking_Disabled);
				tap_tempo_checking_counter = 0;
			}
		}
	}
	return 1;
}



