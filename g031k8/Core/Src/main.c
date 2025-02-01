/*NOTES ON THIS BUILD:

Kinda works! Can lock frequencies in range of 1-15Hz, with not so much obvious error seen on the scope, i.e. there being a jump in the index upon sync
because of a inaccurate frequerncy.

Issues:
- Changing frequency on CLK_IN signal generator can brick the oscillator;
- Sometimes a continuous CLK_IN signal causes the frequency set to be the 'pot control' mode values, as if it changes the mode for some reason into
the pot control mode, before then realising its receiving clocks again and goes back to working for a bit
- Square mode is locking to the 'SINE_OR_TRI' perceived apex indexes


Solutions:
- I think we can fix issue 2 by creating another Params struct for 'Running Values', with and another for 'Pot Control Values' or summat

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

	//START FREQ. GEN and PWM GEN TIMERS and ENABLE PWM OUTPUT
	Start_PWM_Gen_Timer_Main_and_Secondary_Oscillators(&htim1, TIM_CHANNEL_2, TIM_CHANNEL_4);
	Start_Freq_Gen_Timer();
	Start_Input_Capture_Timer();

	//ENABLE INTERRUPTS
	Global_Interrupt_Enable();

	//START UART RECEIVE
	Start_UART_Receive();

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



