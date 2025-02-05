/*NOTES ON THIS BUILD:

Kinda works! Can lock frequencies in range of 1-15Hz, with not so much obvious error seen on the scope, i.e. there being a jump in the index upon sync
because of a inaccurate frequerncy.

Issues:
- Square mode is locking to the 'SINE_OR_TRI' perceived apex indexes
-Tap tempo has stopped working?

Improvements in the build:
- Have made all the status bits part of 1 uint32_t and all flags are just specific bits of that integer :)
- Making CLK_IN interrupt driven works absolute wonders on the accuracy of the frequency implemented by input capture, DO
NOT BE TEMPTED TO MAKE CLK_IN NOT TRIGGERED BY INTERRUPTS.
- Obvs leave tap tempo done by LPTIM checking

*/

//INCLUDES
#include "main.h"

//const char one_byte_data = 'j';

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



