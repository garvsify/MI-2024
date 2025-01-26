/*NOTES ON THIS BUILD:

Issues:

- It works apart from speed pot being way too fast

*/

//INCLUDES
#include "system.h"

volatile char rx_buffer[1] = {0};
const char one_byte_data = 'j';

int main(void)
{
	//SYSTEM INIT
	System_Init();

	//STARTUP BITS AND BOBS
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

			//Input_Capture_Processing(interrupt_period, &params_to_be_loaded);
		}
	}
	return 1;
}



