/*NOTES ON THIS BUILD:
Every works basically perfectly until the speed is decreased through half_scale, at which point you can no longer control the speed!

Also literally the only issue with the symmetry with the speed above half-scale is that it gets slightly quicker when the symmetry increases (both CW and CCW equally)
so I imagine that the shortened period needs to be increased, because it is too quick, or the lengthened period needs to be increased because it is too quick.
*/

//INCLUDES
#include "system.h"

int main(void)
{
	System_Init();

	//ENABLE INTERRUPTS
	Global_Interrupt_Enable();

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions);

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

	}

	return 1;
}




