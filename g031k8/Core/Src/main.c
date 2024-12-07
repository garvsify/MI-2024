//INCLUDES
#include "system.h"

int main(void)
{
	System_Init();

	//ENABLE INTERRUPTS
	Global_Interrupt_Enable();

	//START ADC TRIG. TIMER
	//Start_ADC_Trig_Timer();
	speed_parameters.ADC_values_ready = NO;
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions);

	//WAIT
	while(initial_ADC_conversion_complete == NO){}; //wait while first ADC conversion is ongoing

	//PROCESS RAW AND FINAL FREQ. GEN. TIMER START VALUES AND PRESCALER
	Process_TIM16_Raw_Start_Value_and_Raw_Prescaler(&speed_parameters);
	Process_TIM16_Final_Start_Value_and_Prescaler_Adjust(&speed_parameters);

	//START FREQ. GEN and PWM GEN TIMERS and ENABLE PWM OUTPUT
	Start_PWM_Gen_Timer();
	Start_Freq_Gen_Timer();

	while (1)
	{
		if(TIM16_callback_active == NO){

			//if((speed_parameters.raw_values_calculated == NO) && (speed_parameters.symmetry_adjusted_values_calculated == NO)){
			if(speed_parameters.ADC_values_ready == YES){

				//HAL_GPIO_TogglePin(SYM_PROC_GPIO_Port, SYM_PROC_Pin);

				Global_Interrupt_Disable(); //DO NOT DELETE

				Process_TIM16_Raw_Start_Value_and_Raw_Prescaler(&speed_parameters);
				Process_TIM16_Final_Start_Value_and_Prescaler_Adjust(&speed_parameters);

				//HAL_GPIO_TogglePin(SYM_PROC_GPIO_Port, SYM_PROC_Pin);

				Global_Interrupt_Enable(); //DO NOT DELETE
			}
		}
	}
	return 1;
}




