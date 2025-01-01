/*NOTES ON THIS BUILD:

*/

//INCLUDES
#include "system.h"

int main(void)
{
	System_Init();

	//duty_delay_line_read_pointer_offset = 255; //phase difference between the two waves

	current_depth = 255; //whilst depth pot is being used as delay adjust

	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE); //make sure the overflow (update) interrupt is enabled for TIM2

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions);

	//WAIT
	while(initial_ADC_conversion_complete == NO){}; //wait while first ADC conversion is ongoing

	HAL_ADC_Stop_DMA(&hadc1);

	//START FREQ. GEN and PWM GEN TIMERS and ENABLE PWM OUTPUT
	Start_PWM_Gen_Timer_Main_Oscillator();
	Start_PWM_Gen_Timer_Secondary_Oscillator();
	Start_Freq_Gen_Timer();
	HAL_GPIO_WritePin(TAP_TEMPO_TOGGLE_GPIO_Port, TAP_TEMPO_TOGGLE_Pin, 1);
	Start_Input_Capture_Timer();

	//ENABLE INTERRUPTS
	Global_Interrupt_Enable();

	HAL_Delay(4000);
	HAL_GPIO_WritePin(TAP_TEMPO_TOGGLE_GPIO_Port, TAP_TEMPO_TOGGLE_Pin, 0);
	HAL_Delay(50);
	HAL_GPIO_WritePin(TAP_TEMPO_TOGGLE_GPIO_Port, TAP_TEMPO_TOGGLE_Pin, 1);
	HAL_Delay(550);
	HAL_GPIO_WritePin(TAP_TEMPO_TOGGLE_GPIO_Port, TAP_TEMPO_TOGGLE_Pin, 0);
	HAL_Delay(50);
	HAL_GPIO_WritePin(TAP_TEMPO_TOGGLE_GPIO_Port, TAP_TEMPO_TOGGLE_Pin, 1);


	while (1)
	{


	}
	return 1;
}




