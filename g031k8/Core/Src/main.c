//INCLUDES
#include "system.h"

int main(void)
{
	__enable_irq(); //enable interrupts
	System_Init();

	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1); //start PWM

	//HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

	/*//DISABLE TIMERS and PWM OUTPUT
	__HAL_TIM_DISABLE(&htim14); //disable TIM14
	__HAL_TIM_DISABLE(&htim16); //disable TIM16
	__HAL_TIM_DISABLE(&htim17); //disable TIM17
	__HAL_TIM_MOE_DISABLE(&htim14); //disable main output of TIM14 - i.e. PWM output OFF */

	/*__enable_irq(); //enable interrupts*/

	/*//START ADC TRIG. TIMER
	__HAL_TIM_SET_COUNTER(&htim17, 0); //set counter to 0
	__HAL_TIM_ENABLE(&htim17); //enable TIM17
	HAL_TIM_OC_Start(&htim17, TIM_CHANNEL_1); //enable ADC triggering - if this doesn't work use HAL_TIM_Base_Start()*/

	/*//WAIT
	while(initial_ADC_conversion_complete == 0){}; //wait while first ADC conversion is ongoing

	process_TIM16_raw_start_value_and_prescaler();
	process_TIM16_final_start_value_and_prescaler_adjust();

	//START FREQ. GEN and PWM GEN TIMERS and ENABLE PWM OUTPUT
	__HAL_TIM_SET_COUNTER(&htim14, 0); //set TIM14 counter to 0
	__HAL_TIM_SET_COUNTER(&htim14, 0); //set TIM16 counter to 0
	__HAL_TIM_ENABLE(&htim14); //enable TIM14
	__HAL_TIM_ENABLE(&htim16); //enable TIM16
	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1); //start PWM
	HAL_TIM_OC_Start(&htim16, TIM_CHANNEL_1); //start freq. gen - if this doesn't work use HAL_TIM_Base_Start()
	*/
	while (1)
	{
		/*process_TIM16_raw_start_value_and_prescaler();
		process_TIM16_final_start_value_and_prescaler_adjust();*/
	}
	return 1;
}




