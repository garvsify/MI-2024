/*NOTES ON THIS BUILD:

Don't know where the bug is yet, but basically have found that when you do two slow taps on the tap tempo switch,
and then in the timer relapse period do two fast taps, the micro shits itself and locks up

*/

//INCLUDES
#include "system.h"

volatile char rx_buffer[1] = {0};

int main(void)
{
	System_Init();

	Startup();

	//START FREQ. GEN and PWM GEN TIMERS and ENABLE PWM OUTPUT
	Start_PWM_Gen_Timer_Main_and_Secondary_Oscillators(&htim1, TIM_CHANNEL_2, TIM_CHANNEL_4);
	Start_Freq_Gen_Timer();
	Start_Input_Capture_Timer();

	//ENABLE INTERRUPTS
	Global_Interrupt_Enable();

	const char one_byte_data = 'j';

	HAL_UART_Receive_DMA(&huart2, (uint8_t*)rx_buffer, sizeof(rx_buffer));

	HAL_LPTIM_SetOnce_Start_IT(&hlptim1, LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK, LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK);

	HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

	while (1)
	{
		Speed_pot_check();

		/*if(UART_DMA_TX_is_complete == YES){

			UART_DMA_TX_is_complete = NO;
			HAL_UART_Transmit_DMA(&huart2, (const uint8_t*)&one_byte_data, sizeof(one_byte_data));
		}*/

		HAL_Delay(50);
	}
	return 1;
}




