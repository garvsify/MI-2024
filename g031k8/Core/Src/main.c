//INCLUDES
#include "system.h"

int main(void)
{
	System_Init();

	//ENABLE INTERRUPTS
	Global_Interrupt_Enable();

	//START ADC TRIG. TIMER
	Start_ADC_Trig_Timer();

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
		if(TIM16_callback_active == NO){

			if(all_parameters_required_for_next_TIM16_interrupt_calculated == NO){

				HAL_GPIO_TogglePin(SYM_PROC_GPIO_Port, SYM_PROC_Pin);

				Global_Interrupt_Disable(); //DO NOT DELETE

				processing_TIM16_final_start_value_and_prescaler = YES;

				Process_TIM16_Raw_Start_Value_and_Raw_Prescaler();
				Process_TIM16_Final_Start_Value_and_Prescaler_Adjust();

				TIM16_final_start_value_locked = TIM16_final_start_value;
				TIM16_prescaler_adjust_locked = TIM16_prescaler_adjust;

				processing_TIM16_final_start_value_and_prescaler = NO;

				all_parameters_required_for_next_TIM16_interrupt_calculated = YES;

				HAL_GPIO_TogglePin(SYM_PROC_GPIO_Port, SYM_PROC_Pin);

				Global_Interrupt_Enable(); //DO NOT DELETE
			}
		}
	}
	return 1;
}




