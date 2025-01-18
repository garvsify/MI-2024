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

	HAL_Delay(100);

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

		if(TIM16_callback_finished == YES){

			Calculate_Next_Main_Oscillator_Values(&params, (enum Next_Values_Processing_Mode)REGULAR_MODE);
			Write_Next_Main_Oscillator_Values_to_Delay_Line(&params, &delay_line);
			HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions); //this function takes ages to execute!
			TIM16_callback_finished = NO;
		}
		else if(input_capture_processing_can_be_started == YES){

			Input_Capture_Processing(interrupt_period, &params_to_be_loaded);
		}
	}
	return 1;
}



