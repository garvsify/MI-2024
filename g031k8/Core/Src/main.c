/*NOTES ON THIS BUILD:
Every works basically perfectly until the speed is decreased through half_scale, at which point you can no longer control the speed!

*/

//INCLUDES
#include "system.h"

int main(void)
{
	System_Init();

	//ENABLE INTERRUPTS
	Global_Interrupt_Enable();

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions);

	//WAIT
	while(initial_ADC_conversion_complete == NO){}; //wait while first ADC conversion is ongoing

	//PROCESS RAW AND FINAL FREQ. GEN. TIMER START VALUES AND PRESCALER
	Process_TIM16_Raw_Start_Value_and_Raw_Prescaler();
	Process_TIM16_Final_Start_Value_and_Prescaler_Adjust();

	//START FREQ. GEN and PWM GEN TIMERS and ENABLE PWM OUTPUT
	Start_PWM_Gen_Timer();
	Start_Freq_Gen_Timer();

	while (1)
	{
		//if(TIM16_callback_active == NO){

			//if(all_parameters_required_for_next_TIM16_interrupt_calculated == NO){

				if((adc_conversion_initiated_and_values_ready == YES) && (freq_gen_isr_initiated_and_finished == YES)){

					Global_Interrupt_Disable(); //DO NOT DELETE

					TIM16_final_start_value_exit_locked = TIM16_final_start_value_locked; //store previous locked values
					TIM16_prescaler_divisors_final_index_exit_locked = TIM16_prescaler_divisors_final_index_locked; //store previous locked values

					Global_Interrupt_Enable(); //DO NOT DELETE

					ISR_triggered_whilst_processing = YES;

					HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 1);
					HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 0);
					HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 1);

					Process_TIM16_Raw_Start_Value_and_Raw_Prescaler();
					Process_TIM16_Final_Start_Value_and_Prescaler_Adjust();

					TIM16_final_start_value_locked = TIM16_final_start_value;
					TIM16_prescaler_divisors_final_index_locked = TIM16_prescaler_divisors_final_index;

					adc_conversion_initiated_and_values_ready = NO;
					freq_gen_isr_initiated_and_finished = NO;

					HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 0);
				}
			//}
			//}
		//}
	}
	return 1;
}




