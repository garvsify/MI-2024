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

	//HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions);
	HAL_ADC_Start_IT(&hadc1);

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
		if((isr_done == YES) && (adc_values_ready == YES)){

			Global_Interrupt_Disable(); //DO NOT DELETE

			HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 1);

			processing_TIM16_final_start_value_and_prescaler = YES;

			Process_TIM16_Raw_Start_Value_and_Raw_Prescaler();
			Process_TIM16_Final_Start_Value_and_Prescaler_Adjust();

			TIM16_final_start_value_locked = TIM16_final_start_value;
			TIM16_prescaler_divisors_final_index_locked = TIM16_prescaler_divisors_final_index;

			processing_TIM16_final_start_value_and_prescaler = NO;

			HAL_GPIO_WritePin(ISR_MEAS_GPIO_Port, ISR_MEAS_Pin, 0);

			isr_done = NO;
			adc_values_ready = NO;

			Global_Interrupt_Enable(); //DO NOT DELETE
		}
	}
	return 1;
}




