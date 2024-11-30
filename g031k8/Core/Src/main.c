//INCLUDES
#include "system.h"

int main(void)
{
	System_Init();

	Global_Interrupt_Enable();

	//START ADC TRIG. TIMER
	Start_OC_TIM(&htim17, TIM_CHANNEL_1); //start adc trig.

	//WAIT
	while(initial_ADC_conversion_complete == 0){}; //wait while first ADC conversion is ongoing

	process_TIM16_raw_start_value_and_prescaler();
	process_TIM16_final_start_value_and_prescaler_adjust();

	//START FREQ. GEN and PWM GEN TIMERS and ENABLE PWM OUTPUT
	Start_PWM_TIM(&htim14, TIM_CHANNEL_1); //start PWM
	Start_OC_TIM(&htim16, TIM_CHANNEL_1); //start freq. gen.
	while (1)
	{
		process_TIM16_raw_start_value_and_prescaler();
		process_TIM16_final_start_value_and_prescaler_adjust();
	}
	return 1;
}




