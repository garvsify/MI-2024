/*NOTES ON THIS BUILD:

Kinda works! Can lock frequencies in range of 1-15Hz, with not so much obvious error seen on the scope, i.e. there being a jump in the index upon sync
because of a inaccurate frequerncy.

Issues:
- Changing frequency on CLK_IN signal generator bricks the oscillator;
- Sometimes a continuous CLK_IN signal causes the frequency set to be the STATE_0 mode value, this is not due to a non-mutex in adc_callback
- Square mode is locking to the 'SINE_OR_TRI' perceived apex indexes

Improvements in the build:
- Making CLK_IN interrupt driven works absolute wonders on the accuracy of the frequency implemented by input capture, DO
NOT BE TEMPTED TO MAKE CLK_IN NOT TRIGGERED BY INTERRUPTS.
- Obvs leave tap tempo done by LPTIM checking

*/

//INCLUDES
#include "main.h"

//const char one_byte_data = 'j';

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

		if(input_capture_processing_can_be_started == YES){

			Input_Capture_Processing(interrupt_period, &params_to_be_loaded);
		}
	}
	return 1;
}



