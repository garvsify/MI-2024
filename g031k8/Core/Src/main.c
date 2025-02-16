/*NOTES ON THIS BUILD:


Issues:

  - At some CLK IN speed settings, the the manual pot and clk in pot can sometimes control the speed
	even before timeout?

  - Sometimes CLK IN gets bricked by turning on CLK IN - had a look on the rigol scope, apart from a bit of
  bouncing on the pin it seems like its about to sync correctly but then dies?

  - Further testing with the rigol and using the monitor pin shows that the uC never enters the reelapse isr (TIM3)
  so that's really weird???

*/

//INCLUDES
#include "main.h"

//VARIABLE DEFINITIONS
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



