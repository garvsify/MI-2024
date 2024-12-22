/*NOTES ON THIS BUILD:
Every works basically perfectly until the speed is decreased through half_scale, at which point you can no longer control the speed!
*/

//INCLUDES
#include "system.h"

static ADC_ChannelConfTypeDef ADC_CH_Cfg = {0};

int main(void)
{
	System_Init();

	ADC_CH_Cfg.Channel = ADC_CHANNEL_0;
	HAL_ADC_ConfigChannel(&hadc1, &ADC_CH_Cfg);
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

	ADC_CH_Cfg.Channel = ADC_CHANNEL_1;
	HAL_ADC_ConfigChannel(&hadc1, &ADC_CH_Cfg);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 0.1);
	current_speed = (uint16_t)HAL_ADC_GetValue(&hadc1) >> 2; //convert to 10-bit



	ADC_CH_Cfg.Channel = ADC_CHANNEL_4;
	HAL_ADC_ConfigChannel(&hadc1, &ADC_CH_Cfg);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 0.1);
	current_depth = (uint16_t)HAL_ADC_GetValue(&hadc1) >> 4; //convert to 8-bit



	ADC_CH_Cfg.Channel = ADC_CHANNEL_5;
	HAL_ADC_ConfigChannel(&hadc1, &ADC_CH_Cfg);
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
	//ENABLE INTERRUPTS
	Global_Interrupt_Enable();
	Start_PWM_Gen_Timer();
	Start_Freq_Gen_Timer();

	while (1)
	{

	}
	return 1;
}




