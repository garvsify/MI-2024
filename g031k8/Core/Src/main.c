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


	hadc1.Instance->CHSELR = ADC_CHANNEL_0;
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 0.1);
	uint16_t ADC_result = (uint16_t)HAL_ADC_GetValue(&hadc1); //set ADC_Result to waveshape index value

	if(ADC_result <= TRIANGLE_MODE_ADC_THRESHOLD){
		current_waveshape = TRIANGLE_MODE; //triangle wave
	}
	else if (ADC_result <= SINE_MODE_ADC_THRESHOLD){
		current_waveshape = SINE_MODE; //sine wave
	}
	else if (ADC_result <= SQUARE_MODE_ADC_THRESHOLD){
		current_waveshape = SQUARE_MODE; //square wave
	}
	else{
		current_waveshape = SINE_MODE; //if error, return sine
	}



	hadc1.Instance->CHSELR = ADC_CHANNEL_1;
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 0.1);
	current_speed_linear = (uint16_t)HAL_ADC_GetValue(&hadc1) >> 2; //convert to 10-bit



	hadc1.Instance->CHSELR = ADC_CHANNEL_4;
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 0.1);
	current_depth = (uint16_t)HAL_ADC_GetValue(&hadc1) >> 4; //convert to 8-bit



	hadc1.Instance->CHSELR = ADC_CHANNEL_5;
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 0.1);
	#if SYMMETRY_ADC_RESOLUTION == 10
		current_symmetry = (uint16_t)HAL_ADC_GetValue(&hadc1) >> 2;

	#endif

	#if SYMMETRY_ADC_RESOLUTION == 8
		current_symmetry = (uint16_t)HAL_ADC_GetValue(&hadc1) >> 4;

	#endif

	#if SYMMETRY_ADC_RESOLUTION == 12
		current_symmetry = (uint16_t)HAL_ADC_GetValue(&hadc1);

	#endif



	//PROCESS RAW AND FINAL FREQ. GEN. TIMER START VALUES AND PRESCALER
	Process_TIM16_Raw_Start_Value_and_Raw_Prescaler();
	Process_TIM16_Final_Start_Value_and_Prescaler_Adjust();

	//START FREQ. GEN and PWM GEN TIMERS and ENABLE PWM OUTPUT
	Start_PWM_Gen_Timer();
	Start_Freq_Gen_Timer();

	while (1)
	{
		if(isr_done == YES){

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

			Global_Interrupt_Enable(); //DO NOT DELETE
		}
	}
	return 1;
}




