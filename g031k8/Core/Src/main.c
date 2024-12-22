/*NOTES ON THIS BUILD:

*/

//INCLUDES
#include "system.h"

int main(void)
{
	System_Init();

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


	//START FREQ. GEN and PWM GEN TIMERS and ENABLE PWM OUTPUT
	Start_PWM_Gen_Timer();
	Start_Freq_Gen_Timer();

	//ENABLE INTERRUPTS
	Global_Interrupt_Enable();

	while (1)
	{

	}
	return 1;
}




