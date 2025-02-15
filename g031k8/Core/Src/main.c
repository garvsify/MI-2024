/*NOTES ON THIS BUILD:

Have found that the reason that we can't get out of tap tempo mode is because even when tap
tempo is actioned, and it visually gets into the mode, the fsm doesn't update from tap pending mode
which is of course why we can't get back into manual mode

*/

//INCLUDES
#include "main.h"

volatile uint32_t statuses = 0; //set of all status bits (to reduce memory usage)

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

			Input_Capture_Processing(interrupt_period, &params_to_be_loaded);
		}
	}
	return 1;
}



