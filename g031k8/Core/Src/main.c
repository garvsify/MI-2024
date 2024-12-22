/*NOTES ON THIS BUILD:

*/

//INCLUDES
#include "system.h"

int main(void)
{
	System_Init();

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions);

	//WAIT
	while(initial_ADC_conversion_complete == NO){}; //wait while first ADC conversion is ongoing

	HAL_ADC_Stop_DMA(&hadc1);

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




