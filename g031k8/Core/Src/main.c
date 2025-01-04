/*NOTES ON THIS BUILD:

*/

//INCLUDES
#include "system.h"

int main(void)
{
	System_Init();

	Startup();

	//START FREQ. GEN and PWM GEN TIMERS and ENABLE PWM OUTPUT
	Start_PWM_Gen_Timer_Main_Oscillator();
	Start_PWM_Gen_Timer_Secondary_Oscillator();
	Start_Freq_Gen_Timer();
	Start_Input_Capture_Timer();

	//ENABLE INTERRUPTS
	Global_Interrupt_Enable();

	const char one_byte_data = 'p';

	while (1)
	{
		Speed_pot_check();

		if(UART_DMA_TX_is_complete == YES){

			UART_DMA_TX_is_complete = NO;
			HAL_UART_Transmit_DMA(&huart2, (const uint8_t*)&one_byte_data, sizeof(one_byte_data));
		}

		//HAL_Delay(300);
	}
	return 1;
}




