/*NOTES ON THIS BUILD:

NOOOOOOOO, there is something about puttinf everything in the main loop that is causing increase in frequency at extremes of
symmetry adjustment. ALso in this build, the soeed range is less slow and more fast at its extremes

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

	//START TAP TEMPO SWITCH MONITORING AND UART RECEIVE
	Start_Tap_Tempo_Monitoring_Timers_and_UART_Receive();

	while (1)
	{
		/*if(UART_DMA_TX_is_complete == YES){

			UART_DMA_TX_is_complete = NO;
			HAL_UART_Transmit_DMA(&huart2, (const uint8_t*)&one_byte_data, sizeof(one_byte_data));
		}*/


		if(TIM16_ISR_has_completed == YES){

			Calculate_Next_Duty_Values();

			if(calculation_of_next_duty_values_has_completed == YES){

				HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions); //this function takes ages to execute!

				if(ADC_conversion_has_completed == YES){

					if(speed_pot_is_disabled == NO){

						Process_TIM16_Raw_Start_Value_and_Raw_Prescaler(current_speed, SPEED_ADC_RESOLUTION, NUMBER_OF_FREQUENCY_STEPS, &TIM16_raw_start_value, &TIM16_raw_prescaler);
						Process_TIM16_Final_Start_Value_and_Final_Prescaler(TIM16_raw_start_value, &TIM16_final_start_value, TIM16_raw_prescaler, &TIM16_final_prescaler, current_symmetry, current_waveshape, current_halfcycle, current_quadrant, current_index);
					}

					calculation_of_raw_and_final_values_has_completed = YES;

					if(calculation_of_raw_and_final_values_has_completed == YES){

						if(input_capture_needs_to_be_processed == YES){

							Process_Input_Capture_Value();
						}

						TIM16_ISR_has_completed = NO;
						calculation_of_next_duty_values_has_completed = NO;
						ADC_conversion_has_completed = NO;
						calculation_of_raw_and_final_values_has_completed = NO;
					}
				}
			}
		}
	}
	return 1;
}


