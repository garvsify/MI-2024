//INCLUDES
#include "system.h"

int main(void)
{
	System_Init();

	Global_Interrupt_Enable();

	//START ADC TRIG. TIMER
	Start_ADC_Trig_Timer();

	//WAIT
	while(initial_ADC_conversion_complete == 0){}; //wait while first ADC conversion is ongoing

	process_TIM16_raw_start_value_and_prescaler();
	process_TIM16_final_start_value_and_prescaler_adjust();

	//START FREQ. GEN and PWM GEN TIMERS and ENABLE PWM OUTPUT
	Start_PWM_Gen_Timer();
	Start_Freq_Gen_Timer();

	while (1)
	{
		process_TIM16_raw_start_value_and_prescaler();
		process_TIM16_final_start_value_and_prescaler_adjust();
	}
	return 1;
}




