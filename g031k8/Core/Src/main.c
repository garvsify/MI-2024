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
	//DEBUG//Start_OC_TIM(&htim16, TIM_CHANNEL_1); //start freq. gen.
	while (1)
	{
		//DEBUG//process_TIM16_raw_start_value_and_prescaler();
		//DEBUG//process_TIM16_final_start_value_and_prescaler_adjust();


		HAL_Delay(500);
		uint8_t x = 50;
		if((duty + x) > 1023){
			duty = 0;
		}
		else{
			duty += x;
		}
		__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, duty);
		//htim14.Instance->CCR1 = duty;

	}
	return 1;
}




